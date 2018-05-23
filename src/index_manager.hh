/**
 *	@file 	index_manager.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  Implements the index manager which handles the initialization of the vector space model components and
 *          the different indices (Inverted, Tiered, Cluster)
 *	@bugs 	Currently no bugs known
 *	@todos	to exception handling for all calls to .at() -> possible out of range exception. Write DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include "types.hh"
#include "trace.hh"
#include "exception.hh"
#include "string_util.hh"
#include "ir_util.hh"
#include "vec_util.hh"
#include "similarity_util.hh"
#include "cluster.hh"
#include "document.hh"
#include "inverted_index.hh"
#include "tiered_index.hh"
#include "random_projection.hh"
#include "word_embeddings.hh"
#include "query_execution_engine.hh"

class IndexManager {
  private:
    explicit IndexManager();
    IndexManager(const IndexManager&) = delete;
    IndexManager(IndexManager&&) = delete;
    IndexManager& operator=(const IndexManager&) = delete;
    IndexManager& operator=(IndexManager&&) = delete;
    ~IndexManager() = default;

  private:
    /**
     * @brief Builds all Indices
     *
     * @param postinglist_out the inverted index postinglists
     * @param tieredpostinglist_out the tiered index postinglists
     * @param cluster_out the cluster map
     * @param leaders the leaders of the clustered index
     */
    void buildIndices(str_postinglist_mt* postinglist_out,
                      str_tierplmap_mt* tieredpostinglist_out,
                      cluster_mt* cluster_out,
                      const sizet_vt& leaders);

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
    inline const InvertedIndex& getInvertedIndex() const { return _invertedIndex; }
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
     * @brief Get the word embeddings index objectt
     *
     * @return const WordEmbeddings& the word embeddings index
     */
    inline WordEmbeddings& getWordEmbeddingsIndex() { return _wordEmbeddingsIndex; }

    // TODO docs
    /**
     * @brief 
     * 
     * @param doc 
     */
    void buildTfIdfVector(Document& doc);
    // TODO docs
    /**
     * @brief 
     * 
     * @param doc 
     */
    void buildRandProjVector(Document& doc);
    // TODO docs
    /**
     * @brief 
     * 
     * @param doc 
     */
    void buildWordEmbeddingsVector(Document& doc);

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
    void init(const CB& aControlBlock, doc_mt& aDocMap);

  private:
    const CB* _cb;
    doc_mt* _docs;

    str_float_mt _idf_map;
    string_vt _collection_terms; // does not have to be sorted

    InvertedIndex& _invertedIndex;
    TieredIndex& _tieredIndex;
    Cluster& _clusteredIndex;
    WordEmbeddings& _wordEmbeddingsIndex;
};
