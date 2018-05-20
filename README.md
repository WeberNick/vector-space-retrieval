# Vector Space Retrieval
An efficient Vector Space Model (VSM) implementation for retrieving medical data, built within the team project for the course [Information Retrieval and Web Search](http://dws.informatik.uni-mannheim.de/en/teaching/courses-for-master-candidates/ie663websearchandinformationretrieval/). This is an active project and currently in development. Expect major changes in the comming weeks.

## Table of Contents
 * [Problem Description](#problem-description)
 * [Prerequisites](#prerequisites)
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
 * If you want to use the system with a web interface you will need to have [Node.js](https://nodejs.org/en/) installed. (more details alex..)
 * Other libraries used in our project are the [Oleander Stemming Library](http://www.oleandersolutions.com/stemming/stemming.html) and a [C++ JSON Library](https://nlohmann.github.io/json/). The source code of these libraries is automatically cloned when running our installation script.

## Getting Started



# Project Structure

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
