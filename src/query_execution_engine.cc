#include "query_execution_engine.hh"
#include "index_manager.hh"
#include "measure.hh"

/**
 * @brief Construct a new Query Processing Engine:: Query Processing Engine object
 */
QueryExecutionEngine::QueryExecutionEngine() :
    _cb(nullptr)
{}

void QueryExecutionEngine::init(const control_block_t& aControlBlock) {
    if (!_cb) {
        _cb = &aControlBlock;
        TRACE("QueryExecutionEngine: Initialized");
    }
}

const pair_sizet_float_vt QueryExecutionEngine::search(std::string& query, size_t topK, IR_MODE searchType) {
    Document queryDoc = QueryManager::getInstance().createQueryDoc(query, "query-0", true);
    return this->search(queryDoc, topK, searchType);
}

const pair_sizet_float_vt QueryExecutionEngine::search(Document& queryDoc, size_t topK, IR_MODE searchType) {
    pair_sizet_float_vt found_indices; // result vector

    if (queryDoc.getContent().size() == 0) { // if content is empty stop searching
        return found_indices;
    }

    switch (searchType) {
    case IR_MODE ::kVANILLA: {
        found_indices = this->searchCollectionCos(&queryDoc, IndexManager::getInstance().getInvertedIndex().getDocIDList(queryDoc.getContent()), topK);
    } break;
    case IR_MODE::kVANILLA_RAND: {
        found_indices = this->searchRandomProjCos(&queryDoc, IndexManager::getInstance().getInvertedIndex().getDocIDList(queryDoc.getContent()), topK);
    } break;
    case IR_MODE::kVANILLA_W2V: {
        found_indices = this->searchCollectionCos(&queryDoc, IndexManager::getInstance().getInvertedIndex().getDocIDList(queryDoc.getContent()), topK, true);
    } break;
    case IR_MODE ::kCLUSTER: {
        std::vector<std::pair<size_t, float>> leader_indexes = this->searchClusterCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeaders(), 0);

        // Get docIds from the clusters to search in, vector will be filled from the IndexManager::getInstance().getClusteredIndex().getIDs() method
        sizet_vt clusterDocIds;
        IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK, clusterDocIds);

        // Search the docs from the clusters
        found_indices = this->searchClusterCos(&queryDoc, clusterDocIds, topK);
    } break;
    case IR_MODE::kCLUSTER_RAND: {
        std::vector<std::pair<size_t, float>> leader_indexes =
            this->searchRandomProjCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeaders(), 0);

        // Get docIds from the clusters to search in, vector will be filled from the IndexManager::getInstance().getClusteredIndex().getIDs() method
        sizet_vt clusterDocIds;
        IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK, clusterDocIds);

        // Search the docs from the clusters
        found_indices = this->searchRandomProjCos(&queryDoc, clusterDocIds, topK);
    } break;
    case IR_MODE::kCLUSTER_W2V: {
        std::vector<std::pair<size_t, float>> leader_indexes =
            this->searchClusterCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeaders(), 0, true);

        // Get docIds from the clusters to search in, vector will be filled from the IndexManager::getInstance().getClusteredIndex().getIDs() method
        sizet_vt clusterDocIds;
        IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK, clusterDocIds);
        // Search the docs from the clusters
        found_indices = this->searchClusterCos(&queryDoc, clusterDocIds, topK, true);
    } break;
    case IR_MODE ::kTIERED: {
        found_indices = this->searchTieredCos(&queryDoc, IndexManager::getInstance().getTieredIndex().getDocIDList(topK, queryDoc.getContent()), topK);
    } break;
    case IR_MODE::kTIERED_RAND: {
        found_indices = this->searchRandomProjCos(&queryDoc, IndexManager::getInstance().getTieredIndex().getDocIDList(topK, queryDoc.getContent()), topK);
    } break;
    case IR_MODE::kTIERED_W2V: {
        found_indices = this->searchTieredCos(&queryDoc, IndexManager::getInstance().getTieredIndex().getDocIDList(topK, queryDoc.getContent()), topK, true);
    } break;
    case IR_MODE ::kNoMode: break;
    case IR_MODE ::kNumberOfModes: break;
    default: break;
    }
    return found_indices;
}

