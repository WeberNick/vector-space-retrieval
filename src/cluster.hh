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
        typedef std::unordered_map<Document, doc_ptr_vt> cluster_mt;

    public:
        explicit Clustering();
        Clustering(const Clustering&) = delete;
        Clustering(Clustering&&) = delete;
        Clustering& operator=(const Clustering&) = delete;
        Clustering& operator=(Clustering&&) = delete;
        ~Clustering();
    
    public:

    public:
        inline const doc_ptr_vt&    getLeaders(){ return _leaders; }
        inline const cluster_mt&    getCluster(){ return _cluster; }

    private:
        void init();
        void chooseLeaders();


    private:
        doc_ptr_vt  _leaders;
        cluster_mt  _cluster; 
};


