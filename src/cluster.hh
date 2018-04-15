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
#include "utility.hh"
#include "document.hh"
#include "document_manager.hh"

#include <vector>
#include <unordered_map>
#include <random>
#include <cmath>
#include <algorithm>

class Cluster
{
    friend class IndexManager;

    public:
        using cluster_mt = std::unordered_map<const Document*, doc_ptr_vt>;

    private:
        explicit Cluster();
        Cluster(const Cluster&) = default;
        Cluster(Cluster&&) = delete;
        Cluster& operator=(const Cluster&) = delete;
        Cluster& operator=(Cluster&&) = delete;
        ~Cluster();

    private:
        inline static Cluster& getInstance()
        {
            static Cluster lInstance;
            return lInstance;
        }
        void init(const control_block_t& aCB);
        void init();
        void chooseLeaders();
        void fillCluster();

    public:
        inline const doc_ptr_vt&    getLeaders(){ return _leaders; }
        inline const cluster_mt&    getCluster(){ return _cluster; }

    private:
        doc_ptr_vt  _leaders; //stores pointer to leader documents inside the doc mngr's map
        cluster_mt  _cluster; //stores <Doc*, Vector<Doc*>> pairs, the first pointer is a leader document
        const control_block_t* _cb;
};


