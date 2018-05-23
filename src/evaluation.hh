/*********************************************************************
 * @file    evaluation.hh
 * @author 	Nick Weber
 * @date    Apr 29, 2018
 * @brief 	Class handles the evaluation of query performance
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

#include "measure.hh"
#include "trace.hh"
#include "types.hh"
#include "exception.hh"
#include "vec_util.hh"
#include "file_util.hh"
#include "document.hh"
#include "document_manager.hh"
#include "lib/nlohmann/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

#include <algorithm>
#include <cmath>
#include <ctime>
#include <experimental/filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <functional>
#include <iterator>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
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
class Evaluation {
  private:
    /**
     * @brief Class to handle the IR evaluation
     * @section DESCRIPTION
     * TBD
     */
    class IR_PerformanceManager {
      public:
        /**
         * @brief Class representing a single relevance score for a query - document pair
         */
        class RelScore {
          public:
            /* Constructors, assignment operators and destructor */
            RelScore() = default;
            RelScore(const std::string& aQueryID, const std::string& aDocID, const uint aScore) : _qID(aQueryID), _dID(aDocID), _score(aScore) {}
            RelScore(const RelScore&) = default;
            RelScore(RelScore&&) = default;
            RelScore& operator=(const RelScore&) = default;
            RelScore& operator=(RelScore&&) = default;
            ~RelScore() = default;

          public:
            inline const std::string& getQueryID() const { return _qID; }
            inline const std::string& getDocumentID() const { return _dID; }
            inline uint getScore() const { return _score; }
            inline const std::string& getQueryID() { return _qID; }
            inline const std::string& getDocumentID() { return _dID; }
            inline uint getScore() { return _score; }

          private:
            std::string _qID;
            std::string _dID;
            uint _score;
        };

      public:
        using scores_vt = std::vector<RelScore>;
        using query_scores_mt = std::unordered_map<std::string, scores_vt>;

      private:
        /* Constructors, assignment operators and destructor */
        explicit IR_PerformanceManager();
        explicit IR_PerformanceManager(const IR_PerformanceManager&) = delete;
        explicit IR_PerformanceManager(IR_PerformanceManager&&) = delete;
        IR_PerformanceManager& operator=(const IR_PerformanceManager&) = delete;
        IR_PerformanceManager& operator=(IR_PerformanceManager&&) = delete;
        ~IR_PerformanceManager() = default;

      public:
        inline static IR_PerformanceManager& getInstance() {
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
        double bDCG(const std::string& aQueryID, const sizet_vt& aRanking, uint& aCounter);

        /**
         * @brief Calculates the relevance Discounted Cumulative Gain (DCG with higher emphasis on
         *          relevance) as sum((2^rel_i - 1) / log_2(i + 1))
         * @param aQueryID  a query to calculate the DCG for
         * @param aRanking  a ranking of documents produced for the respective query
         * @return the calculated relevance DCG
         */
        double rDCG(const std::string& aQueryID, const sizet_vt& aRanking, uint& aCounter);

        /**
         * @brief Calculates the ideal Discounted Cumulative Gain (best possible DCG score) as
         *          sum((2^rel_i - 1) / log_2(i + 1))
         * @param aQueryID  a query to calculate the DCG for
         * @return the calculated ideal DCG
         */
        double iDCG(const std::string& aQueryID, const uint aRelDocsFound);

        /**
         * @brief Calculates the normalized Discounted Cumulative Gain (DCG) as (DCG / IDCG)
         * @param aQueryID  a query to calculate the DCG for
         * @param aRanking  a ranking of documents produced for the respective query
         * @return the calculated normalized DCG
         */
        double nDCG(const std::string& aQueryID, const sizet_vt& aRanking, const bool aBDCG = false);

      private:
        const scores_vt& getQueryScores(const std::string& aQueryID);
        sizet_vt getRelevantDocIDs(const std::string& aQueryID);
        uint getScore(const std::string& aQueryID, const std::string& aDocID);

      private:
        query_scores_mt _queryScores;
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
        /* Constructors, assignment operators and destructor */
        EvalResults() :
            _type(kNoType), _mode(kNoMode), _perfRuntime(), _perfAccuracy(), _perfPrecision(), _perfRecall(), _perfFmeasure(), _perfAvgPrecision(), _perfDCG(), _perfMAP(0) {}
        explicit EvalResults(const EvalResults&) = delete;
        explicit EvalResults(EvalResults&&) = delete;
        EvalResults& operator=(const EvalResults&) = delete;
        EvalResults& operator=(EvalResults&&) = delete;
        ~EvalResults() = default;

        /**
         * @brief Initializes the EvalResults object with its corresponding mode 
         * @param aMode the mode to initialize the EvalResult object with
         */
        inline void init(QUERY_TYPE aQueryType, IR_MODE aMode) 
        {
            if(_type == kNoType && _mode == kNoMode) 
            {
                _type = aQueryType;
                _mode = aMode;
                const std::string lTraceMsg = std::string("Evaluation::EvalResults: Initialized for type '") + typeToString(_type) + std::string("', mode '") + modeToString(_mode) + std::string("'");
                TRACE(lTraceMsg);
            }
        }

      public:
        /**
         * @brief Getter for the map containing runtime measurements for each query
         * @return the map with runtime performance measurements
         */
        inline const query_rtp_mt& getPerfRuntime() const { return _perfRuntime; }

        /**
         * @brief Getter for a runtime measurement for a specific query
         * @param aQueryID the query to retrieve the measurement for
         * @return the performance measurement
         */
        inline double getPerfRuntime(const std::string& aQueryID) const { return getPerfRuntime().at(aQueryID); }
        inline double getPerfRuntime(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfRuntime(aQueryID); }

        /**
         * @brief Getter for the map containing accuracy measurements for each query
         * @return the map with accuracy measurements
         */
        inline const query_acc_mt& getPerfAccuracy() const { return _perfAccuracy; }

        /**
         * @brief Getter for a accuracy measurement for a specific query
         * @param aQueryID the query to retrieve the measurement for
         * @return the performance measurement
         */
        inline double getPerfAccuracy(const std::string& aQueryID) const { return getPerfAccuracy().at(aQueryID); }
        inline double getPerfAccuracy(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfAccuracy(aQueryID); }

        /**
         * @brief Getter for the map containing precision measurements for each query
         * @return the map with precision measurements
         */
        inline const query_pre_mt& getPerfPrecision() const { return _perfPrecision; }

        /**
         * @brief Getter for a precision measurement for a specific query
         * @param aQueryID the query to retrieve the measurement for
         * @return the performance measurement
         */
        inline double getPerfPrecision(const std::string& aQueryID) const { return getPerfPrecision().at(aQueryID); }
        inline double getPerfPrecision(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfPrecision(aQueryID); }

        /**
         * @brief Getter for the map containing recall measurements for each query
         * @return the map with recall measurements
         */
        inline const query_rec_mt& getPerfRecall() const { return _perfRecall; }

        /**
         * @brief Getter for a recall measurement for a specific query
         * @param aQueryID the query to retrieve the measurement for
         * @return the performance measurement
         */
        inline double getPerfRecall(const std::string& aQueryID) const { return getPerfRecall().at(aQueryID); }
        inline double getPerfRecall(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfRecall(aQueryID); }

        /**
         * @brief Getter for the map containing F-measure  measurements for each query
         * @return the map with F-measure measurements
         */
        inline const query_fms_mt& getPerfFMeasure() const { return _perfFmeasure; }

        /**
         * @brief Getter for a F-measure measurement for a specific query
         * @param aQueryID the query to retrieve the measurement for
         * @return the performance measurement
         */
        inline double getPerfFMeasure(const std::string& aQueryID) const { return getPerfFMeasure().at(aQueryID); }
        inline double getPerfFMeasure(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfFMeasure(aQueryID); }

        /**
         * @brief Getter for the map containing averga precision  measurements for each query
         * @return the map with average precision measurements
         */
        inline const query_apr_mt& getPerfAvgPrecision() const { return _perfAvgPrecision; }
        inline const query_apr_mt& getPerfAvgPrecision() { return _perfAvgPrecision; }

        /**
         * @brief Getter for a average precision measurement for a specific query
         * @param aQueryID the query to retrieve the measurement for
         * @return the performance measurement
         */
        inline double getPerfAvgPrecision(const std::string& aQueryID) const { return getPerfAvgPrecision().at(aQueryID); }
        inline double getPerfAvgPrecision(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfAvgPrecision(aQueryID); }

        /**
         * @brief Getter for the map containing DCG measurements for each query
         * @return the map with DCG measurements
         */
        inline const query_dcg_mt& getPerfDCG() const { return _perfDCG; }

        /**
         * @brief Getter for a DCG measurement for a specific query
         * @param aQueryID the query to retrieve the measurement for
         * @return the performance measurement
         */
        inline double getPerfDCG(const std::string& aQueryID) const { return getPerfDCG().at(aQueryID); }
        inline double getPerfDCG(const std::string& aQueryID) { return static_cast<const EvalResults&>(*this).getPerfDCG(aQueryID); }

        /**
         * @brief Getter for the mean average precision measurement 
         * @return the mean average precision measurement
         */
        inline double getPerfMAP() const { return _perfMAP; }
        inline double getPerfMAP() { return static_cast<const EvalResults&>(*this).getPerfMAP(); }

        /**
         * @brief Setter for a runtime measurement of a specific query
         * @param aQueryName    the query to set the measurement for
         * @param aTime         the runtime measurement to assign
         */
        inline void setTime(const std::string& aQueryName, const double aTime) { _perfRuntime[aQueryName] = aTime; }
        
        /**
         * @brief Setter for a accuracy measurement of a specific query
         * @param aQueryName    the query to set the measurement for
         * @param aAccuracy     the accuracy measurement to assign
         */
        inline void setAccuracy(const std::string& aQueryName, const double aAccuracy) { _perfAccuracy[aQueryName] = aAccuracy; }
        
        /**
         * @brief Setter for a precision measurement of a specific query
         * @param aQueryName    the query to set the measurement for
         * @param aPrecision    the precision measurement to assign
         */
        inline void setPrecision(const std::string& aQueryName, const double aPrecision) { _perfPrecision[aQueryName] = aPrecision; }
        
        /**
         * @brief Setter for a recall measurement of a specific query
         * @param aQueryName    the query to set the measurement for
         * @param aRecall       the recall measurement to assign
         */
        inline void setRecall(const std::string& aQueryName, const double aRecall) { _perfRecall[aQueryName] = aRecall; }
        
        /**
         * @brief Setter for a F-Measure measurement of a specific query
         * @param aQueryName    the query to set the measurement for
         * @param aFMeasure     the F-Measure measurement to assign
         */
        inline void setFMeasure(const std::string& aQueryName, const double aFMeasure) { _perfFmeasure[aQueryName] = aFMeasure; }
        
        /**
         * @brief Setter for a average precision measurement of a specific query
         * @param aQueryName    the query to set the measurement for
         * @param aAvgPrecision the average precision measurement to assign
         */
        inline void setAvgPrecision(const std::string& aQueryName, const double aAvgPrecision) { _perfAvgPrecision[aQueryName] = aAvgPrecision; }
        
        /**
         * @brief Setter for a DCG measurement of a specific query
         * @param aQueryName    the query to set the measurement for
         * @param aDCG          the DCG measurement to assign
         */
        inline void setDCG(const std::string& aQueryName, const double aDCG) { _perfDCG[aQueryName] = aDCG; }

        /**
         * @brief Setter for a runtime measurement of a specific query
         * @param aQueryName    the query to set the measurement for
         * @param aMap          the map  measurement to assign
         */
        inline void setMAP(double aMAP) { _perfMAP = aMAP; }

      private:
        QUERY_TYPE      _type;
        IR_MODE         _mode;
        query_rtp_mt _perfRuntime;
        query_acc_mt _perfAccuracy;
        query_pre_mt _perfPrecision;
        query_rec_mt _perfRecall;
        query_fms_mt _perfFmeasure;
        query_apr_mt _perfAvgPrecision;
        query_dcg_mt _perfDCG;
        double _perfMAP;
    };

  public:
    using IR = IR_PerformanceManager;
    using RS = IR::RelScore;
    using results_mt = std::unordered_map<IR_MODE, EvalResults>;
    using type_results_mt = std::unordered_map<QUERY_TYPE, results_mt>;

  private:
    /* Constructors, assignment operators and destructor */
    Evaluation();
    explicit Evaluation(const Evaluation&) = delete;
    explicit Evaluation(Evaluation&&) = delete;
    Evaluation& operator=(const Evaluation&) = delete;
    Evaluation& operator=(Evaluation&&) = delete;
    ~Evaluation() = default;

  public:
    /**
     * @brief This getInstance() call is the only way to access the single instance of the Evaluation's object
     * @return the single instance of the Evaluation object
     */
    inline static Evaluation& getInstance() 
    {
        static Evaluation lInstance;
        return lInstance;
    }

    /**
     * @brief Initializes the single Evaluation instance 
     * @param aControlBlock the control block containing specific runtime information
     */
    void init(const CB& aControlBlock);

  public:
    /**
     * @brief starts the run time performance measurement
     * @param aMode the mode for which the measurement is done (enum representing vanilla, tiered, etc..)
     * @pram aQueryName the name of the current query to evaluate
     */
    void start(const QUERY_TYPE aQueryType, const IR_MODE aMode, const std::string& aQueryName);

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
    void evalIR(const QUERY_TYPE aQueryType, const IR_MODE aMode, const std::string& aQueryName, const pair_sizet_float_vt& aRanking);
    void evalIR(const QUERY_TYPE aQueryType, const IR_MODE aMode, const std::string& aQueryName, const sizet_vt& aRanking);

    /**
     * @brief constructs the physical JSON object with all evaluation results
     */
    void constructJSON();
    void constructJSON(const str_set& aQueryNames);

  private:
    inline EvalResults& getEvalResult(const QUERY_TYPE aQueryType, const IR_MODE aMode) {
        try {
            return _qTypeToEvalResults.at(aQueryType).at(aMode);
        } catch (const std::out_of_range& ex) {
            const std::string lErrMsg = std::string("Query Type '") + typeToString(aQueryType) + std::string("' or IR mode '") + modeToString(aMode) + std::string("' not found in evaluation data");
            TRACE(lErrMsg);
            throw InvalidArgumentException(FLF, lErrMsg);
        }
    }

  private:
    IR_PerformanceManager& _irpm;
    type_results_mt        _qTypeToEvalResults;
    QUERY_TYPE             _type;
    IR_MODE                _mode;
    str_set                _measuredQueries;
    std::string            _queryName;
    std::string            _evalPath;
    Measure*               _measureInstance;
    double                 _time;
    bool                   _started;

    const CB*       _cb;
};

using IRPM = Evaluation::IR;
