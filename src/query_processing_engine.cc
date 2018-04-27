#include "query_processing_engine.hh"
#include "index_manager.hh"
#include "posting_list.hh"
#include "utility.hh"
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

using Comparator = std::function<bool(std::pair<size_t, float>, std::pair<size_t, float>)>;


QueryProcessingEngine::QueryProcessingEngine() :
    _cb(nullptr), _init(false), _stopwordFile("./data/stopwords.large") // relative path from /path/to/repo/vector-space-retrieval
{}

void QueryProcessingEngine::init(const control_block_t& aControlBlock) {
    _cb = &aControlBlock;
    if (!_init) {
        read(_stopwordFile);
        _init = true;
    }
}

void QueryProcessingEngine::read(const std::string& aFile) {
    std::ifstream file(aFile);
    std::string line;
    while (std::getline(file, line)) {
        Utility::StringOp::splitStringBoost(line, ',', this->_stopword_list);
    }
}

/**
 * @brief Entry point for the search. Here the query will get preprocessed and it gets decided which search type to execute
 *
 * @param query The actual query
 * @param topK  How many results to retrieve
 * @param searchType What type of search to we process
 * @return
 */
std::vector<size_t> QueryProcessingEngine::search(Document* query, size_t topK, Utility::VSMType searchType) {

  // Preprocess query
  std::vector<std::string> preprocessed_content;

  for (auto& elem : query->getContent()) {
    preprocessed_content.push_back(Utility::IR::stemPorter(elem));
  }

  Document proc_query(query->getDocID(), preprocessed_content);

  std::cout << "Searching for: ";

  for (size_t j = 0; j < proc_query.getContent().size(); ++j) {
    std::cout << "(" << j << "|" << proc_query.getContent()[j] << ")"
              << " ";
  }
  std::cout << std::endl;

  std::vector<size_t> found_indexes;

  switch (searchType) {
    case Utility::VSMType::VANILLA:
      found_indexes = QueryProcessingEngine::cosineScoreVanillaSearch(&proc_query, DocumentManager::getInstance().getDocumentMap(), topK);
      break;
    case Utility::VSMType::CLUSTER:
      found_indexes = QueryProcessingEngine::cosineScoreClusterSearch(&proc_query, IndexManager::getInstance().getClusteredIndex().getLeaders(), topK);
      break;
  }

  return found_indexes;
}

/**
 * @brief Returns the ids of the most similar document in \collection - Main search funtion
 *
 * @param query The query we want to search
 * @param collection The collection we want to search in
 * @return
 */
std::vector<size_t> QueryProcessingEngine::cosineScoreVanillaSearch(const Document* query, const doc_mt& collection, size_t topK) {

    // unsigned long count = collection.size();

    // Map of doc id to scores
    std::map<size_t, float> docId2Scores;
    std::set<size_t> doc_ids;
    // Map of doc id to length og that doc
    std::map<size_t, float> docId2Length; // TODO: maybe this can be deleted if we can guarantee the order of the order in collection and scores, but I
                                          // guess
    // so, this is not the implementation like in the IR Book

    // Fill length vector with the lengths of the documents
    for (auto& elem : collection) {
        docId2Length[elem.first] = elem.second.getContent().size();
        doc_ids.insert(elem.first);
    }

    // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
    for (size_t j = 0; j < query->getContent().size(); ++j) {
        std::cout << "Retrieve posting list for term: " << query->getContent()[j] << std::endl;

        try {
            const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(query->getContent()[j]);
            for (auto& posting : postingList.getPosting()) {
                const bool is_in = doc_ids.find(posting.first) != doc_ids.end(); // check if current doc looked at, also in the collection we want to search in, because we only have global posting list index
                if (is_in) {
                    docId2Scores[posting.first] +=
                        (posting.second * postingList.getIdf() * (Utility::IR::calcTf(query->getContent()[j], query->getContent()) * postingList.getIdf()));
                }
            }
        } catch (const InvalidArgumentException& e) { std::cout << "Error thrown" << std::endl; }
    }

    // Divide every score of a doc by the length of the document
    for (const auto& elem : docId2Length) {
        docId2Scores[elem.first] = docId2Scores[elem.first] / docId2Length[elem.first];
    }

    // Sort the map descending into a set with lambda
    std::set<std::pair<size_t, float>, Comparator> setOfCounts(
        docId2Scores.begin(), docId2Scores.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });

    std::vector<size_t> topKIndexes;
    size_t counter = 0;

    // Fill vector with the first topK doc ids
    for (auto& elem : setOfCounts) {
        if (counter == topK) { break; }
        topKIndexes.push_back(elem.first);
        counter++;
    }

    return topKIndexes;
}

