#pragma once

#include "document.hh"
#include "inverted_index.hh"
#include "tiered_index.hh"

class IndexManager {
  private:
    explicit IndexManager();
    IndexManager(const IndexManager&) = delete;
    IndexManager(IndexManager&&) = delete;
    IndexManager& operator=(const IndexManager&) = delete;
    IndexManager& operator=(IndexManager&&) = delete;
    ~IndexManager();

  public:
    inline const InvertedIndex& getInvertedIndex() { return _invertedIndex; }
    inline const TieredIndex& getTieredIndex() { return _tieredIndex; }
    inline static IndexManager& getInstance() {
        static IndexManager indexManager;
        return indexManager;
    }
  
  public:
    void buildInvertedIndex(const doc_mt& aCollection);
    void buildTieredIndex(const doc_mt& aCollection);

  private:
    InvertedIndex _invertedIndex;
    TieredIndex _tieredIndex;
};
