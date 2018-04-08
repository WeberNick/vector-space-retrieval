/**
 *	@file 	cluster.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief  Implements the pre-clustering functionality for documents	
 *	@bugs 	Currently no bugs known
 *	@todos	-
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include "types.hh"
#include "utils.hh"
#include "document.hh"
#include "document_manager.hh"

#include <vector>
#include <unordered_map>
#include <random>
#include <cmath>
#include <algorithm>

class Clustering
{
    public:
        typedef std::unordered_map<const Document*, doc_ptr_vt> cluster_mt;

    private:
        explicit Clustering();
        Clustering(const Clustering&) = delete;
        Clustering(Clustering&&) = delete;
        Clustering& operator=(const Clustering&) = delete;
        Clustering& operator=(Clustering&&) = delete;
        ~Clustering();

    public:
        static Clustering& getInstance()
        {
            static Clustering lInstance;
            return lInstance;
        }
    
    public:
        inline const doc_ptr_vt&    getLeaders(){ return _leaders; }
        inline const cluster_mt&    getCluster(){ return _cluster; }

    private:
        void init();
        void chooseLeaders();
        void initCluster();
        void fillCluster();


    private:
        doc_ptr_vt  _leaders; //stores pointer to leader documents inside the doc mngr's map
        cluster_mt  _cluster; //stores <Doc*, Vector<Doc*>> pairs, the first pointer is a leader document
        //maybe remove leader vector? can store leader implicitly in cluster map...
};


