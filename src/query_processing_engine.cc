#include "query_processing_engine.hh"
#include "index_manager.hh"

/**
 * @brief Construct a new Query Processing Engine:: Query Processing Engine object
 */
QueryProcessingEngine::QueryProcessingEngine() : _cb(nullptr), _stopwordFile() {}

/**
 * @brief Initializes the singleton
 *
 * @param aControlBlock
 */
void QueryProcessingEngine::init(const control_block_t& aControlBlock) {
    if (!_cb) {
        _cb = &aControlBlock;
        _stopwordFile = _cb->stopwordPath(); // relative path from /path/to/repo/vector-space-retrieval
        read(_stopwordFile);
        TRACE("QueryExecutionEngine: Initialized");
    }
}

void QueryProcessingEngine::read(const std::string& aFile) {
    std::ifstream file(aFile);
    std::string line;
    while (std::getline(file, line)) {
        Utility::StringOp::splitStringBoost(line, ',', this->_stopword_list);
    }
}

const pair_sizet_float_vt QueryProcessingEngine::search(std::string& query, size_t topK, IR_MODE searchType, bool use_lsh) {
    Document queryDoc = DocumentManager::getInstance().createQuery(query);
    return this->search(queryDoc, topK, searchType, use_lsh);
}

const pair_sizet_float_vt QueryProcessingEngine::search(Document& queryDoc, size_t topK, IR_MODE searchType, bool use_lsh) {

    std::cout << "Searching in mode: " << modeToString(searchType) << std::endl;
    std::cout << "Searching for: ";
    for (auto& elem : queryDoc.getContent()) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    pair_sizet_float_vt found_indices; // result vector

    switch (searchType) {
    case IR_MODE ::kVANILLA: {

        if (use_lsh) {
            found_indices = this->searchRandomProjCos(&queryDoc, DocumentManager::getInstance().getIDs(), topK);
        } else {
            found_indices = this->searchCollectionCos(&queryDoc, DocumentManager::getInstance().getIDs(), topK);
        }
    } break;
    case IR_MODE ::kCLUSTER: {
        if (use_lsh) {
            std::vector<std::pair<size_t, float>> leader_indexes =
                this->searchRandomProjCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeaders(), 0);

            // Get docIds from the clusters to search in, vector will be filled from the IndexManager::getInstance().getClusteredIndex().getIDs() method
            sizet_vt clusterDocIds;
            IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK, clusterDocIds);

            // Search the docs from the clusters
            found_indices = this->searchRandomProjCos(&queryDoc, clusterDocIds, topK);
        } else {
            std::vector<std::pair<size_t, float>> leader_indexes =
                this->searchClusterCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeaders(), 0);

            // Get docIds from the clusters to search in, vector will be filled from the IndexManager::getInstance().getClusteredIndex().getIDs() method
            sizet_vt clusterDocIds;
            IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK, clusterDocIds);

            // Search the docs from the clusters
            found_indices = this->searchClusterCos(&queryDoc, clusterDocIds, topK);
        }
    } break;
    case IR_MODE ::kTIERED: {
        if (use_lsh) {
            found_indices = QueryProcessingEngine::searchRandomProjCos(
                &queryDoc, IndexManager::getInstance().getTieredIndex().getDocIDList(topK, queryDoc.getContent()), topK);
        } else {
            found_indices =
                QueryProcessingEngine::searchTieredCos(&queryDoc, IndexManager::getInstance().getTieredIndex().getDocIDList(topK, queryDoc.getContent()), topK);
        }
    } break;
    case IR_MODE ::kRANDOM: {
        found_indices = QueryProcessingEngine::searchRandomProjCos(&queryDoc, DocumentManager::getInstance().getIDs(), topK);
    } break;
    case IR_MODE ::kNoMode: break;
    case IR_MODE ::kNumberOfModes: break;
    default: break;
    }

    // Return search result
    std::cout << "Returning results" << std::endl;
    for (auto& elem : found_indices) {
        std::cout << "id: " << elem.first << " sim: " << elem.second << std::endl;
    }
    return found_indices;
}

const pair_sizet_float_vt QueryProcessingEngine::searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {
    std::cout << "in search collection cos" << std::endl;

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
        } catch (const InvalidArgumentException& e) { continue; /* One of the query terms does not appear in the document collection. */ }
    }

    std::cout << "after long loop search collection cos" << std::endl;

    for (const auto& elem : docId2Length) { // Divide every score of a doc by the length of the document
        docId2Scores[elem.first] = docId2Scores[elem.first] / docId2Length[elem.first];
    }

    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) { // Convert map into vector of pairs
        results.push_back(elem);
    }
    std::cout << "after result push collection cos" << std::endl;

    // Sort vector desc
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });
    std::cout << "after result search collection cos" << std::endl;
    return (!topK || topK > results.size()) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}

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
    return (!topK || topK > results.size()) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}

const size_t QueryProcessingEngine::searchClusterCosFirstIndex(const Document* query, const sizet_vt& collectionIds) {
    return QueryProcessingEngine::getInstance().searchClusterCos(query, collectionIds, 1)[0].first; // get most similar leader
}

const pair_sizet_float_vt QueryProcessingEngine::searchTieredCos(const Document* query, const sizet_vt& collectionIds, size_t topK, bool useW2V) {

    std::map<size_t, float> docId2Scores;

    for (auto& elem : collectionIds) {
        float sim;
        if (useW2V) {
            std::cout << " we are using word2vec" << std::endl;
            sim = Utility::SimilarityMeasures::calcCosSim(*query, DocumentManager::getInstance().getDocument(elem));
        } else {
            sim = Utility::SimilarityMeasures::calcCosSim(*query, DocumentManager::getInstance().getDocument(elem));
        }
        docId2Scores[elem] = sim;
    }

    // Convert map into vector of pairs
    std::vector<std::pair<size_t, float>> results;
    for (const auto& elem : docId2Scores) {
        results.push_back(elem);
    }

    // Sort vector desc
    std::sort(results.begin(), results.end(), [](std::pair<size_t, float> elem1, std::pair<size_t, float> elem2) { return elem1.second > elem2.second; });
    return (!topK || topK > results.size()) ? results : std::vector<std::pair<size_t, float>>(results.begin(), results.begin() + topK);
}

const pair_sizet_float_vt QueryProcessingEngine::searchRandomProjCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {

    std::cout << "using rand projections vectors " << std::endl;
    std::map<size_t, float> docId2Scores;

    for (auto& elem : collectionIds) {
        docId2Scores[elem] = Utility::SimilarityMeasures::calcHammingDist(query->getRandProjTiVec(),
                                                                          DocumentManager::getInstance().getDocumentMap().at(elem).getRandProjTiVec());
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
