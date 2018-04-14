/**
 *	@file 	args.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Implementation of command line arguements
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	This class implements the command line arguements. A command line arguement has the form:
 * 	--[command] [optional parameter]
 * 	Where '--' indicates a command will follow,
 *	'command' is the command name (e.g., 'help'),
 *	'optional parameter' is an optional parameter only needed for certain commands (e.g., for 'path' and string is needed)
 *	A complete invocation example could be:
 *	"./evsr --path "../data/" --trace --print" to use the relative path from src, activate tracing and printing information to std out
 *
 *	@section USAGE
 *	In main, create class 'Args' with the default constructor and add the following code:
 *      //create Args object with default constructor
 *      Args lArgs;
 *      //create vector where to store the command line arguments (used for internal processing)
 *      argdesc_vt lArgDesc;
 *      //pass vector to a internal routine which processes the command line arguments
 *      construct_arg_desc(lArgDesc);
 *      //parses arguments in their respective date type, stores them in an object of Args and return an status flag
 *      if(!parse_args<Args>(1, argc, argv, lArgDesc, lArgs)) //if != 0, an error occured while parsing arguments...
 *      {
 *          std::cerr << "error while parsing arguments." << std::endl;
 *          return -1;
 *      }
 *      //now use the data stored in object lArgs like this:
 *      bool help = lArgs.help();
 *      //write your own functions and command line arguments in this header and use the same structure as seen in args.cc
 */
#pragma once

#include "argbase.hh"
#include "types.hh"
#include <string>

class Args {
  public:
    explicit Args();
    explicit Args(const Args&) = delete;
    Args(Args&&) = delete;
    Args& operator=(const Args&) = delete;
    Args& operator=(Args&&) = delete;
    ~Args();

  public:
    inline const bool help() { return _help; }
    inline void help(const bool& x) { _help = x; }

    inline const bool trace() { return _trace; }
    inline void trace(const bool& x) { _trace = x; }

    inline const bool measure() { return _measure; }
    inline void measure(const bool& x) { _measure = x; }

    inline const bool print() { return _print; }
    inline void print(const bool& x) { _print = x; }

    inline const std::string path() { return _path; }
    inline void path(const std::string& x) { _path = x; }

    inline const uint results() { return _results; }
    inline void results(const uint& x) { _results = x; }

    inline const uint tiers() { return _tiers; }
    inline void tiers(const uint& x) { _tiers = x; }

    inline const uint dimensions() { return _dimensions; }
    inline void dimensions(const uint& x) { _dimensions = x; }

  private:
    bool _help;
    bool _trace;
    bool _measure;
    bool _print;

    std::string _path;

    uint    _results;
    uint    _tiers;
    uint    _dimensions;
};

using argdesc_vt = std::vector<argdescbase_t<Args>*>;

void construct_arg_desc(argdesc_vt& aArgDesc);
