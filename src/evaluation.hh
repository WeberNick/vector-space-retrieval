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
#include <algorithm>
#include <iterator>
#include <ios>
#include <fstream>
#include <ctime>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


/**
 * @brief This singleton class is used for evaluating runtime and information retrieval performance.
 * 
 * @section	DESCRIPTION
 * This class consists of several nested classes not visible to the outside. The nested classes all 
 * handle different functionalities of evaluation. The nested class IR_PerformanceManager provides
 * several IR evaluation methods such as accuracy, F-measure, MAP and DCG. For more details, read
 * the description of the nested class. Users of this class do not have to worry about the internal 
 * details. A simple call to start/stop measures the runtime performance between the start and stop
 * call. To Evaluate IR performance, a call to evalIR is enough to evaluate the ranking with all
 * implemented IR evaluation techniques (as named above).
 * 
 * @section USE
 * 1. By calling Evaluation::getInstance(), the caller gets access to the evaluation instance.
 * 2. Calling start on the evaluation object, starts run time measurement. Two arguments have to be 
 * provided: the enum value of the current IR mode to measure and a query name
 * 3. Calling stop on the evaluation object, stops the runtime measurement and persists the 
 * measurement results in an internal data structure
 * 4. Calling evalIR with the same arguments as start plus a ranking will calculate all IR evaluation 
 * methods and persist them in an internal data structure
 * 5. A call to constructJSON will create the JSON object which can be used to parse the results etc..
 */
class Evaluation
{
    private:
        /**
         * @brief Class to handle the IR evaluation
         * @section DESCRIPTION
         * TBD
         */
        class IR_PerformanceManager
        {
            public:
                /**
                 * @brief Class representing a single relevance score for a query - document pair
                 */
                class RelScore
                {
                    public:
                        RelScore() = delete;
                        RelScore(const std::string& aQueryID, const std::string& aDocID, const uint aScore) :
                            _qID(aQueryID), _dID(aDocID), _score(aScore){}
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
                /**
                 * @brief Calculate the number of TP, TN, FP and FN in the produced ranking for a given query
                 * @param aQueryID  a query to calculate the TP/TN/FP/FN for
                 * @param aRanking  a ranking to calculate the TP/TN/FP/FN for
                 * @param aTP       reference to store the number of true positives in
                 * @param aTN       reference to store the number of true negatives in
                 * @param aFP       reference to store the number of false positives in
                 * @param aFN       reference to store the number of false negatives in
                 */
                void tp_tn_fp_fn(const std::string& aQueryID, const sizet_vt& aRanking, size_t& aTP, size_t& aTN, size_t& aFP, size_t& aFN);

                /**
                 * @brief Calculates the accuracy of a ranking as ((tp + tn) / (tp + tn + fp + fn))
                 * @param aQueryID  a query to calculate the accuracy for
                 * @param aRanking  a ranking of documents produced for the respective query
                 * @return the calculated accuracy
                 */
                double accuracy(const std::string& aQueryID, const sizet_vt& aRanking);

                /**
                 * @brief Calculates the precision of a ranking as (tp / (tp + fp))
                 * @param aQueryID  a query to calculate the precision for
                 * @param aRanking  a ranking of documents produced for the respective query
                 * @return the calculated precision
                 */
                double precision(const std::string& aQueryID, const sizet_vt& aRanking);

                /**
                 * @brief Calculates the recall of a ranking as ((tp) / (tp + fn))
                 * @param aQueryID  a query to calculate the recall for
                 * @param aRanking  a ranking of documents produced for the respective query
                 * @return the calculated recall
                 */
                double recall(const std::string& aQueryID, const sizet_vt& aRanking);

                /**
                 * @brief Calculates the F-measure of a ranking as (((beta^2 + 1) * P * R) / (beta^2 * P + R))
                 * @param aQueryID  a query to calculate the F-measure for
                 * @param aRanking  a ranking of documents produced for the respective query
                 * @param aBeta     a beta value to influence the weight given to precision
                 *                  (=1 equal weight on P/R, < 1 more weight on R, > 1 more weight on P
                 * @return the calculated F-measure
                 */
                double fMeasure(const std::string& aQueryID, const sizet_vt& aRanking, const double aBeta = 1);

