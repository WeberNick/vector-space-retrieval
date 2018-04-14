#pragma once

#include "document.hh"
#include <string>
#include <vector>

class QueryProcessingEngine {
  private:
    QueryProcessingEngine() {}
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

  public:
    void search(const Document* query, size_t topK);
    std::vector<size_t> cosineScore(const Document* query, const doc_mt &collection, size_t topK);
    const size_t cosineScoreCluster(Document* query, const std::vector<Document*>& collection);
};
