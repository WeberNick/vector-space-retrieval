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
#include "gnuplot-iostream.hh"

#include <string>
#include <vector>
#include <ios>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

class Evaluation
{
    public:
        explicit Evaluation(const std::string& aQueryName, const IR_MODE aMode);
        explicit Evaluation(const Evaluation&) = delete;
        explicit Evaluation(Evaluation&&) = delete;
        Evaluation& operator=(const Evaluation&) = delete;
        Evaluation& operator=(Evaluation&&) = delete;
        ~Evaluation() = default;

    public:
        //for run time performance
        void start();
        double stop(); //automatically writes performance to file
        void evalPerformanceOfIR();
    
    public:
        static void init(const CB& aControlBlock);
        //todo: discuss with alex/nico, how to plot exactly.. (per query, cluster of querys, avg of all queries)
        static void plot(const std::string& aQueryName, const std::string aPlotFormat = "pdf");

    public:
        //getter
        //setter
        /**
         * @brief changes the evaluation parameters to reuse an old evaluation object
         */
        void reuse(const std::string& aQueryName, const IR_MODE aMode);
        
    private:
        static const CB*    _cb;

    private:
        std::string     _queryName;
        std::string     _mode;
        Measure*        _measureInstance;
        double          _time;
        std::string     _evalPath;
        std::ofstream   _evalStream;

};

