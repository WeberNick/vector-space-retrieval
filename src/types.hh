#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>

typedef std::size_t size_t;
typedef std::byte byte;
typedef unsigned int uint;
typedef std::vector<size_t> size_t_vt;
typedef std::vector<uint> uint_vt;
typedef std::vector<std::string> string_vt;
typedef std::map<std::string, float> str_float_mt;
typedef std::map<std::string, uint> str_int_mt;

struct control_block_t {
    const bool _trace;   // indicate if tracing is activated
    const bool _measure; // indicate if measurement is activated
    const bool _print;   // indicate if tracing/(error) messages shall be printed (cout)

    const std::string _collectionPath; // the path to the document collection

    const uint _noResults;    // the number of results to return for each query
    const uint _noTiers;      // number of tiers for the tiered index
    const uint _noDimensions; // the number of dimensions for the random projection

    const bool trace() const { return _trace; }
    const bool measure() const { return _measure; }
    const bool print() const { return _print; }
    const std::string& collectionPath() const { return _collectionPath; }
    const uint results() const { return _noResults; }
    const uint tiers() const { return _noTiers; }
    const uint dimensions() const { return _noDimensions; }
};
