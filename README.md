# Vector Space Retrieval
An efficient Vector Space Model (VSM) implementation for retrieving medical data, built within the team project for the course [Information Retrieval and Web Search](http://dws.informatik.uni-mannheim.de/en/teaching/courses-for-master-candidates/ie663websearchandinformationretrieval/). This is an active project and currently in development. Expect major changes in the comming weeks.

## Table of Contents
 * [Problem Description](#problem-description)
 * [Prerequisites](#prerequisites)
 * [Project Structure](#project-structure)
 * [Getting Started](#getting-started)
 
## Problem Description
Describe the problem we want to solve in more detail here. Maybe just copy paste from report..


## Prerequisites
There are several tools you will need to install and execute the application. In the following is a list with all required tools and technologies required for installing and running the system:
 * _UNIX-like_ operating system: All development and testing was carried out on UNIX-like operating systems. On _Windows_ we encountered several compiler problems. Although fixed, we still strongly recommend to use a UNIX-like operating system (tested under _macOS High Sierra Version 10.13.4_, Arch Linux [Version..] and Ubuntu [Version...])
 * [CMake](https://cmake.org): We use CMake as platform independent build system. Before compiling several requirements will be checked by CMake and the compilation process will either fail or a warning will be emitted if requirements are not satisfied.
 * [GNU Make](https://www.gnu.org/software/make/): As development and testing was carried out on UNIX-like operating systems, the de-facto standard build and install system _GNU Make_ is used internally by CMake.
 * [_C++17_ compatible compiler](https://gcc.gnu.org): As all tests were carried out with _GCC 7.3.0_, we recommend to use this one or a never version
 * [_boost_](https://www.boost.org): We use the popular C++ library _boost_ for several convenience functionalities such as efficiently splitting strings or dynamically creating bit vectors. If not already installed on your system, you will need to download the _Header-Only Library_ part of _boost_.
 * [Node.js](https://nodejs.org/en/): If you want to use the system with a web interface you will need to have _Node.js_ installed. (more details alex..)
 * Other libraries used in our project are the [Oleander Stemming Library](http://www.oleandersolutions.com/stemming/stemming.html) and a [C++ JSON Library](https://nlohmann.github.io/json/). The source code of these libraries is automatically cloned when running our installation script.

## Project Structure

### data/
 All data files required for the system are stored here. By default, the system will search here for the required data files. Optionally, you can provide your own file paths as command line argument (for more details see [Command Line Arguments](#command-line-arguments)). __Note__: For queries, it is not possible to provide a custom path for each query type (_all_, _nontopictitles_, _titles_, and so on...) but you can provide a path to the directory where all the query files are stored. It is __IMPORTANT__ to keep the same naming conventions otherwise the query files can not be found (The convention is: `q-[Query Type].queries`).
 * __NICO DESCRIBE THIS OR REFER TO README IN DATA__
 * dev/raw: 
 * test/raw: 
 * train/raw: 
 * collection\_test.docs:
 * collection\_test\_mwe.docs:
 * d-collection.docs:
 * q-all.queries:
 * q-nontopictitles.queries:
 * q-titles.queries:
 * q-viddesc.queries:
 * q-vidtitles.queries:
 * s-3.qrel:
 * stopwords.large:

### docs/
(__ALEX DESCRIBE THIS__)

### evsr-web/
(__ALEX DESCRIBE THIS__)

### python/
 * preprocessing/: (__NICO DESCRIBE THIS__)
 * wiki-crawler/: (__ALEX DESCRIBE THIS__)

### src/
Directory of the C++ source code of the system and its libraries. For further information regarding the source code, take a look into the [Documentation](#docs/) at doc/.

### test/
Directory of the unit tests.

## Getting Started
The build and installation process will be described in the following. Follow the [Quick Start Guide](#quick-start-guide) for a fast installation and get the system running. This works only if the _boost_ library can be located in its default path. For a more detailed installation guide or if you encounter problems, take a look at [Detailed Installation Guide](#detailed-installation-guide).

### Quick Start Guide

1. Make sure all the [Requirements](#prerequisites) are satisfied

2. Clone the source with `git`:
```
git clone https://github.com/WeberNick/vector-space-retrieval.git
```

3. Build and install:
```
cd vector-space-retrieval
./install.sh
```

4. Run
```
./bin/evsr_run
```

### Detailed Installation Guide

 * Make sure all the [Requirements](#prerequisites) are satisfied
 * Clone the source with `git`:
```
git clone https://github.com/WeberNick/vector-space-retrieval.git
```

#### Set Up
 * To install and build the system, several additional options can be (and sometimes must be) provided to the installation process. A complete list of options is provided later.
 * If _boost_ is not located in the default search path (on Linux `/usr/include/boost`, `/usr/local/Cellar/boost` if installed using _homebrew_), you have to provide the absolute or relative path to the include directory of _boost_ via the `-b`option.
 * If _GCC_ is not the default compiler on your system (on macOS `g++` is often mapped to use `clang` instead) you may want to provide a path to the C++ compiler via the `-cxx` option (e.g., `/usr/bin/g++`)
```
cd vector-space-retrieval
./install.sh [-b [/path/to/boost/include]] [-cxx [/path/to/c++-compiler]] 
```

(__ALEX PROVIDE LIST OF OTHER INTERESTING OPTIONS__)

#### Run
 * To run the system, several additional command line arguments can be provided to the executable. A complete list of command line arguments is provided in the console by running
```
./bin/evsr_run --help
```
 * (__DISCUSS THE MOST IMPORTANT ARGUMENTS__)

#### Command Line Arguments
In the following table we briefly introduce all the command line arguments.
| Command Line Argument | Description                                         | Default                      | Expects parameter |
|-----------------------|-----------------------------------------------------|------------------------------|-------------------|
|                --help | Print all command line arguments                    | false                        | -                 |
|               --trace | Activate tracing                                    | false                        | -                 |
|             --measure | Activate the performance measurement                | false                        | -                 |
|     --collection-path | Path to the collection file                         | ./data/d-collection.docs     | String Path       |
|         --query--path | Path to the query directory                         | ./data/                      | String Path       |
|         --scores-path | Path to the relevance score file                    | ./data/s-3.qrel              | String Path       |
|       --stopword-path | Path to the stopword file                           | ./data/stopwords.large       | String Path       |
|     --word-embeddings | Path to the word embeddings file                    | ./data/w2v/glove.6B.300d.txt | String Path       |
|          --trace-path | Path to the log directory                           | ./                           | String Path       |
|           --eval-path | Path to the evaluation directory                    | ./                           | String Path       |
|                --topk | The top K results returned                          | 10                           | unsigned int      |
|               --tiers | Number of tiers used by the  tiered index           | 10                           | unsigned int      |
|          --dimensions | Number of dimensions used by the random projections | 1000                         | unsigned int      |




## ./

* main.cc - Main entry point, starts application
* CMakeLists.txt - Links `evsr_lib` to main executable `evsr_run`, build documentation, builds tests

## ./src

* CMakeLists.txt builds the `evsr_lib` executable which is linked to the `evsr_run` and the unit tests later on
* Files and directories for our efficient vector space retrieval IR

## ./preprocessing

* preprocess.py - aggregate queries and document collection from [nfcorpus](http://www.cl.uni-heidelberg.de/statnlpgroup/nfcorpus/)

## ./tests

* CMakeLists.txt downloads googletest library if not already installed and builds googletest and our own unit tests

### ./tests/unit_tests

* CMakeLists.txt builds `Unit_Tests_run` and links it against our own `evsr_lib`, `gtest` and `gtest_main`
* Also contains tests for different parts of our library
* Tests can be run by either exectuing `Unit_Tests_run` or Ctest from CMake

# Set Up

To initial everything after cloning or unzipping the repo run `./configure.sh` to create needed directories, download external libraries via CMake and let CMake compile everything.

**IMPORTANT:**

You need to have Boost installed on your machine in order to build the binary. It is recommended to use the `-b` option of `./configure.sh` to tell the script the location of Boost

```
 SYNOPSIS
    configure.sh [-h] [-c[compiler_path]]  [-cxx[compiler_path]] [-b[boost_path]] [-a]

 DESCRIPTION
    Setting up CMake, directory structure and compile everything to start the main evsr executable

 OPTIONS
    -c [path], --c [path]         Set a custom C compiler path
    -cxx [path], --cxx [path]     Set a custom C++ compiler path
    -b, --boost                   Set path to your boost library
    -a, --all                     Also delete external lib folders and clone again

 EXAMPLES
    ./configure.sh -c /usr/local/Cellar/gcc/7.3.0_1/bin/gcc-7 -cxx /usr/local/Cellar/gcc/7.3.0_1/bin/g++-7 -b /usr/local/Cellar/boost/1.66.0/ -a
```

# Server

To use a UI to interact with the binary, you need to have `Node.js` and `npm` installed on your machine.
Issue the following commands :

Install dependencies:

```
$ cd ./evsr-web
$ npm install
```

```
$ cd ./evsr-web/client
$ npm install
```

```
$ cd ./evsr-web/server
$ npm install
```

Build the React application & start server, then [localhost:8080](localhost:8080)

```
$ cd ./evsr-web
$ npm run start
```

Build

The server uses the build version of the React application. If you want to not use the build React application, issue the following commands in two separate terminal sessions.

1st Terminal

```
$ cd ./evsr-web/client
$ npm run start
```

2nd Terminal

```
$ cd ./evsr-web/server
$ npm run start
```

Then visit [localhost:3000](localhost:3000). This is now the development server of the React applications which interacts with the backend of the server on localhost:8080