const pair_sizet_float_vt QueryExecutionEngine::searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK, bool use_w2v) {

    std::map<size_t, float> docId2Length;
    for (auto& elem : collectionIds) { // Map of doc id to length og that doc
        docId2Length[elem] = DocumentManager::getInstance().getDocumentMap().at(elem).getNormLength();
    }
    std::map<size_t, float> docId2Scores;
    
    // if we are using w2v we can not use our posting list, instead we have to use the normal tfidf vectors + the document word embedding vector
    if (use_w2v) {
        float_vt queryWordEmbedding = Util::combineVectors((*query).getTfIdfVector(), ((*query).getWordEmbeddingsVector()));
        for (auto& elem : collectionIds) {
            float sim = Util::calcCosSim(queryWordEmbedding,Util::combineVectors(DocumentManager::getInstance().getDocument(elem).getTfIdfVector(), DocumentManager::getInstance().getDocument(elem).getWordEmbeddingsVector()));
            docId2Scores[elem] = sim;
        }
    } else {

        for (auto& elem : collectionIds) {
            
            float sim = Util::calcCosSim(*query, DocumentManager::getInstance().getDocument(elem));
            docId2Scores[elem] = sim;
        }

        /* Efficient Vanilla VSM Algorithm, not used for comparison reasons
        const string_vt& qcontent = query->getContent();
        for (const auto& term : qcontent) { // Calculate weightings per doc using the tf-idf of the word in the doc collection times the tf-idf of the term in the query
            try {
                const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList(term);
                for (auto& [id, tf] : postingList.getPosting()) {
                    float idf = IndexManager::getInstance().getIdf(term);
                    docId2Scores[id] += (tf * idf * (query->getTf(term) * idf));
                }
            } catch (const InvalidArgumentException& e) { continue; } // One of the query terms does not appear in the document collection.
        }*/
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

const pair_sizet_float_vt QueryExecutionEngine::searchClusterCos(const Document* query, const sizet_vt& collectionIds, size_t topK, bool use_w2v) {

    std::map<size_t, float> docId2Scores;

    if (use_w2v){
        float_vt queryWordEmbedding = Util::combineVectors((*query).getTfIdfVector(), ((*query).getWordEmbeddingsVector()));
        for (auto& elem : collectionIds) {
            float sim = Util::calcCosSim(queryWordEmbedding,
                                        Util::combineVectors(DocumentManager::getInstance().getDocument(elem).getTfIdfVector(), DocumentManager::getInstance().getDocument(elem).getWordEmbeddingsVector()));
            docId2Scores[elem] = sim;
        }
    } else {
        for (auto& elem : collectionIds) {
            float sim = Util::calcCosSim(*query, DocumentManager::getInstance().getDocument(elem));
            docId2Scores[elem] = sim;
        }
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

size_t QueryExecutionEngine::searchClusterCosFirstIndex(const Document* query, const sizet_vt& collectionIds) {
    return QueryExecutionEngine::getInstance().searchClusterCos(query, collectionIds, 1)[0].first; // get most similar leader
}

const pair_sizet_float_vt QueryExecutionEngine::searchTieredCos(const Document* query, const sizet_vt& collectionIds, size_t topK, bool use_w2v) {

    std::map<size_t, float> docId2Scores;

     if (use_w2v){
        float_vt queryWordEmbedding = Util::combineVectors((*query).getTfIdfVector(), ((*query).getWordEmbeddingsVector()));
        for (auto& elem : collectionIds) {
            float sim = Util::calcCosSim(queryWordEmbedding,
                                         Util::combineVectors(DocumentManager::getInstance().getDocument(elem).getTfIdfVector(), DocumentManager::getInstance().getDocument(elem).getWordEmbeddingsVector()));
            docId2Scores[elem] = sim;
        }
    } else {
        for (auto& elem : collectionIds) {
            float sim = Util::calcCosSim(*query, DocumentManager::getInstance().getDocument(elem));
            docId2Scores[elem] = sim;
        }
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

const pair_sizet_float_vt QueryExecutionEngine::searchRandomProjCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {

    std::map<size_t, float> docId2Scores;
    for (auto& elem : collectionIds) {
        docId2Scores[elem] = Util::calcHammingDist(query->getRandProjVec(),DocumentManager::getInstance().getDocumentMap().at(elem).getRandProjVec());
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
