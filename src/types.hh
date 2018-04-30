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
#include <vector>

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

struct control_block_t {
    const bool _trace;   // indicate if tracing is activated
    const bool _measure; // indicate if measurement is activated
    const bool _plot;   // indicate if tracing/(error) messages shall be printed (cout)

    const std::string _collectionPath; // the path to the document collection
    const std::string _tracePath;
    const std::string _evalPath;

    const uint _noResults;    // the number of results to return for each query
    const uint _noTiers;      // number of tiers for the tiered index
    const uint _noDimensions; // the number of dimensions for the random projection

    bool trace() const { return _trace; }
    bool measure() const { return _measure; }
    bool plot() const { return _plot; }
    const std::string& collectionPath() const { return _collectionPath; }
    const std::string& tracePath() const { return _tracePath; }
    const std::string& evalPath() const { return _evalPath; }
    uint results() const { return _noResults; }
    uint tiers() const { return _noTiers; }
    uint dimensions() const { return _noDimensions; }
};
using CB = control_block_t;

enum IR_MODE
{
    kNoMode = -1,
    kVANILLA = 0,
    kTIERED = 1,
    kCLUSTER = 2,
    kRANDOM = 3,
    kNumberOfModes = 4
};

inline std::string modeToString(IR_MODE aMode)
{
    switch(aMode)
    {
        case -1:
            return "Invalid!";
            break; //not needed but used for convention
        case 0:
            return "VanillaVSM";
            break; //not needed but used for convention
        case 1:
            return "TieredIndex";
            break; //not needed but used for convention
        case 2:
            return "Cluster";
            break; //not needed but used for convention
        case 3:
            return "RandomProjection";
            break; //not needed but used for convention
        default:
            return "Mode not supported";
            break;
    }
}
