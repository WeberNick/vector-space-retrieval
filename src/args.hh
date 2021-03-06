/**
 *	@file 	 args.hh
 *	@author	 Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
             Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief	 Implementation of command line arguements
 *
 *	@section DESCRIPTION
 *	         This class implements the command line arguements. A command line arguement has the form:
 * 	         --[command] [optional parameter]
 * 	         Where '--' indicates a command will follow,
 *	         'command' is the command name (e.g., 'help'),
 *	         'optional parameter' is an optional parameter only needed for certain commands (e.g., for 'path' a string is needed)
 *	         A complete invocation example could be:
 *	         "./bin/evsr_run --path "../data/" --trace --print" to use the relative path from src, activate tracing and printing information to std out
 *
 *	@section USAGE
 *	         In main, create class 'Args' with the default constructor and add the following code:
 *
 *               Args lArgs;                                           // create Args object with default constructor
 *               argdesc_vt lArgDesc;                                  // create vector where to store the command line arguments (used for internal processing)
 *               construct_arg_desc(lArgDesc);                         // pass vector to a internal routine which processes the command line arguments
 *               // parses arguments in their respective date type, stores them in an object of Args and return an status flag
 *               if(!parse_args<Args>(1, argc, argv, lArgDesc, lArgs)) // if != 0, an error occured while parsing arguments...
 *               {
 *                   std::cerr << "error while parsing arguments." << std::endl;
 *                   return -1;
 *               }
 *               bool help = lArgs.help();                             // now use the data stored in object lArgs like this:
 *               // write your own functions and command line arguments in this header and use the same structure as seen in args.cc
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
    ~Args() = default;

  public:
    inline bool help() { return _help; }
    inline void help(const bool& x) { _help = x; }

    inline bool trace() { return _trace; }
    inline void trace(const bool& x) { _trace = x; }

    inline bool measure() { return _measure; }
    inline void measure(const bool& x) { _measure = x; }

    inline bool server() { return _server; }
    inline void server(const bool& x) { _server = x; }

    inline const std::string& collectionPath() { return _collectionPath; }
    inline void collectionPath(const std::string& x) { _collectionPath = x; }

    inline const std::string& queryPath() { return _queryPath; }
    inline void queryPath(const std::string& x) { _queryPath = x; }

    inline const std::string& relevanceScoresPath() { return _relScoresPath; }
    inline void relevanceScoresPath(const std::string& x) { _relScoresPath = x; }

    inline const std::string& stopwordPath() { return _stopwordPath; }
    inline void stopwordPath(const std::string& x) { _stopwordPath = x; }

    inline const std::string& wordEmbeddingsPath() { return _wordEmbeddingsPath; }
    inline void wordEmbeddingsPath(const std::string& x) { _wordEmbeddingsPath = x; }

    inline const std::string& tracePath() { return _tracePath; }
    inline void tracePath(const std::string& x) { _tracePath = x; }

    inline const std::string& evalPath() { return _evalPath; }
    inline void evalPath(const std::string& x) { _evalPath = x; }

    inline uint results() { return _results; }
    inline void results(const uint& x) { _results = x; }

    inline uint tiers() { return _tiers; }
    inline void tiers(const uint& x) { _tiers = x; }

    inline uint dimensions() { return _dimensions; }
    inline void dimensions(const uint& x) { _dimensions = x; }

    inline uint seed() { return _seed; }
    inline void seed(const uint& x) { _seed = x; }

  private:
    bool _help;
    bool _trace;
    bool _measure;
    bool _server;

    std::string _collectionPath;
    std::string _queryPath;
    std::string _relScoresPath;
    std::string _stopwordPath;
    std::string _wordEmbeddingsPath;
    std::string _tracePath;
    std::string _evalPath;

    uint _results;
    uint _tiers;
    uint _dimensions;
    uint _seed;
};
using argdesc_vt = std::vector<argdescbase_t<Args>*>;
void construct_arg_desc(argdesc_vt& aArgDesc);
