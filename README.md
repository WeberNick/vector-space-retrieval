# Vector Space Retrieval
An efficient Vector Space Model (VSM) implementation for retrieving medical data, built within the team project for the course "Information Retrieval and Web Search" (http://dws.informatik.uni-mannheim.de/en/teaching/courses-for-master-candidates/ie663websearchandinformationretrieval/). This is an active project and currently in development. Expect major changes in the comming weeks.

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

To initial everything after cloning or unzipping the repo run `./configure.sh` to create needed directories, download the GoogleTest library via CMake and let CMake compile everything.

```
 SYNOPSIS
    configure.sh [-h] [-c[compiler_path]]  [-cxx[compiler_path]]

 DESCRIPTION
    Setting up CMake, directory structure and compile everything to start the main evsr executable

 OPTIONS
    -c [path], --c [path]         Set a custom C compiler path
    -cxx [path], --cxx [path]     Set a custom C++ compiler path

 EXAMPLES
    configure.sh -c "/usr/local/Cellar/gcc/7.3.0_1/bin/gcc-7 -cxx /usr/local/Cellar/gcc/7.3.0_1/bin/g++-7
```
