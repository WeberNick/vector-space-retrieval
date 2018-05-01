/*********************************************************************
 * @file    evaluation.hh
 * @author 	Nick Weber
 * @date    Apr 29, 2018
 * @brief 	this class handels the evaluation of query performance
 * @bugs 	TBD
 * @todos 	TBD
 * 
 * @section	DESCRIPTION
 * TBD
 * 
 * @section USE
 * TBD
 ********************************************************************/
#pragma once

#include "types.hh"
#include "trace.hh"
#include "measure.hh"
#include "lib/nlohmann/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

#include <string>
#include <vector>
#include <ios>
#include <fstream>
#include <ctime>
#include <iostream>
#include <unordered_map>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

using query_mt = std::unordered_map<std::string, double>;
using eval_mt = std::unordered_map<std::string, query_mt>;

class Evaluation
{
    private:
        explicit Evaluation();
        explicit Evaluation(const Evaluation&) = delete;
        explicit Evaluation(Evaluation&&) = delete;
        Evaluation& operator=(const Evaluation&) = delete;
        Evaluation& operator=(Evaluation&&) = delete;
        ~Evaluation() = default;

    public:
        inline static Evaluation& getInstance()
        {
            static Evaluation lInstance;
            return lInstance;
        }
        void init(const CB& aControlBlock);

    public:
        //for run time performance
        void start(const IR_MODE aMode, const std::string& aQueryName);
        void stop(); //automatically writes performance to file
        void evalIR();
        void constructJSON();
    

    public:
        //getter
        //setter
        
    private:

    private:
        std::string     _mode;
        std::string     _queryName;
        eval_mt         _evalMap;
        std::string     _evalPath;
        Measure*        _measureInstance;
        double          _time;
        bool            _started;

        const CB*    _cb;
};
