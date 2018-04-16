#include "query_processing_engine.hh"
#include "index_manager.hh"
#include "posting_list.hh"
#include "utility.hh"
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <iostream>
#include <sstream>

using Comparator = std::function<bool(std::pair<size_t, float>, std::pair<size_t, float>)>;

using namespace ::boost;
using namespace ::boost::multi_index;

// define a multiply indexed set with indices by id and name
typedef multi_index_container<QueryProcessingEngine::SearchHelper,
                              indexed_by<
                                  // sort by employee::operator<
                                  ordered_unique<identity<QueryProcessingEngine::SearchHelper>>,

                                  // sort by less<string> on name
                                  ordered_non_unique<member<QueryProcessingEngine::SearchHelper, double, &QueryProcessingEngine::SearchHelper::score>>>>
    doc2scores_set;

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
        this->_stopword_list.push_back(Utility::StringOp::trim_copy(line));
    }
}

/**
 * @brief Returns the ids of the most similar document in \collection
 *
 * @param query
 * @param collection
 * @return
 */
std::vector<size_t> QueryProcessingEngine::cosineScore(const Document* query, const doc_mt& collection, size_t topK) {

    unsigned long count = collection.size();

    // Map of doc id to scores
    std::map<size_t, float> docId2Scores;
    // Map of doc id to length og that doc
    std::map<size_t, float> docId2Length; // TODO: maybe this can be deleted if we can guarantee the order of the order in collection and scores, but I
                                          // guess
    // so, this is not the implementation like in the IR Book

    // Fill length vector with the lengths of the documents
    for (auto& elem : collection) {
        docId2Length[elem.first] = elem.second.getContent().size();
    }

    // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
    for (size_t j = 0; j < query->getContent().size(); ++j) {
        std::cout << "Retrieve posting list for term: " << query->getContent()[j] << std::endl;

        try {
            const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(query->getContent()[j]);
            for (auto& posting : postingList.getPosting()) {

                docId2Scores[posting.first] +=
                    (posting.second * postingList.getIdf() * (Utility::IR::calcTF(query->getContent()[j], query->getContent()) * postingList.getIdf()));
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
const size_t QueryProcessingEngine::cosineScoreCluster(Document* query, const std::vector<Document*>& collection) {

    unsigned long count = collection.size();

    // Map of doc id to scores
    std::map<size_t, float> docId2Scores;
    // Map of doc id to length og that doc
    std::map<size_t, float> docId2Length; // TODO: maybe this can be deleted if we can guarantee the order of the order in collection and scores, but I guess
                                          // so, this is not the implementation like in the IR Book

    // Fill length vector with the lengths of the documents
    for (int i = 0; i < count; ++i) {
        docId2Length[collection[i]->getID()] = collection[i]->getContent().size();
    }

    // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
    for (size_t j = 0; j < query->getContent().size(); ++j) {
        const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(query->getContent()[j]);

        for (auto& posting : postingList.getPosting()) {
            docId2Scores[posting.first] +=
                (posting.second * postingList.getIdf()) * (Utility::IR::calcTF(query->getContent()[j], query->getContent()) * postingList.getIdf());
        }
    }

    // Divide every score of a doc by the length of the document
    for (const auto& elem : docId2Length) {
        docId2Scores[elem.first] = docId2Scores[elem.first] / docId2Length[elem.first];
    }

    // Sort the map descending into a set with labmda
    std::set<std::pair<size_t, float>, Comparator> setOfCounts(
        docId2Scores.begin(), docId2Scores.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });

    // Retrieve the first (most similar document)
    std::pair<size_t, float> first = *setOfCounts.begin();

    // Search for the docID of the first element in the collection and return it
    for (size_t k = 0; k < collection.size(); ++k) {
        if (collection[k]->getID() == first.first) { return k; }
    }

    // Most similar doc not found
    return static_cast<const size_t>(-1);
}
std::vector<size_t> QueryProcessingEngine::search(Document* query, size_t topK) {






    std::vector<std::string> preprocessed_content;


    for (auto& elem : query->getContent()) {
        preprocessed_content.push_back(Utility::IR::stemPorter(elem));
    }

    Document proc_query(query->getDocID(), preprocessed_content);

    std::cout << "Searching for: ";

    for (int j = 0; j < proc_query.getContent().size(); ++j) {
        std::cout << "(" << j << "|" << proc_query.getContent()[j] << ")"
                  << " ";
    }
    std::cout << std::endl;

    std::vector<size_t> found_indexes = QueryProcessingEngine::cosineScore(&proc_query, DocumentManager::getInstance().getDocumentMap(), topK);
    return found_indexes;
}
