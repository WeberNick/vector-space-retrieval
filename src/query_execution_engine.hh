/*
 * @file    query_execution_engine.hh
 * @author 	Nick Weber
 *          Alexander Weiß
 *          Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 * @date    May 18, 2018
 * @brief 	Top level implementation of the class responsible for
 *          searching our document collection. Implements different
 *          types of search modes.
 * 
 * @section	DESCRIPTION docto_
 */

#pragma once

#include "types.hh"
#include "trace.hh"
#include "ir_util.hh"
#include "string_util.hh"
#include "similarity_util.hh"
#include "document.hh"
#include "query_manager.hh"
#include "posting_list.hh"

#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

class QueryExecutionEngine {
  private:
    QueryExecutionEngine();                                                 // Constructor hidden
    QueryExecutionEngine(QueryExecutionEngine const&) = delete;            // Nick: modified to delete
    QueryExecutionEngine& operator=(QueryExecutionEngine const&) = delete; // Nick: modified to delete
    ~QueryExecutionEngine() = default;                                      // Destructor hidden

  public:
    /**
     * @brief Get the query execution engine instance
     *
     * @return QueryExecutionEngine& the instance
     */
    inline static QueryExecutionEngine& getInstance() {
        static QueryExecutionEngine lInstance;
        return lInstance;
    }
    /**
     * @brief Initialize control block and query execution engine
     *
     * @param aControlBlock the control block
     */
    void init(const CB& aControlBlock);

  public:

    /**
     * @brief A top level implementation of the search function. Use a string and type to search for similar documents
     * If search is called on a raw string preprocessing is always done, and then the raw string is converted into a
     * document.
     *
     * @param query The raw string query
     * @param topK How many results are retrieved
     * @param searchType What type of search should be executed
     * @return pair_sizet_float_vt A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt search(std::string& query, size_t topK, IR_MODE searchType);

    /**
     * @brief A top level implementation of the search function. Use a string and type to search for similar documents
     * This version of search is called on a document object, so it assumes that the content of the document is already
     * preprocessed and will not preprocess it again
     *
     * @param query A query document
     * @param topK How many results are retrieved
     * @param searchType What type of search should be executed
     * @return pair_sizet_float_vt A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt search(Document& query, size_t topK, IR_MODE searchType);

    /**
     * @brief Search function for searching the whole document collection
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @param topK How many results are retrieved
     * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK, bool use_w2v = false);

    /**
     * @brief Search function for searching the cluster representation
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @param topK How many results are retrieved
     * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt searchClusterCos(const Document* query, const sizet_vt& collectionIds, size_t topK, bool use_w2v = false);

    /**
     * @brief Just a wrapper function for @see QueryExecutionEngine::searchClusterCos to retrieve the docId of the most similar doc
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @return const size_t docId of the most similar doc
     */
    size_t searchClusterCosFirstIndex(const Document* query, const sizet_vt& collectionIds);

    /**
     * @brief Search function for Search function for searching the tiered index representation
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @param topK How many results are retrieved
     * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt searchTieredCos(const Document* query, const sizet_vt& collectionIds, size_t topK, bool use_w2v = false);

    /**
     * @brief Search function for searching when random projections are used
     *
     * @param query A preprocessed query document
     * @param collectionIds IDs of docs to search in
     * @param topK How many results are retrieved
     * @return pair_sizet_float_vt  A list of document - similarity pairs ordered descending
     */
    const pair_sizet_float_vt searchRandomProjCos(const Document* query, const sizet_vt& collectionIds, size_t topK);

  private:
    const CB* _cb;
};
