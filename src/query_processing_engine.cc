#include "query_processing_engine.hh"
#include "index_manager.hh"
#include "posting_list.hh"
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

using Comparator = std::function<bool(std::pair<size_t, float>, std::pair<size_t, float>)>;

/**
 * @brief Construct a new Query Processing Engine:: Query Processing Engine object
 */
QueryProcessingEngine::QueryProcessingEngine() : _cb(nullptr), _init(false), _stopwordFile() {}

/**
 * @brief initializes the singleton
 *
 * @param aControlBlock
 */
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

pair_sizet_float_vt QueryProcessingEngine::search(std::string& query, size_t topK, IR_MODE searchType) {
    Utility::IR::removeStopword(query, getStopwordlist()); // Remove stopwords
    Utility::StringOp::trim(query);                        // Trim whitespaces

    string_vt proc_query;
    Utility::StringOp::splitString(query, ' ', proc_query);   // Split string by whitespaces
    Utility::StringOp::removeEmptyStringsFromVec(proc_query); // Remove eventually empty strings from the query term vector

    pair_sizet_float_vt found_indices; // result vector

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
    case IR_MODE::kVANILLA: found_indices = QueryProcessingEngine::searchCollectionCos(&queryDoc, DocumentManager::getInstance().getDocumentMap(), topK); break;
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

pair_sizet_float_vt QueryProcessingEngine::searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {
    std::map<size_t, float> docId2Length;
    for (auto& elem : collectionIds) { // Map of doc id to length og that doc
        docId2Length[elem] = DocumentManager::getInstance().getDocumentMap().at(elem).getNormLength();
    }

    std::map<size_t, float> docId2Scores;
    const string_vt& qcontent = query->getContent();
    for (const auto& term :
         qcontent) { // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
        try {
            const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(term);
            for (auto& [id, tf] : postingList.getPosting()) {
                // check if current doc looked at, also in the collection we want to search in, because we only have global posting list index
                if (std::find(collectionIds.begin(), collectionIds.end(), id) != collectionIds.end()) {
                    docId2Scores[id] += (tf * postingList.getIdf() * (Utility::IR::calcTf(term, qcontent) * postingList.getIdf()));
                }
            }
        } catch (const InvalidArgumentException& e) { std::cout << e.what() << std::endl; }
    }

    for (const auto& elem : docId2Length) { // Divide every score of a doc by the length of the document
        docId2Scores[elem.first] = docId2Scores[elem.first] / docId2Length[elem.first];
    }

    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) { // Convert map into vector of pairs
        results.push_back(elem);
    }

    // Sort vector desc
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });
    return (!topK) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}

const size_t QueryProcessingEngine::searchCollectionCosFirstIndex(const Document* query, const sizet_vt& collectionIds) {
    return QueryProcessingEngine::getInstance().searchCollectionCos(query, collectionIds, 1)[0].first; // get most similar leader
}

pair_sizet_float_vt QueryProcessingEngine::cosineScoreLSHSearch(const Document* query, const sizet_vt& collectionIds, size_t topK) {
    std::map<size_t, float> docId2Scores;

    // for (auto& elem : collectionIds) {
    // docId2Scores[elem] = Utility::SimilarityMeasures::calcHammingDist(query.getLSHBitvec(),
    // DocumentManager::getInstance().getDocumentMap().at(elem).getLSHBitvec());
    //}

    // Convert map into vector of pairs
    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) {
        results.push_back(elem);
    }

    // Sort vector desc
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });
    return (!topK) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}
