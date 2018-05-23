/**
 *	@file 	types.hh
 *	@author	Alexander Weiß, Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de), Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  Specification of basic types and the control block used in the project
 *	@bugs 	Currently no bugs known
 *	@todos	Write DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <vector>

using size_t = std::size_t;
using byte = std::byte;
using uint = unsigned int;
using sizet_set = std::set<size_t>;
using uint_vt = std::vector<uint>;
using float_vt = std::vector<float>;
using sizet_vt = std::vector<size_t>;
using string_vt = std::vector<std::string>;
using string_vvt = std::vector<string_vt>;
using str_set = std::unordered_set<std::string>;
using str_int_mt = std::map<std::string, uint>;
using str_float_mt = std::map<std::string, float>;
using str_sizet_mt = std::unordered_map<std::string, size_t>;
using str_str_mt = std::unordered_map<std::string, std::string>;
using sizet_float_mt = std::map<size_t, float>;
using float_vector_vt = std::vector<std::vector<float>>;
using pair_sizet_float_vt = std::vector<std::pair<size_t, float>>;
using sizet_set = std::set<size_t>;

struct control_block_t {
    
    const bool _trace;   // indicate if tracing is activated
    const bool _measure; // indicate if measurement is activated
    const bool _server;

    const std::string _collectionPath;     // the path to the document collection
    const std::string _queryPath;          // the path where the query files are stored
    const std::string _relScoresPath;      // the path to the relevance score file
    const std::string _stopwordPath;       // the path to the stopword file
    const std::string _wordEmbeddingsPath; // the path to the word embeddings file
    const std::string _tracePath;          // the path to the trace logs
    const std::string _evalPath;           // the path to the evaluation

    const uint _noResults;    // the number of results to return for each query
    const uint _noTiers;      // number of tiers for the tiered index
    const uint _noDimensions; // the number of dimensions for the random projection
    const uint _seed;         // seed for random projection and selecting the cluster leaders

    bool trace() const { return _trace; }
    bool measure() const { return _measure; }
    bool server() const { return _server; }
    const std::string& collectionPath() const { return _collectionPath; }
    const std::string& queryPath() const { return _queryPath; }
    const std::string& relevanceScoresPath() const { return _relScoresPath; }
    const std::string& tracePath() const { return _tracePath; }
    const std::string& evalPath() const { return _evalPath; }
    const std::string& stopwordPath() const { return _stopwordPath; }
    const std::string& wordEmbeddingsPath() const { return _wordEmbeddingsPath; }
    uint results() const { return _noResults; }
    uint tiers() const { return _noTiers; }
    uint dimensions() const { return _noDimensions; }
    uint seed() const { return _seed; }
};
using CB = control_block_t;

enum IR_MODE {
    kNoMode = -1,
    kVANILLA = 0,
    kVANILLA_RAND = 1,
    kVANILLA_W2V = 2,
    kTIERED = 3,
    kTIERED_RAND = 4,
    kTIERED_W2V = 5,
    kCLUSTER = 6,
    kCLUSTER_RAND =7,
    kCLUSTER_W2V = 8,
    kNumberOfModes = 9
};

inline std::string modeToString(IR_MODE aMode) {
    switch (aMode) {
        case kNoMode:
            return "Invalid!"; break;        // not needed but used for convention
        case kVANILLA: 
            return "VanillaVSM"; break;       // not needed but used for convention
        case kVANILLA_RAND: 
            return "VanillaVSM_RAND"; break;       // not needed but used for convention    
        case kVANILLA_W2V: 
            return "VanillaVSM_W2V"; break;       // not needed but used for convention
        case kTIERED: 
            return "TieredIndex"; break;      // not needed but used for convention
         case kTIERED_RAND: 
            return "TieredIndex_RAND"; break;      // not needed but used for convention    
        case kTIERED_W2V: 
            return "TieredIndex_W2V"; break;      // not needed but used for convention
        case kCLUSTER: 
            return "Cluster"; break;          // not needed but used for convention
        case kCLUSTER_RAND: 
            return "Cluster_RAND"; break;          // not needed but used for convention
        case kCLUSTER_W2V: 
            return "Cluster_W2V"; break;          // not needed but used for convention
        default:
            return "Mode not supported"; break;
    }
}

inline IR_MODE stringToMode(const std::string& aMode) 
{
    if(aMode == "kNoMode"){ return kNoMode; }
    else if(aMode == "kVANILLA"){ return kVANILLA; } 
    else if(aMode == "kVANILLA_RAND"){ return kVANILLA_RAND; } 
    else if(aMode == "kVANILLA_W2V"){ return kVANILLA_W2V; } 
    else if(aMode == "kTIERED"){ return kTIERED; } 
    else if(aMode == "kTIERED_RAND"){ return kTIERED_RAND; } 
    else if(aMode == "kTIERED_W2V"){ return kTIERED_W2V; } 
    else if(aMode == "kCLUSTER"){ return kCLUSTER; } 
    else if(aMode == "kCLUSTER_RAND"){ return kCLUSTER_RAND; } 
    else if(aMode == "kCLUSTER_W2V"){ return kCLUSTER_W2V; } 
    else{ return kNoMode; }
}

enum QUERY_TYPE
{
    kNoType = -1,
    kALL = 0,
    kNTT = 1,
    kTITLES = 2,
    kVIDDESC = 3,
    kVIDTITLES = 4,
    kNumberOfTypes = 5
};


inline std::string typeToString(QUERY_TYPE aType) {
    switch (aType) {
        case kNoType:
            return "InvalidType!"; break;        // not needed but used for convention
        case kALL: 
            return "all"; break;       // not needed but used for convention
        case kNTT: 
            return "non-topic-titles"; break;       // not needed but used for convention    
        case kTITLES: 
            return "titles"; break;       // not needed but used for convention
        case kVIDDESC: 
            return "vid-description"; break;      // not needed but used for convention
         case kVIDTITLES: 
            return "vid-titles"; break;      // not needed but used for convention    
        default:
            return "Type not supported"; break;
    }
}
