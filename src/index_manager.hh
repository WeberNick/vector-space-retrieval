/**
 *	@file 	index_manager.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  Implements the index manager which handles the initialization of the vector space model components and
 *          the different indices (Inverted, Tiered, Cluster)
 *	@bugs 	Currently no bugs known
 *	@todos	Write DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
 */
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
    /**
     * @brief Builds all Indices
     *
     * @param postinglist_out the inverted index postinglists
     */
    void buildIndices(postinglist_mt& postinglist_out);

  public:
    /**
     * @brief Get the idf map
     *
     * @return const str_float_mt& the idf map
     */
    inline const str_float_mt& getIdfMap() { return _idf_map; }
    /**
     * @brief Get the distinct terms in the collection
     *
     * @return const string_vt& the collection terms
     */
    inline const string_vt& getCollectionTerms() { return _collection_terms; }
    /**
     * @brief Get the inverse document frequency for the term
     *
     * @param term the term
     * @return const float the inverse document frequency
     */
    inline float getIdf(std::string term) { return _idf_map.at(term); }

    /**
     * @brief Get the inverted index object
     *
     * @return const InvertedIndex& the inverted index
     */
    inline const InvertedIndex& getInvertedIndex() { return _invertedIndex; }
    /**
     * @brief Get the tiered index object
     *
     * @return const TieredIndex& the tiered index
     */
    inline const TieredIndex& getTieredIndex() { return _tieredIndex; }
    /**
     * @brief Get the clustered index object
     *
     * @return const Cluster& the clustered index
     */
    inline Cluster& getClusteredIndex() { return _clusteredIndex; }

    /**
     * @brief Get the IndexManager Singleton instance.
     *
     * @return IndexManager& a reference to the IndexManager Singleton instance.
     */
    inline static IndexManager& getInstance() {
        static IndexManager indexManager;
        return indexManager;
    }
    /**
     * @brief initialize control block and index manager
     *
     * @param aControlBlock the control block
     */
    void init(const control_block_t& aControlBlock, doc_mt& aDocMap);

  private:
    const control_block_t* _cb;
    doc_mt* _docs;

    bool _init;
    str_float_mt _idf_map;
    string_vt _collection_terms; // does not have to be sorted

    InvertedIndex _invertedIndex;
    TieredIndex _tieredIndex;
    Cluster _clusteredIndex;
};