                /**
                 * @brief Calculates the average precision of a ranking
                 * @param aQueryID  a query to calculate the fverage precision for
                 * @param aRanking  a ranking of documents produced for the respective query
                 * @return the calculated average precision
                 */
                double avgPrecision(const std::string& aQueryID, const sizet_vt& aRanking);

                /**
                 * @brief Calculates the mean average precision (MAP) of all queries 
                 * @param aAvgPrecisionMap a map containing all avg precision values for each query
                 * @return the calculated mean average precision
                 */
                double meanAvgPrecision(const std::unordered_map<std::string, double>& aAvgPrecisionMap);
                
                /**
                 * @brief Calculates the basic Discounted Cumulative Gain (DCG) as sum(rel_i / log_2(i + 1))
                 * @param aQueryID  a query to calculate the DCG for
                 * @param aRanking  a ranking of documents produced for the respective query
                 * @return the calculated basic DCG
                 */
                double bDCG(const std::string& aQueryID, const sizet_vt& aRanking);

                /**
                 * @brief Calculates the relevance Discounted Cumulative Gain (DCG with higher emphasis on 
                 *          relevance) as sum((2^rel_i - 1) / log_2(i + 1))
                 * @param aQueryID  a query to calculate the DCG for
                 * @param aRanking  a ranking of documents produced for the respective query
                 * @return the calculated relevance DCG
                 */
                double rDCG(const std::string& aQueryID, const sizet_vt& aRanking);

                /**
                 * @brief Calculates the ideal Discounted Cumulative Gain (best possible DCG score) as 
                 *          sum((2^rel_i - 1) / log_2(i + 1))
                 * @param aQueryID  a query to calculate the DCG for
                 * @return the calculated ideal DCG
                 */
                double iDCG(const std::string& aQueryID);

                /**
                 * @brief Calculates the normalized Discounted Cumulative Gain (DCG) as (DCG / IDCG) 
                 * @param aQueryID  a query to calculate the DCG for
                 * @param aRanking  a ranking of documents produced for the respective query
                 * @return the calculated normalized DCG
                 */
                double nDCG(const std::string& aQueryID, const sizet_vt& aRanking, const bool aBDCG = true);

            private:
                const scores_vt& getQueryScores(const std::string& aQueryID);
                sizet_vt getRelevantDocIDs(const std::string& aQueryID);
                uint getScore(const std::string& aQueryID, const std::string& aDocID);

            private:
                query_scores_mt  _queryScores; 
                const CB* _cb;
        };

        /**
         * @brief Container of query results for one specific IR mode (vanilla, cluster, ...)
         */
        class EvalResults
        {
            public:
                using query_rtp_mt = std::unordered_map<std::string, double>;
                using query_acc_mt = std::unordered_map<std::string, double>;
                using query_pre_mt = std::unordered_map<std::string, double>;
                using query_rec_mt = std::unordered_map<std::string, double>;
                using query_fms_mt = std::unordered_map<std::string, double>;
                using query_apr_mt = std::unordered_map<std::string, double>;
                using query_map_mt = std::unordered_map<std::string, double>;
                using query_dcg_mt = std::unordered_map<std::string, double>;

            public:
                EvalResults() : 
                    _mode(""), 
                    _perfRuntime(), 
                    _perfAccuracy(),
                    _perfPrecision(),
                    _perfRecall(),
                    _perfFmeasure(),
                    _perfAvgPrecision(),
                    _perfDCG(),
                    _perfMAP(0)
                {}
                explicit EvalResults(const EvalResults&) = delete;
                explicit EvalResults(EvalResults&&) = delete;
                EvalResults& operator=(const EvalResults&) = delete;
                EvalResults& operator=(EvalResults&&) = delete;
                ~EvalResults() = default;
            
