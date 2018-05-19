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

#include "document_manager.hh"

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
    ~Cluster() = default;

  private:
    /**
     * @brief Get the Cluster Singleton instance.
     * @return Cluster& a reference to the Cluster Singleton instance.
     */
    inline static Cluster& getInstance() 
    {
        static Cluster lInstance;
        return lInstance;
    }

    /**
     * @brief Initialize control block and cluster
     * @param aControlBlock the control block containing runtime specific data
     */
    void init(const CB& aControlBlock);

    /**
     * @brief Choose sqrt(N) random leaders
     */
    void chooseLeaders();

  public:
    /**
     * @brief Get the document IDs of the leader
     * @return a vector containing all document IDs
     */
    inline const sizet_vt& getLeaders() { return _leaders; }

    /**
     * @brief Getter for the cluster map
     * @return the cluster map
     */
    inline cluster_mt& getCluster() { return _cluster; }

  public:
    /**
     * @brief 
     * @param aLeaders 
     * @param aTopK 
     * @return const sizet_vt 
     */
    void getIDs(const std::vector<std::pair<size_t, float>>& aLeaders, const size_t aTopK, sizet_vt& aOutputVec);

  private:
    const CB* _cb;
    sizet_vt _leaders;
    cluster_mt _cluster; // stores <DocID, Vector<DocID>> pairs, the first id represents a leader document
};
