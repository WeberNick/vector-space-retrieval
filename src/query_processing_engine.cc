#include "query_processing_engine.hh"
#include "index_manager.hh"
#include "measure.hh"
#include "posting_list.hh"

#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

/**
 * @brief Construct a new Query Processing Engine:: Query Processing Engine object
 */
QueryProcessingEngine::QueryProcessingEngine() : _cb(nullptr), _init(false), _stopwordFile() {}

/**
 * @brief Initializes the singleton
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

/**
 * @brief Reads a stopwords file for the function @see QueryProcessingEngine#createQueryDoc
 *
 * @param aFile The file containing stopwords separated by ","
 */
void QueryProcessingEngine::read(const std::string& aFile) {
    std::ifstream file(aFile);
    std::string line;
    while (std::getline(file, line)) {
        Utility::StringOp::splitStringBoost(line, ',', this->_stopword_list);
    }
}

/**
 * @brief Creates a preprocessed document query out of a string
 *
 * @param query The raw query string
 * @return Document
 */
Document QueryProcessingEngine::createQueryDoc(std::string& query) {
    Utility::IR::removeStopword(query, getStopwordlist()); // Remove stopwords
    Utility::StringOp::trim(query);                        // Trim whitespaces

    string_vt proc_query;
    Utility::StringOp::splitString(query, ' ', proc_query);   // Split string by whitespaces
    Utility::StringOp::removeEmptyStringsFromVec(proc_query); // Remove eventually empty strings from the query term vector

    std::vector<std::string> preprocessed_content;
    for (auto& elem : proc_query) { // Preprocess query
        std::string preprocess = Utility::IR::stemPorter(elem);
        std::cout << "Preprocessed: " << preprocess << std::endl;
        preprocessed_content.push_back(preprocess);
    }

    return Document("query-0", preprocessed_content);
}

/**
 * @brief A top level implementation of the search function. Use a string and type to search for similar documents
 *
 * @param query The raw string query
 * @param topK How many results are retrieved
 * @param searchType What type of search should be executed
 * @return pair_sizet_float_vt A list of document - similarity pairs ordered descending
 */
const pair_sizet_float_vt QueryProcessingEngine::search(std::string& query, size_t topK, IR_MODE searchType) {
    Document queryDoc = this->createQueryDoc(query);

    std::cout << "Searching for: ";
    for (size_t j = 0; j < queryDoc.getContent().size(); ++j) {
        std::cout << "(" << j << "|" << queryDoc.getContent()[j] << ")"
                  << " ";
    }
    std::cout << std::endl;

    pair_sizet_float_vt found_indices; // result vector

    switch (searchType) {
    case IR_MODE ::kVANILLA: {
        found_indices = QueryProcessingEngine::searchCollectionCos(&queryDoc, DocumentManager::getInstance().getIDs(), topK);
    } break;
    case IR_MODE ::kCLUSTER: {
        std::vector<std::pair<size_t, float>> leader_indexes =
            QueryProcessingEngine::searchClusterCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeaders(), 0);

        // Get docIds from the clusters to search in, vector will be filled from the IndexManager::getInstance().getClusteredIndex().getIDs() method
        sizet_vt clusterDocIds;
        IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK, clusterDocIds);

        // Search the docs from the clusters
        found_indices = QueryProcessingEngine::searchClusterCos(&queryDoc, clusterDocIds, topK);
    } break;
    case IR_MODE ::kTIERED: {
        found_indices = QueryProcessingEngine::searchTieredCos(&queryDoc, DocumentManager::getInstance().getIDs(), topK);
    } break;
    case IR_MODE ::kNoMode: break;
    case IR_MODE ::kNumberOfModes: break;
    case IR_MODE ::kRANDOM: break;
    }

    // Return search result
    return found_indices;
}

/**
 * @brief Search function for searching the whole document collection
 *
 * @param query A preprocessed query document
 * @param collectionIds IDs of docs to search in
 * @param topK How many results are retrieved
 * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
 */
const pair_sizet_float_vt QueryProcessingEngine::searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {
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
                float idf = IndexManager::getInstance().getIdf(term);
                docId2Scores[id] += (tf * idf * (Utility::IR::calcTf(term, qcontent) * idf));
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

/**
 * @brief Search function for searching the cluster representation
 *
 * @param query A preprocessed query document
 * @param collectionIds IDs of docs to search in
 * @param topK How many results are retrieved
 * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
 */
const pair_sizet_float_vt QueryProcessingEngine::searchClusterCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {
    std::map<size_t, float> docId2Scores;

    for (auto& elem : collectionIds) {
        docId2Scores[elem] = Utility::SimilarityMeasures::calcCosDist(*query, DocumentManager::getInstance().getDocument(elem));
    }

    // Convert map into vector of pairs
    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) {
        results.push_back(elem);
    }

    // Sort vector desc
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });
    return (!topK) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
    return results;
}

/**
 * @brief Just a wrapper function for @see QueryProcessingEngine::searchClusterCos to retrieve the docId of the most similar doc
 *
 * @param query A preprocessed query document
 * @param collectionIds IDs of docs to search in
 * @return const size_t docId of the most similar doc
 */
const size_t QueryProcessingEngine::searchClusterCosFirstIndex(const Document* query, const sizet_vt& collectionIds) {
    return QueryProcessingEngine::getInstance().searchClusterCos(query, collectionIds, 1)[0].first; // get most similar leader
}

/**
 * @brief Search function for Search function for searching the tiered index representation
 *
 * @param query A preprocessed query document
 * @param collectionIds IDs of docs to search in
 * @param topK How many results are retrieved
 * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
 */
const pair_sizet_float_vt QueryProcessingEngine::searchTieredCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {

    /*
        std::map<size_t, float> docId2Length;
        for (auto& elem : collectionIds) { // Map of doc id to length og that doc
            docId2Length[elem] = DocumentManager::getInstance().getDocumentMap().at(elem).getNormLength();
        }

        std::map<size_t, float> docId2Scores;
        const string_vt& qcontent = query->getContent();
        for (const auto& term :
             qcontent) { // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
            try {
                const PostingList& postingList = IndexManager::getInstance().getTieredIndex().getPostingList(term, //tier);
                for (auto& [id, tf] : postingList.getPosting()) {
                    float idf = IndexManager::getInstance().getIdf(term);
                    docId2Scores[id] += (tf * idf * (Utility::IR::calcTf(term, qcontent) * idf));
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
        */
}

/**
 * @brief Search function for searching when random projections are used
 *
 * @param query A preprocessed query document
 * @param collectionIds IDs of docs to search in
 * @param topK How many results are retrieved
 * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
 */
const pair_sizet_float_vt QueryProcessingEngine::searchRandomProjCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {
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
