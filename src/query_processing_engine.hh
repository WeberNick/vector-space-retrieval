#pragma once

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
    std::vector<int> cosineScore(const std::string& query, size_t topK);
};
