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
#include "document_manager.hh"
#include "document.hh"
#include "lib/nlohmann/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

#include <string>
#include <vector>
#include <ios>
#include <fstream>
#include <ctime>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


class Evaluation
{
    private:
        class IR_PerformanceManager
        {
            public:
                class RelScore
                {
                    public:
                        explicit RelScore() = delete;
                        explicit RelScore(const std::string& aQueryID, const std::string& aDocID, const uint aScore);
                        explicit RelScore(const RelScore&) = delete;
                        explicit RelScore(RelScore&&) = default;
                        RelScore& operator=(const RelScore&) = delete;
                        RelScore& operator=(RelScore&&) = default;
                        ~RelScore() = default;
                    
                    public:
                        inline const std::string&   getQueryID() const { return _qID; }
                        inline const std::string&   getDocumentID() const { return _dID; }
                        inline uint                 getScore() const { return _score; }

                    private:
                        const std::string   _qID;
                        const std::string   _dID;
                        const uint          _score;
                };

            public:
                using scores_vt = std::vector<RelScore>;
                using query_scores_mt = std::unordered_map<std::string, scores_vt>;

            private:
                explicit IR_PerformanceManager();
                explicit IR_PerformanceManager(const IR_PerformanceManager&) = delete;
                explicit IR_PerformanceManager(IR_PerformanceManager&&) = delete;
                IR_PerformanceManager& operator=(const IR_PerformanceManager&) = delete;
                IR_PerformanceManager& operator=(IR_PerformanceManager&&) = delete;
                ~IR_PerformanceManager() = default;

            public:
                inline static IR_PerformanceManager& getInstance()
                {
                    static IR_PerformanceManager lInstance;
                    return lInstance;
                }
                void init(const CB& aControlBlock);

            public:
                uint getScore(const std::string& aQueryID, const std::string& aDocID);
                //basic DCG
                double bDCG(const std::string& aQueryID, const pair_sizet_float_vt& aRanking);
                //DCG with emphasis for relevant docs
                double rDCG(const std::string& aQueryID, const pair_sizet_float_vt& aRanking);
                //ideal DCG
                double iDCG(const std::string& aQueryID);
                //normalized DCG
                double nDCG(const std::string& aQueryID, const pair_sizet_float_vt& aRanking, const bool aBDCG);

            public:
                inline const scores_vt& getQueryScores(const std::string& aQueryID){ return _queryScores.at(aQueryID); }

            private:
                query_scores_mt  _queryScores; 
                const CB* _cb;
        };

        /*********************************************************************
         * @file    evaluation.hh
         * @author 	Nick Weber
         * @date    Mai 16, 2018
         * @brief   Class to manage evaluation results in multiple data structures
         * @bugs 	TBD
         * @todos 	TBD
         * 
         * @section	DESCRIPTION
         * TBD
         * 
         * @section USE
         * TBD
         ********************************************************************/
        class EvalResults
        {
            using query_rtp_mt = std::unordered_map<std::string, double>;
            using query_dcg_mt = std::unordered_map<std::string, double>;
            public:
                EvalResults();
                explicit EvalResults(const EvalResults&) = delete;
                explicit EvalResults(EvalResults&&) = delete;
                EvalResults& operator=(const EvalResults&) = delete;
                EvalResults& operator=(EvalResults&&) = delete;
                ~EvalResults() = default;
            
            public:
                //getter
                inline const query_rtp_mt& getPerfRT() const { return _perfRT; }
                inline const query_dcg_mt& getPerfIR() const { return _perfIR; }
                //setter
                inline void init(const std::string& aMode){if(_mode==""){_mode=aMode;}}
                inline void setTime(const std::string& aQueryName, const double aTime)
                { _perfRT[aQueryName] = aTime; }
                inline void setDCG(const std::string& aQueryName, const double aDCG)
                { _perfIR[aQueryName] = aDCG; }
        
            private:
                std::string     _mode;
                query_rtp_mt    _perfRT;
                query_dcg_mt    _perfIR;
                
        };

    using results_mt = std::unordered_map<std::string, EvalResults>;    

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
        void evalIR(const IR_MODE aMode, const std::string& aQueryName, const std::vector<std::pair<size_t, float>>& aRanking);
        void constructJSON();
    

    public:
        //getter
        //setter

    private:
        IR_PerformanceManager& _irpm;
        results_mt      _evalResults;
        std::string     _mode;
        std::string     _queryName;
        std::string     _evalPath;
        Measure*        _measureInstance;
        double          _time;
        bool            _started;

        const CB*    _cb;
};
