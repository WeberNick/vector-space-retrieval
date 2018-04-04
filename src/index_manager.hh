#pragma once

#include "inverted_index.hh"
#include "tiered_index.hh"

class IndexManager {

  public:
    explicit IndexManager();
    IndexManager(const IndexManager&) = delete;
    IndexManager(IndexManager&&) = delete;
    IndexManager& operator=(const IndexManager&) = delete;
    IndexManager& operator=(IndexManager&&) = delete;
    ~IndexManager();

  public:
    inline const InvertedIndex& getInvertedIndex() { return _invertedIndex; }
    inline const TieredIndex& getTieredIndex() { return _tieredIndex; }
    inline IndexManager& getInstance() {
        static IndexManager indexManager;
        return indexManager;
    }

  private:
    InvertedIndex _invertedIndex;
    TieredIndex _tieredIndex;
};