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

  private:
    void buildIndices(postinglist_mt& postinglist_out);

  public:
    inline const InvertedIndex& getInvertedIndex() { return _invertedIndex; }
    inline const TieredIndex& getTieredIndex() { return _tieredIndex; }
    //inline const Cluster& getClusteredIndex() { return _clusteredIndex; }

    inline static IndexManager& getInstance() {
        static IndexManager indexManager;
        return indexManager;
    }
    void init(const control_block_t& aControlBlock, doc_mt& aDocMap);
  

  private:
    const control_block_t* _cb;

    doc_mt* _docs;
    str_float_mt _idf_map;
    string_vt _collection_terms; // does not have to be sorted

    InvertedIndex _invertedIndex;
    TieredIndex _tieredIndex;
    //Cluster _clusteredIndex;
};
