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
std::vector<std::pair<size_t, float>> QueryProcessingEngine::search(std::vector& query, size_t topK, IR_MODE searchType) {

    // Remove stopwords
    Utility::IR::removeStopword(query, this->getStopwordlist());

    // Trim whitespaces
    Utility::StringOp::trim(query);

    string_vt proc_query;

    // Split string by whitespaces
    Utility::StringOp::splitString(query, ' ', proc_query);

    // Remove eventually empty strings from the query term vector
    Utility::StringOp::removeEmptyStringsFromVec(proc_query);

    // Create doc from query vector
    Document doc("0", proc_query);

    // Preprocess query
    std::vector<std::string> preprocessed_content;

    for (auto& elem : proc_query) {
        preprocessed_content.push_back(Utility::IR::stemPorter(elem));
    }

    Document queryDoc(0, preprocessed_content);

    std::cout << "Searching for: ";

    for (size_t j = 0; j < queryDoc.getContent().size(); ++j) {
        std::cout << "(" << j << "|" << queryDoc.getContent()[j] << ")"
                  << " ";
    }
    std::cout << std::endl;

    // Set up result vector
    std::vector<std::pair<size_t, float>> found_indexes;

    // Execute different search workflows based on the search type
    switch (searchType) {
    case IR_MODE::kVANILLA: found_indexes = QueryProcessingEngine::searchCollectionCos(&queryDoc, DocumentManager::getInstance().getDocumentMap(), topK); break;
    case IR_MODE::kCLUSTER:

        // Get cluster leaders sorted according to query
        std::vector<std::pair<size_t, float>> leader_indexes =
            QueryProcessingEngine::searchCollectionCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeaders(), 0);

        // Get Docs to search in
        sizet_set clusterDocIds = IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK);

        // Search the docs from the clusters
        found_indexes = QueryProcessingEngine::searchCollectionCos(&queryDoc, clusterDocIds, topK);
        break;
    }

    // Return search result
    return found_indexes;
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

/**
 * @brief Search a given set of document ids for a query with cosine similarity
 *
 * @param query
 * @param collectionIds
 * @param topK
 * @return
 */
std::vector<std::pair<size_t, float>> QueryProcessingEngine::searchCollectionCos(const Document* query, const sizet_set collectionIds, size_t topK) {

    std::map<size_t, float> docId2Scores;
    // Map of doc id to length og that doc
    std::map<size_t, float> docId2Length;
    for (auto& elem : collectionIds) {
        docId2Length[elem] = DocumentManager::getInstance().getDocumentMap().at(elem).getNormLength();
    }

    // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
    for (size_t j = 0; j < query->getContent().size(); ++j) {
        std::cout << "Retrieve posting list for term: " << query->getContent()[j] << std::endl;

        try {
            const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(query->getContent()[j]);
            for (auto& posting : postingList.getPosting()) {
                const bool is_in =
                    collectionIds.find(posting.first) !=
                    collectionIds
                        .end(); // check if current doc looked at, also in the collection we want to search in, because we only have global posting list index
                if (is_in) {
                    docId2Scores[posting.first] +=
                        (posting.second * postingList.getIdf() * (Utility::IR::calcTf(query->getContent()[j], query->getContent()) * postingList.getIdf()));
                }
            }
        } catch (const InvalidArgumentException& e) { std::cout << "Error thrown" << std::endl; }
    }

    //
    // Divide every score of a doc by the length of the document
    for (const auto& elem : docId2Length) {
        docId2Scores[elem.first] = docId2Scores[elem.first] / docId2Length[elem.first];
    }

    // Convert map into vector of pairs
    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) {
        results.push_back(elem);
    }

    // Sort vector
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });

    if (topK == 0) {
        return results;
    } else {
        return std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
    }
}

std::vector<size_t> QueryProcessingEngine::cosineScoreLSHSearch(const Document* query, const doc_mt& collection, size_t topK) { return std::vector<size_t>(); }
