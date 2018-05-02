#pragma once

#include "document.hh"
#include "types.hh"
#include "utility.hh"
#include <string>
#include <vector>

class QueryProcessingEngine {
  private:
    QueryProcessingEngine();                                                 // Constructor hidden
    QueryProcessingEngine(QueryProcessingEngine const&) = delete;            // Nick: modified to delete
    QueryProcessingEngine& operator=(QueryProcessingEngine const&) = delete; // Nick: modified to delete
    ~QueryProcessingEngine() {}                                              // Destructor hidden

  public:
    /**
     * @brief 
     * 
     */
    struct SearchHelper {
        uint docId;
        double score;

        SearchHelper(uint docId, double score) : 
          docId(docId),
          score(score)
        {}

        bool operator<(const SearchHelper& e) const { return score < e.score; }
    };

    /**
     * @brief Get the Instance object
     *
     * @return QueryProcessingEngine&
     */
    inline static QueryProcessingEngine& getInstance() {
        static QueryProcessingEngine lInstance;
        return lInstance;
    }
    /**
     * @brief
     *
     * @param aControlBlock
     */
    void init(const CB& aControlBlock);

  private:
    // TODO docs
    /**
     * @brief
     *
     * @param aFile
     */
    void read(const std::string& aFile);

  public:
    /**
     * @brief Get the Stopwordlist
     *
     * @return string_vt the Stopwordlist
     */
    inline string_vt getStopwordlist() { return _stopword_list; }
    /**
     * @brief Entry point for the search. Here the query will get preprocessed and it gets decided which search type to execute
     *
     * @param query The actual query
     * @param topK  How many results to retrieve
     * @param searchType What type of search to we process
     * @return
     */
    pair_sizet_float_vt search(std::string& query, size_t topK, IR_MODE searchType);
    /**
     * @brief Search a given set of document ids for a query with cosine similarity
     *
     * @param query
     * @param collectionIds
     * @param topK
     * @return
     */
    pair_sizet_float_vt searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK);
    pair_sizet_float_vt searchClusterCos(const Document* query, const sizet_vt& collectionIds);
    /**
     * @brief Returns the doc id of the most similar document (Used for genereating the clustered index)
     *
     * @param query
     * @param collectionIds
     * @return
     */
    const size_t searchCollectionCosFirstIndex(const Document* query, const sizet_vt& collectionIds);
    // TODO docs
    /**
     * @brief 
     * 
     * @param query 
     * @param collectionIds 
     * @param topK 
     * @return pair_sizet_float_vt 
     */
    pair_sizet_float_vt cosineScoreLSHSearch(const Document* query, const sizet_vt& collectionIds, size_t topK);

  private:
    const CB*   _cb;

    bool        _init;
    std::string _stopwordFile;
    string_vt   _stopword_list;
};
