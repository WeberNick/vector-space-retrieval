#include "query_processing_engine.hh"
#include "posting_list.hh"
#include "utility.hh"
#include <iostream>
#include <sstream>

typedef std::function<bool(std::pair<std::string, int>, std::pair<std::string, int>)> Comparator;

/**
 * @brief Returns the ids of the most similar document in \collection
 *
 * @param query
 * @param collection
 * @return
 */
std::vector<size_t> QueryProcessingEngine::cosineScore(const Document* query, const doc_mt &collection, size_t topK) {
/*
    unsigned long count = collection.size();

    // Map of doc id to scores
    std::map<size_t, float> docId2Scores;
    // Map of doc id to length og that doc
    std::map<size_t, float> docId2Length; // TODO: maybe this can be deleted if we can guarantee the order of the order in collection and scores, but I guess
    // so, this is not the implementation like in the IR Book

    // Fill length vector with the lengths of the documents

    for(auto &elem: collection){
        docId2Scores[elem.first] = elem.second.getContent().size();
    }

    // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
    for (size_t j = 0; j < query->getContent().size(); ++j) {
        const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(query->getContent()[j]);
        for (auto& posting : postingList) {
            docId2Scores[posting.docId] +=
                (posting.getTF * postingList.getIDF()) * (Utility::IR::calcTF(query->getContent()[j], query->getContent()) * postingList.getIDF());
        }
    }

    // Divide every score of a doc by the length of the document
    for (const auto& elem : docId2Length) {
        docId2Scores[elem.first] = docId2Scores[elem.first] / docId2Length[elem.first];
    }

    // Sort the map ascending into a set with lamda
    std::set<std::pair<size_t, float>, Comparator> setOfCounts(
        docId2Scores.begin(), docId2Scores.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });

    std::vector<size_t> topKIndexes(topK);
    int counter = 0;

    // Fill vector with the first topK doc ids
    for (auto& elem : setOfCounts) {
        if (counter == topK) { break; }
        topKIndexes.push_back(elem.first);
        counter++;
    }

    return topKIndexes;*/
    return std::vector<size_t>();
}

/**
 * @brief Returns the index of the most similar document in \collection
 *
 * @param query
 * @param collection
 * @return
 */
const size_t QueryProcessingEngine::cosineScoreCluster(Document* query, const std::vector<Document*>& collection) {
    /*
    unsigned long count = collection.size();

    // Map of doc id to scores
    std::map<size_t, float> docId2Scores;
    // Map of doc id to length og that doc
    std::map<size_t, float> docId2Length; // TODO: maybe this can be deleted if we can guarantee the order of the order in collection and scores, but I guess
                                          // so, this is not the implementation like in the IR Book

    // Fill length vector with the lengths of the documents
    for (int i = 0; i < count; ++i) {
        docId2Scores[collection[i]->getID()] = collection[i]->getContent().size();
    }

    // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
    for (size_t j = 0; j < query->getContent().size(); ++j) {
        const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(query->getContent()[j]);
        for (auto& posting : postingList) {
            docId2Scores[posting.docId] +=
                (posting.getTF * postingList.getIDF()) * (Utility::IR::calcTF(query->getContent()[j], query->getContent()) * postingList.getIDF());
        }
    }

    // Divide every score of a doc by the length of the document
    for (const auto& elem : docId2Length) {
        docId2Scores[elem.first] = docId2Scores[elem.first] / docId2Length[elem.first];
    }

    // Sort the map ascending into a set with lamda
    std::set<std::pair<size_t, float>, Comparator> setOfCounts(
        docId2Scores.begin(), docId2Scores.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });

    // Retrieve the first (most similar document)
    std::pair<size_t, float> first = *setOfCounts.begin();

    // Search for the docID of the first element in the collection and return it
    for (size_t k = 0; k < collection.size(); ++k) {
        if (collection[k]->getID() == first.first) { return k; }
    }

    // Most similar doc not found
    return static_cast<const size_t>(-1);*/
    return 1;
}
void QueryProcessingEngine::search(const Document *query, size_t topK) {
    QueryProcessingEngine::cosineScore(query, DocumentManager::getInstance().getDocumentMap(), topK);
}
