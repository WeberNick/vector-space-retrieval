#pragma once

#include "cluster.hh"
#include "document.hh"
#include "inverted_index.hh"
#include "tiered_index.hh"
#include "utility.hh"

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
    inline const Cluster& getClusteredIndex() { return _clusteredIndex; }

    inline static IndexManager& getInstance() {
        static IndexManager indexManager;
        return indexManager;
    }
    void init(const control_block_t& aCB);
  

  private:
    const control_block_t* _cb;

    InvertedIndex _invertedIndex;
    TieredIndex _tieredIndex;
    Cluster _clusteredIndex;
};
