#include "query_processing_engine.hh"
#include "index_manager.hh"
#include "posting_list.hh"
#include <iostream>

/**
 * Constructor
 */
QueryProcessingEngine::QueryProcessingEngine() :
    _cb(nullptr), _init(false), _stopwordFile("./data/stopwords.large") // relative path from /path/to/repo/vector-space-retrieval
{}

/**
 * @brief initializes the singleton
 *
 * @param aControlBlock
 */
void QueryProcessingEngine::init(const control_block_t& aControlBlock) {
    _cb = &aControlBlock;
    if (!_init) {
        read(_stopwordFile);
        _init = true;
    }
}

/**
 * @brief Read in a stopword list file
 *
 * @param aFile
 */
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
pair_sizet_float_vt QueryProcessingEngine::search(std::string& query, size_t topK, IR_MODE searchType) {
    // Remove stopwords
    Utility::IR::removeStopword(query, getStopwordlist());

    // Trim whitespaces
    Utility::StringOp::trim(query);

    string_vt proc_query;

    // Split string by whitespaces
    Utility::StringOp::splitString(query, ' ', proc_query);

    // Remove eventually empty strings from the query term vector
    Utility::StringOp::removeEmptyStringsFromVec(proc_query);

    // Preprocess query
    std::vector<std::string> preprocessed_content;

    for (auto& elem : proc_query) {

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

    // Set up result vector
    std::vector<std::pair<size_t, float>> found_indexes;

    // Execute different search workflows based on the search type
    switch (searchType) {
    case IR_MODE::kVANILLA: // found_indexes = QueryProcessingEngine::searchCollectionCos(&queryDoc, DocumentManager::getInstance().getDocumentMap(), topK);
        break;
    case IR_MODE::kTIERED: break;
    case IR_MODE::kRANDOM: break;
    case IR_MODE::kCLUSTER:

        // Get cluster leaders sorted according to query
        std::vector<std::pair<size_t, float>> leader_indexes =
            QueryProcessingEngine::searchCollectionCos(&queryDoc, IndexManager::getInstance().getClusteredIndex().getLeadersVec(), 0);

        // Get Docs to search in
        sizet_vt clusterDocIds = IndexManager::getInstance().getClusteredIndex().getIDs(leader_indexes, topK);

        // Search the docs from the clusters
        found_indexes = QueryProcessingEngine::searchCollectionCos(&queryDoc, clusterDocIds, topK);
        break;
    }

    // Return search result
    return found_indexes;
}

/**
 * @brief Search a given set of document ids for a query with cosine similarity
 *
 * @param query
 * @param collectionIds
 * @param topK
 * @return
 */
pair_sizet_float_vt QueryProcessingEngine::searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK) {

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
                    std::find(collectionIds.begin(), collectionIds.end(), posting.first) !=
                    collectionIds
                        .end(); // check if current doc looked at, also in the collection we want to search in, because we only have global posting list index
                if (is_in) {
                    docId2Scores[posting.first] +=
                        (posting.second * postingList.getIdf() * (Utility::IR::calcTf(query->getContent()[j], query->getContent()) * postingList.getIdf()));
                }
            }
        } catch (const InvalidArgumentException& e) { std::cout << "Error thrown: " << e.what() << std::endl; }
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

/**
 * @brief Returns the doc id of the most similar document (Used for genereating the clustered index)
 *
 * @param query
 * @param collectionIds
 * @return
 */
const size_t QueryProcessingEngine::searchCollectionCosFirstIndex(const Document* query, const sizet_vt& collectionIds) {

    return QueryProcessingEngine::getInstance().searchCollectionCos(query, collectionIds, 1)[0].first; // get most similar leader
}

/**
 * @brief Search function used if the use random projections
 *
 * @param query
 * @param collection
 * @param topK
 * @return
 */
pair_sizet_float_vt QueryProcessingEngine::cosineScoreLSHSearch(const Document* query, const sizet_vt& collectionIds, size_t topK) {

    std::map<size_t, float> docId2Scores;

    for (auto& elem : collectionIds) {
        // docId2Scores[elem] = Utility::SimilarityMeasures::calcHammingDist(query.getLSHBitvec(),
        // DocumentManager::getInstance().getDocumentMap().at(elem).getLSHBitvec());
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
