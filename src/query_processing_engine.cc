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

void QueryProcessingEngine::read(const std::string& aFile) {
    std::ifstream file(aFile);
    std::string line;
    while (std::getline(file, line)) {
        Utility::StringOp::splitStringBoost(line, ',', this->_stopword_list);
    }
}

const pair_sizet_float_vt QueryProcessingEngine::search(std::string& query, size_t topK, IR_MODE searchType) {
    Document queryDoc = DocumentManager::getInstance().createQueryDoc(query);

    /*std::cout << "Searching for: ";
    for (size_t j = 0; j < queryDoc.getContent().size(); ++j) {
        std::cout << "(" << j << "|" << queryDoc.getContent()[j] << ")"
                  << " ";
    }
    std::cout << std::endl;*/

    pair_sizet_float_vt found_indices; // result vector

    switch (searchType) {
    case IR_MODE ::kVANILLA: {
        found_indices = QueryProcessingEngine::searchCollectionCos(&queryDoc, DocumentManager::getInstance().getIDs(), topK);
    } break;
    case IR_MODE ::kCLUSTER: {

        std::cout << "Find leader indexes" << std::endl;
        std::vector<std::pair<size_t, float>> leader_indexes =
            QueryProcessingEngine::searchClusterCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeaders(), 0);

        // Get docIds from the clusters to search in, vector will be filled from the IndexManager::getInstance().getClusteredIndex().getIDs() method
        sizet_vt clusterDocIds;
        IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK, clusterDocIds);

        // Search the docs from the clusters
        std::cout << "Search leader indexes" << std::endl;
        found_indices = QueryProcessingEngine::searchClusterCos(&queryDoc, clusterDocIds, topK);
    } break;
    case IR_MODE ::kTIERED: {
        found_indices =
            QueryProcessingEngine::searchTieredCos(&queryDoc, IndexManager::getInstance().getTieredIndex().getDocIDList(topK, queryDoc.getContent()), topK);
    } break;
    case IR_MODE ::kRANDOM: {
        found_indices = QueryProcessingEngine::searchRandomProjCos(&queryDoc, DocumentManager::getInstance().getIDs(), topK);
    } break;
    case IR_MODE ::kNoMode: break;
    case IR_MODE ::kNumberOfModes: break;
    }

    // Return search result
    return found_indices;
}

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
        } catch (const InvalidArgumentException& e) { /*std::cout << e.what() << std::endl;*/
        }
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
    return (!topK || topK > results.size()) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}

// TODO: Please check, docIdScoresElem seems to only have 1 as score ???
const pair_sizet_float_vt QueryProcessingEngine::searchClusterCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {
    std::map<size_t, float> docId2Scores;

    for (auto& elem : collectionIds) {
        float sim = Utility::SimilarityMeasures::calcCosSim(*query, DocumentManager::getInstance().getDocument(elem));
        docId2Scores[elem] = sim;
    }

    // Convert map into vector of pairs
    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) {
        results.push_back(elem);
    }

    // Sort vector desc
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });

    std::cout << "sorted cluster search results" << std::endl;
    for (int j = 0; j < results.size(); ++j) {
        std::cout << "docIndex" << results[j].first << " similarity" << results[j].second << std::endl;
    }

    return (!topK || topK > results.size()) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}

const size_t QueryProcessingEngine::searchClusterCosFirstIndex(const Document* query, const sizet_vt& collectionIds) {
    return QueryProcessingEngine::getInstance().searchClusterCos(query, collectionIds, 1)[0].first; // get most similar leader
}

const pair_sizet_float_vt QueryProcessingEngine::searchTieredCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {

    std::map<size_t, float> docId2Scores;

    for (auto& elem : collectionIds) {
        float sim = Utility::SimilarityMeasures::calcCosSim(*query, DocumentManager::getInstance().getDocument(elem));
        docId2Scores[elem] = sim;
    }

    // Convert map into vector of pairs
    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) {
        results.push_back(elem);
    }

    // Sort vector desc
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });

    std::cout << "sorted cluster search results" << std::endl;
    for (int j = 0; j < results.size(); ++j) {
        std::cout << "docIndex" << results[j].first << " similarity" << results[j].second << std::endl;
    }

    return (!topK || topK > results.size()) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}

const pair_sizet_float_vt QueryProcessingEngine::searchRandomProjCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {
    std::map<size_t, float> docId2Scores;

    for (auto& elem : collectionIds) {
        docId2Scores[elem] =
            Utility::SimilarityMeasures::calcHammingDist(query->getRandProjVec(), DocumentManager::getInstance().getDocumentMap().at(elem).getRandProjVec());
    }

    // Convert map into vector of pairs
    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) {
        results.push_back(elem);
    }

    // Sort vector ascending as this is a DISTANCE measure
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second < elem2.second; });
    return (!topK || topK > results.size()) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}