            public:
                //getter
                inline const query_rtp_mt& getPerfRuntime() const { return _perfRuntime; }
                inline double getPerfRuntime(const std::string& aQueryID) const { return getPerfRuntime().at(aQueryID); }
                inline double getPerfRuntime(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfRuntime(aQueryID); }
                inline const query_acc_mt& getPerfAccuracy() const { return _perfAccuracy; }
                inline double getPerfAccuracy(const std::string& aQueryID) const { return getPerfAccuracy().at(aQueryID); }
                inline double getPerfAccuracy(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfAccuracy(aQueryID);}
                inline const query_pre_mt& getPerfPrecision() const { return _perfPrecision; }
                inline double getPerfPrecision(const std::string& aQueryID) const { return getPerfPrecision().at(aQueryID); }
                inline double getPerfPrecision(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfPrecision(aQueryID);}
                inline const query_rec_mt& getPerfRecall() const { return _perfRecall; }
                inline double getPerfRecall(const std::string& aQueryID) const { return getPerfRecall().at(aQueryID); }
                inline double getPerfRecall(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfRecall(aQueryID);}
                inline const query_fms_mt& getPerfFMeasure() const { return _perfFmeasure; }
                inline double getPerfFMeasure(const std::string& aQueryID) const { return getPerfFMeasure().at(aQueryID); }
                inline double getPerfFMeasure(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfFMeasure(aQueryID);}
                inline const query_apr_mt& getPerfAvgPrecision() const { return _perfAvgPrecision; }
                inline double getPerfAvgPrecision(const std::string& aQueryID) const { return getPerfAvgPrecision().at(aQueryID); }
                inline double getPerfAvgPrecision(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfAvgPrecision(aQueryID);}
                inline const query_dcg_mt& getPerfDCG() const { return _perfDCG; }
                inline double getPerfDCG(const std::string& aQueryID) const { return getPerfDCG().at(aQueryID); }
                inline double getPerfDCG(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfDCG(aQueryID);}
                inline double getPerfMAP() const { return _perfMAP; }
                inline double getPerfMAP() { return static_cast<const EvalResults&>(*this).getPerfMAP();}
                //setter
                inline void init(const std::string& aMode){if(_mode==""){_mode=aMode;}}
                inline void setTime(const std::string& aQueryName, const double aTime)
                { _perfRuntime[aQueryName] = aTime; }
                inline void setAccuracy(const std::string& aQueryName, const double aAccuracy)
                { _perfAccuracy[aQueryName] = aAccuracy; }
                inline void setPrecision(const std::string& aQueryName, const double aPrecision)
                { _perfPrecision[aQueryName] = aPrecision; }
                inline void setRecall(const std::string& aQueryName, const double aRecall)
                { _perfRecall[aQueryName] = aRecall; }
                inline void setFMeasure(const std::string& aQueryName, const double aFMeasure)
                { _perfFmeasure[aQueryName] = aFMeasure; }
                inline void setAvgPrecision(const std::string& aQueryName, const double aAvgPrecision)
                { _perfAvgPrecision[aQueryName] = aAvgPrecision; }
                inline void setDCG(const std::string& aQueryName, const double aDCG)
                { _perfDCG[aQueryName] = aDCG; }
                inline void setMAP(const double aMAP)
                { _perfMAP = aMAP; }
        
            private:
                std::string     _mode;
                query_rtp_mt    _perfRuntime;
                query_acc_mt    _perfAccuracy;
                query_pre_mt    _perfPrecision;
                query_rec_mt    _perfRecall;
                query_fms_mt    _perfFmeasure;
                query_apr_mt    _perfAvgPrecision;
                query_dcg_mt    _perfDCG;
                double          _perfMAP;
        };

    public:
        using IR = IR_PerformanceManager;
        using RS = IR::RelScore;
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
        /**
         * @brief starts the run time performance measurement
         * @param aMode the mode for which the measurement is done (enum representing vanilla, tiered, etc..)
         * @pram aQueryName the name of the current query to evaluate
         */
        void start(const IR_MODE aMode, const std::string& aQueryName);

        /**
         * @brief stops the run time performance measurement and inserts the measurement result into 
         *        an internal data structure
         */
        void stop();

        /**
         * @brief evaluates the information retrieval performance and stores results in an internal data structure
         * @param aMode the mode for which the evaluation is done (enum representing vanilla, tiered, etc..)
         * @param aQueryName the name of the current query to evaluate
         * @param aRanking the ranking to evaluate
         */
        void evalIR(const IR_MODE aMode, const std::string& aQueryName, const pair_sizet_float_vt& aRanking);
        void evalIR(const IR_MODE aMode, const std::string& aQueryName, const sizet_vt& aRanking);

        /**
         * @brief constructs the physical JSON object with all evaluation results
         */
        void constructJSON();

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
using IRPM = Evaluation::IR;
