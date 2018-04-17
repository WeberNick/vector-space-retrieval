#pragma once

#include "document.hh"
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
    void init(const control_block_t& aControlBlock);

  private:
    void read(const std::string& aFile);

  public:
    string_vt getStopwordlist() { return _stopword_list; }
    std::vector<size_t> search(Document* query, size_t topK);
    std::vector<size_t> cosineScore(const Document* query, const doc_mt& collection, size_t topK);
    const size_t cosineScoreCluster(Document* query, const std::vector<Document*>& collection);

  private:
    const control_block_t* _cb;
    bool _init;
    const std::string _stopwordFile; // defined manually
    string_vt _stopword_list;
};
