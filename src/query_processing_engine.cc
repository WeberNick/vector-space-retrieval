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

/**
 * @brief Construct a new Query Processing Engine:: Query Processing Engine object
 * 
 */
QueryProcessingEngine::QueryProcessingEngine() :
    _cb(nullptr),
    _init(false),
    _stopwordFile() 
{}

void QueryProcessingEngine::init(const control_block_t& aControlBlock) {
    if (!_init) {
        _cb = &aControlBlock;
        _stopwordFile = _cb->stopwordFile(); // relative path from /path/to/repo/vector-space-retrieval
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

std::vector<std::pair<size_t, float>> QueryProcessingEngine::search(std::string& query, size_t topK, IR_MODE searchType) {
    Utility::IR::removeStopword(query, getStopwordlist()); // Remove stopwords
    Utility::StringOp::trim(query); // Trim whitespaces
    
    string_vt proc_query;
    Utility::StringOp::splitString(query, ' ', proc_query); // Split string by whitespaces
    Utility::StringOp::removeEmptyStringsFromVec(proc_query); // Remove eventually empty strings from the query term vector

    std::vector<std::pair<size_t, float>> found_indices; // result vector

    std::vector<std::string> preprocessed_content; 
    for (auto& elem : proc_query) { // Preprocess query
        std::string preprocess = Utility::IR::stemPorter(elem);
        std::cout << "Preprocessed: " << preprocess << std::endl;
        preprocessed_content.push_back(preprocess);
    }

    Document queryDoc("0", preprocessed_content);
    std::cout << "Searching for: ";
    for (size_t j = 0; j < queryDoc.getContent().size(); ++j) {
        std::cout << "(" << j << "|" << queryDoc.getContent()[j] << ")"
                  << " ";
    }
    std::cout << std::endl;
    
    switch (searchType) { // Execute different search workflows based on the search type
        case IR_MODE::kNoMode:
        case IR_MODE::kNumberOfModes:
        case IR_MODE::kTIERED:
        case IR_MODE::kRANDOM:
        case IR_MODE::kVANILLA: 
            // found_indices = QueryProcessingEngine::searchCollectionCos(&queryDoc, DocumentManager::getInstance().getDocumentMap(), topK);
            // break;
        case IR_MODE::kCLUSTER:
            // Get cluster leaders sorted according to query
            std::vector<std::pair<size_t, float>> leader_indexes =
            QueryProcessingEngine::searchCollectionCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeaders(), 0);

            sizet_vt clusterDocIds; // Get Docs to search in
            IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK, clusterDocIds);

            // Search the docs from the clusters
            found_indices = QueryProcessingEngine::searchCollectionCos(&queryDoc, clusterDocIds, topK);
            break;
    }

    return found_indices; // Return search result
}

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

std::vector<std::pair<size_t, float>> QueryProcessingEngine::searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {
    std::map<size_t, float> docId2Length;
    for (auto& elem : collectionIds) { // Map of doc id to length og that doc
        docId2Length[elem] = DocumentManager::getInstance().getDocumentMap().at(elem).getNormLength();
    }
    
    std::map<size_t, float> docId2Scores;
    const string_vt& qcontent = query->getContent();
    for (const auto& term : qcontent) { // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
        try {
            const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(term);
            for (auto& [id, tf] : postingList.getPosting()) {
                // check if current doc looked at, also in the collection we want to search in, because we only have global posting list index
                if (std::find(collectionIds.begin(), collectionIds.end(), id) != collectionIds.end()) {
                    docId2Scores[id] += (tf * postingList.getIdf() * (Utility::IR::calcTf(term, qcontent) * postingList.getIdf()));
                }
            }
        } catch (const InvalidArgumentException& e) {
            std::cout << e.what() << std::endl;
        }
    }

    for (const auto& elem : docId2Length) { // Divide every score of a doc by the length of the document
        docId2Scores[elem.first] = docId2Scores[elem.first] / docId2Length[elem.first];
    }
    
    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) { // Convert map into vector of pairs
        results.push_back(elem);
    }

    // Sort vector
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });
    return (!topK) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}

std::vector<size_t> QueryProcessingEngine::cosineScoreLSHSearch(const Document* query, const doc_mt& collection, size_t topK) {
    // TODO implement
    return std::vector<size_t>();
}
