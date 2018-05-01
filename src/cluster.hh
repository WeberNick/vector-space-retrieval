/**
 *	@file 	cluster.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief  Implements the pre-clustering functionality for documents
 *	@bugs 	Currently no bugs known
 *	@todos	A cluster can be created and it will automatically initiated itself.
 *	        But: need to add functionality.
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include "types.hh"
#include "exception.hh"
#include "trace.hh"
#include "utility.hh"

#include "document.hh"
#include "document_manager.hh"
#include "query_processing_engine.hh"

#include <algorithm>
#include <cmath>
#include <random>
#include <unordered_map>
#include <vector>
#include <utility>
#include <set>

using cluster_mt = std::unordered_map<size_t, sizet_vt>;

class Cluster
{
  private:
    friend class IndexManager;
    explicit Cluster();
    Cluster(const Cluster&) = default;
    Cluster(Cluster&&) = delete;
    Cluster& operator=(const Cluster&) = delete;
    Cluster& operator=(Cluster&&) = delete;
    ~Cluster();

  private:
    /**
     * @brief Get the Cluster Singleton instance.
     *
     * @return Cluster& a reference to the Cluster Singleton instance.
     */
    inline static Cluster& getInstance() {
        static Cluster lInstance;
        return lInstance;
    }
    /**
     * @brief Initialize control block and cluster
     *
     * @param aControlBlock the control block
     */
    void init(const CB& aControlBlock, cluster_mt& aCluster);

    /**
     * @brief Choose Leaders
     *
     */
    void chooseLeaders();

  public:
    /**
     * @brief Get the Leaders object
     *
     * @return const doc_ptr_vt&
     */
    inline const sizet_set& getLeaders() { return _leaders; }
    /**
     * @brief Get the Leaders Vec object
     * 
     * @return const sizet_vt& 
     */
    inline const sizet_vt& getLeadersVec() { return _leadersVec; }
    /**
     * @brief Get the Cluster object
     *
     * @return const cluster_mt&
     */
    inline cluster_mt& getCluster() { return _cluster; }

  public:
    /**
     * @brief 
     * 
     * @param aLeaders 
     * @param aTopK 
     * @return const sizet_vt 
     */
    const sizet_vt getIDs(const std::vector<std::pair<size_t, float>>& aLeaders, const size_t aTopK);

  private:
    const CB* _cb;

    bool _init;           // was the cluster initialized?
    sizet_set _leaders;   // stores IDs of leaders as a set
    sizet_vt _leadersVec; // stores IDs of leaders as a list
    cluster_mt _cluster;  // stores <DocID, Vector<DocID>> pairs, the first id represents a leader document
};