#pragma once

#include "document.hh"
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
    std::vector<size_t> search(Document* query, size_t topK, Utility::VSMType searchType);
    std::vector<size_t> cosineScoreLSHSearch(const Document* query, const doc_mt& collection, size_t topK);
    std::vector<size_t> cosineScoreVanillaSearch(const Document* query, const doc_mt& collection, size_t topK);
    std::vector<size_t> cosineScoreClusterSearch(const Document* query, const doc_ptr_vt& leaders, size_t topK);
    size_t cosineScoreClusterBuilding(const Document* query, const doc_ptr_vt& collection);

  private:
    const CB* _cb;
    bool _init;
    const std::string _stopwordFile; // defined manually
    string_vt _stopword_list;
};
