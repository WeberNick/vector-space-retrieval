#pragma once

#include "document.hh"
#include "types.hh"
#include "utility.hh"
#include <string>
#include <vector>

class QueryProcessingEngine {
  private:
    QueryProcessingEngine();
    // ctor hidden
    QueryProcessingEngine(QueryProcessingEngine const&) = delete;            // Nick: modified to delete
    QueryProcessingEngine& operator=(QueryProcessingEngine const&) = delete; // Nick: modified to delete
    ~QueryProcessingEngine() {}
    // dtor hidden

  public:
    static QueryProcessingEngine& getInstance() {
        static QueryProcessingEngine lInstance;
        return lInstance;
    }

    struct SearchHelper {
        unsigned int docId;
        double score;

        SearchHelper(int docId, double score) : docId(docId), score(score) {}

        bool operator<(const SearchHelper& e) const { return score < e.score; }
    };

    /**
     * @brief
     *
     * @param aControlBlock
     */
    void init(const CB& aControlBlock);

  private:
    void read(const std::string& aFile);

  public:
    string_vt getStopwordlist() { return _stopword_list; }
    pair_sizet_float_vt search(std::string& query, size_t topK, IR_MODE searchType);
    pair_sizet_float_vt searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK);
    pair_sizet_float_vt cosineScoreLSHSearch(const Document* query, const sizet_vt& collectionIds, size_t topK);
    const size_t searchCollectionCosFirstIndex(const Document* query, const sizet_vt& collectionIds);

  private:
    const CB* _cb;
    bool _init;
    const std::string _stopwordFile; // defined manually
    string_vt _stopword_list;
};
