#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>

typedef std::size_t size_t;
typedef std::vector<size_t> size_t_vt;
typedef std::byte byte;
typedef unsigned int uint;
typedef std::vector<uint> uint_vt;
typedef std::vector<std::string> string_vt;
typedef std::map<std::string, float> float_map;
typedef std::map<std::string, uint> int_map;


struct control_block_t
{
    const bool          _trace;
    const bool          _measure;
    const bool          _print;


    const std::string   _collectionPath;
    

    const uint          _noTiers;
    const uint          _noDimensions;


    const bool          trace()         const { return _trace; }
    const bool          measure()       const { return _measure; }
    const bool          print()         const { return _print; }
    const std::string&  collectionPath()const { return _collectionPath; }
    const uint          tiers()         const { return _noTiers; }
    const uint          dimensions()    const { return _noDimensions; }
};