/**
 * @brief Returns the index of the most similar document in \collection
 *
 * @param query
 * @param collection
 * @return
 */
size_t QueryProcessingEngine::cosineScoreClusterBuilding(const Document* query, const doc_ptr_vt& collection) {

    std::map<size_t, float> docId2Scores;

    // TODO: Normal cosine sim calculation without fancy algo
    for (auto& doc : collection) {
        docId2Scores[doc->getID()] = Utility::SimilarityMeasures::calcCosSim(*query, *doc);
    }

    // Sort the map descending into a set with lambda
    std::set<std::pair<size_t, float>, Comparator> setOfCounts(
        docId2Scores.begin(), docId2Scores.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });

    // Retrieve the first (most similar document)
    std::pair<size_t, float> first = *setOfCounts.begin();

    // Search for the docID of the first element in the collection and return it
    int counter = 0;
    for (size_t k = 0; k < collection.size(); ++k) {
        if (collection[k]->getID() == first.first) { return k; }
        ++counter;
    }
    // Most similar doc not found
    return static_cast<const size_t>(-1);
}

std::vector<size_t> QueryProcessingEngine::cosineScoreClusterSearch(const Document* query, const doc_ptr_vt& leaders, size_t topK) {

    std::map<size_t, float> docId2Scores;
    // Map of doc id to length og that doc
    std::map<size_t, float> docId2Length;
    std::set<size_t> leader_ids;

    // Fill length vector with the lengths of the documents
    for (auto& elem : leaders) {
        docId2Length[elem->getID()] = elem->getContent().size();
        leader_ids.insert(elem->getID());
    }

    // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
    for (size_t j = 0; j < query->getContent().size(); ++j) {
        std::cout << "Retrieve posting list for term: " << query->getContent()[j] << std::endl;

        try {
            const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(query->getContent()[j]);
            for (auto& posting : postingList.getPosting()) {
                const bool is_in = leader_ids.find(posting.first) != leader_ids.end(); // check if current doc looked at, also in the collection we want to search in, because we only have global posting list index
                if (is_in) {
                    docId2Scores[posting.first] +=
                        (posting.second * postingList.getIdf() * (Utility::IR::calcTf(query->getContent()[j], query->getContent()) * postingList.getIdf()));
                }
            }
        } catch (const InvalidArgumentException& e) { std::cout << "Error thrown" << std::endl; }
    }

    // Divide every score of a doc by the length of the document
    for (const auto& elem : docId2Length) {
        docId2Scores[elem.first] = docId2Scores[elem.first] / docId2Length[elem.first];
    }

    // Sort the map descending into a set with lambda
    std::set<std::pair<size_t, float>, Comparator> setOfCounts(
        docId2Scores.begin(), docId2Scores.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });

    // Now begin to iterate and build result index vector -
    // If there are not enough docs in one vector the next most similar one is taken and it's cluster is searched
    std::vector<size_t> result;
    auto iterator = setOfCounts.begin();
    const Document* chosenLeader;


    while (result.size() < topK) {

        // Find leader in leaders vector
        for (size_t k = 0; k < leaders.size(); ++k) {
            if (leaders[k]->getID() == (*iterator).first) { chosenLeader = leaders[k]; }
        }
        std::cout << "Leader: " << chosenLeader->getID() << "is searched" << std::endl;

        std::vector<size_t> leaderRes =
            QueryProcessingEngine::cosineScoreVanillaSearch(query, IndexManager::getInstance().getClusteredIndex().getCluster().at(chosenLeader), topK);
        result.insert(result.end(), leaderRes.begin(), leaderRes.end());
        iterator++;
    }



    return result;
}

std::vector<size_t> QueryProcessingEngine::cosineScoreLSHSearch(const Document* query, const doc_mt& collection, size_t topK) { return std::vector<size_t>(); }
