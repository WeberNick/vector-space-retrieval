#pragma once

#include "document.hh"
#include "types.hh"
#include "utility.hh"
#include <string>
#include <vector>

class QueryProcessingEngine {
   private:
    QueryProcessingEngine();                                                  // Constructor hidden
    QueryProcessingEngine(QueryProcessingEngine const&) = delete;             // Nick: modified to delete
    QueryProcessingEngine& operator=(QueryProcessingEngine const&) = delete;  // Nick: modified to delete
    ~QueryProcessingEngine() {}                                               // Destructor hidden

   public:
    /**
     * @brief Get the Instance object
     *
     * @return QueryProcessingEngine&
     */
    inline static QueryProcessingEngine& getInstance() {
        static QueryProcessingEngine lInstance;
        return lInstance;
    }

    void init(const CB& aControlBlock);

   private:
    /**
     * @brief Reads a stopwords file for the function @see QueryProcessingEngine#createQueryDoc
     *
     * @param aFile The file containing stopwords separated by ","
     */
    void read(const std::string& aFile);

   public:
    inline string_vt getStopwordlist() { return _stopword_list; }

    /**
     * @brief A top level implementation of the search function. Use a string and type to search for similar documents
     *
     * @param query The raw string query
     * @param topK How many results are retrieved
     * @param searchType What type of search should be executed
     * @return pair_sizet_float_vt A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt search(std::string& query, size_t topK, IR_MODE searchType);

    /**
     * @brief Search function for searching the whole document collection
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @param topK How many results are retrieved
     * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK);

    /**
     * @brief Search function for searching the cluster representation
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @param topK How many results are retrieved
     * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt searchClusterCos(const Document* query, const sizet_vt& collectionIds, size_t topK);

    /**
     * @brief Just a wrapper function for @see QueryProcessingEngine::searchClusterCos to retrieve the docId of the most similar doc
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @return const size_t docId of the most similar doc
     */
    const size_t searchClusterCosFirstIndex(const Document* query, const sizet_vt& collectionIds);

    /**
     * @brief Search function for Search function for searching the tiered index representation
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @param topK How many results are retrieved
     * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt searchTieredCos(const Document* query, const sizet_vt& collectionIds, size_t topK);

    /**
     * @brief Search function for searching when random projections are used
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @param topK How many results are retrieved
     * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt searchRandomProjCos(const Document* query, const sizet_vt& collectionIds, size_t topK);

   private:
    const CB* _cb;

    bool        _init;
    std::string _stopwordFile;
    string_vt _stopword_list;
};
