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
#include <string>
#include <map>
#include <vector>
#include <set>
#include <utility>

using size_t = std::size_t;
using byte = std::byte;
using uint = unsigned int;
using sizet_set = std::set<size_t>;
using uint_vt = std::vector<uint>;
using float_vt = std::vector<float>;
using sizet_vt = std::vector<size_t>;
using string_vt = std::vector<std::string>;
using str_int_mt = std::map<std::string, uint>;
using str_float_mt = std::map<std::string, float>;
using sizet_float_mt = std::map<size_t, float>;
using float_vector_vt = std::vector<std::vector<float>>;
using pair_sizet_float_vt = std::vector<std::pair<size_t, float>>;
using sizet_set = std::set<size_t>;

struct control_block_t {
    const bool _trace;   // indicate if tracing is activated
    const bool _measure; // indicate if measurement is activated
    const bool _plot;    // indicate if tracing/(error) messages shall be printed (cout)

    const std::string _collectionPath; // the path to the document collection
    const std::string _relScoresPath;  // the path to the relevance scores
    const std::string _tracePath;      // the path to the trace logs
    const std::string _evalPath;       // the path to the evaluation file
    const std::string _stopwordFile;   // the path to the stopword file

    const uint _noResults;    // the number of results to return for each query
    const uint _noTiers;      // number of tiers for the tiered index
    const uint _noDimensions; // the number of dimensions for the random projection

    bool trace() const { return _trace; }
    bool measure() const { return _measure; }
    bool plot() const { return _plot; }
    const std::string& collectionPath() const { return _collectionPath; }
    const std::string& relevanceScoresPath() const { return _relScoresPath; }
    const std::string& tracePath() const { return _tracePath; }
    const std::string& evalPath() const { return _evalPath; }
    const std::string& stopwordFile() const { return _stopwordFile; }
    uint results() const { return _noResults; }
    uint tiers() const { return _noTiers; }
    uint dimensions() const { return _noDimensions; }
};
using CB = control_block_t;

enum IR_MODE {
    kNoMode = -1,
    kVANILLA = 0,
    kTIERED = 1,
    kTIEREDW2V = 2,
    kCLUSTER = 3,
    kCLUSTERW2V = 4,
    kRANDOM = 5,
    kRANDOMW2V = 6,
    kNumberOfModes = 7
};

inline std::string modeToString(IR_MODE aMode) {
    switch (aMode) {
        case kNoMode:
            return "Invalid!"; break;        // not needed but used for convention
        case kVANILLA: 
            return "VanillaVSM"; break;       // not needed but used for convention
        case kTIERED: 
            return "TieredIndex"; break;      // not needed but used for convention
        case kTIEREDW2V: 
            return "TieredIndexW2V"; break;      // not needed but used for convention
        case kCLUSTER: 
            return "Cluster"; break;          // not needed but used for convention
        case kCLUSTERW2V: 
            return "ClusterW2V"; break;          // not needed but used for convention
        case kRANDOM: 
            return "RandomProjection"; break; // not needed but used for convention
        case kRANDOMW2V: 
            return "RandomProjectionW2V"; break; // not needed but used for convention
        default:
            return "Mode not supported"; break;
    }
}

inline IR_MODE stringToMode(std::string aMode) {

    if (aMode.compare("kVANILLA") == 0) {
        return kVANILLA;
    } else if (aMode.compare("kTIERED") == 0) {
        return kTIERED;
    } else if (aMode.compare("kCLUSTER") == 0) {
        return kCLUSTER;
    } else {
        return IR_MODE ::kNoMode;
    }
}
