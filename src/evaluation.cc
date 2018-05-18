#include "evaluation.hh"
#include "utility.hh"

IRPM::IR_PerformanceManager() : 
    _queryScores(), _cb(nullptr)
{}
                
void IRPM::init(const CB& aControlBlock)
{
    if(!_cb)
    {
        _cb = &aControlBlock;
        const std::string& lRelScorePath = _cb->relevanceScoresPath();
        std::ifstream file(lRelScorePath);
        std::string line;
        TRACE(std::string("Evaluation::IR_PerformanceManager: Start reading query relevance scores from '") + lRelScorePath + std::string("'"));
        while (std::getline(file, line)) {
            string_vt parts;
            Utility::StringOp::splitStringBoost(line, '~', parts);
            const std::string qID = parts[0];
            const std::string dID = parts[1];
            const uint score = std::stoul(parts[2]);
            _queryScores[qID].emplace_back(qID, dID, score);
        }
        TRACE(std::string("Evaluation::IR_PerformanceManager: Finished reading query relevance scores"));
        TRACE("Evaluation::IR_PerformanceManager: Initialized");
    }
}

void IRPM::tp_tn_fp_fn(const std::string& aQueryID, const sizet_vt& aRanking, size_t& aTP, size_t& aTN, size_t& aFP, size_t& aFN)
{
    const DocumentManager& lDocMngr = DocumentManager::getInstance();
    sizet_vt lDocIDs = lDocMngr.getIDs(); //ids of the doc collection
    sizet_vt lRetrieved = aRanking; //ids of the retrieved docs
    sizet_vt lNotRetrieved = Utility::VecUtil::difference(lDocIDs, lRetrieved); //ids of the not retrieved docs
    sizet_vt lRelevant = getRelevantDocIDs(aQueryID); //ids of the relevant docs
    sizet_vt lNotRelevant = Utility::VecUtil::difference(lDocIDs, lRelevant); //ids of the not relevant docs
    aTP = Utility::VecUtil::intersectionCount(lRelevant, lRetrieved);
    aTN = Utility::VecUtil::intersectionCount(lNotRelevant, lNotRetrieved);
    aFP = Utility::VecUtil::intersectionCount(lNotRelevant, lRetrieved);
    aFN = Utility::VecUtil::intersectionCount(lRelevant, lNotRetrieved);
}
    
double IRPM::accuracy(const std::string& aQueryID, const sizet_vt& aRanking)
{
    size_t lTP = 0, lTN = 0, lFP = 0, lFN = 0;
    tp_tn_fp_fn(aQueryID, aRanking, lTP, lTN, lFP, lFN);
    const double lNumerator = static_cast<double>(lTP) + static_cast<double>(lTN);
    const double lDenominator = lNumerator + static_cast<double>(lFP) + static_cast<double>(lFN);
    return lNumerator / lDenominator;
}

double IRPM::precision(const std::string& aQueryID, const sizet_vt& aRanking)
{
    size_t lTP = 0, lTN = 0, lFP = 0, lFN = 0;
    tp_tn_fp_fn(aQueryID, aRanking, lTP, lTN, lFP, lFN);
    const double lNumerator = static_cast<double>(lTP);
    const double lDenominator = lNumerator + static_cast<double>(lFP);
    return lNumerator / lDenominator;
}

double IRPM::recall(const std::string& aQueryID, const sizet_vt& aRanking)
{
    size_t lTP = 0, lTN = 0, lFP = 0, lFN = 0;
    tp_tn_fp_fn(aQueryID, aRanking, lTP, lTN, lFP, lFN);
    const double lNumerator = static_cast<double>(lTP);
    const double lDenominator = lNumerator + static_cast<double>(lFN);
    return lNumerator / lDenominator;
}

double IRPM::fMeasure(const std::string& aQueryID, const sizet_vt& aRanking, const double aBeta)
{
    const double lPrecision = precision(aQueryID, aRanking);
    const double lRecall = recall(aQueryID, aRanking);
    const double lNumerator = (std::pow(aBeta, 2) + 1) * lPrecision * lRecall; 
    const double lDenominator = (std::pow(aBeta, 2) * lPrecision) + lRecall;
    return lNumerator / lDenominator;
}

double IRPM::avgPrecision(const std::string& aQueryID, const sizet_vt& aRanking)
{
    const sizet_vt lRelevant = getRelevantDocIDs(aQueryID); //ids of the relevant docs
    double lSum = 0;
    for(size_t id : lRelevant)
    {
        size_t pos = std::distance(aRanking.cbegin(), std::find(aRanking.cbegin(), aRanking.cend(), id));
        if(pos < aRanking.size()) //found
        {
            const sizet_vt lSub(aRanking.cbegin(), aRanking.cbegin() + pos);
            lSum += precision(aQueryID, lSub);
        }
    }
    return lSum / lRelevant.size();
}

double IRPM::meanAvgPrecision(const std::unordered_map<std::string, double>& aAvgPrecisionMap)
{
    double lSum = 0;
    for(const auto& elem : aAvgPrecisionMap)
    {
        lSum += elem.second;
    }
    return lSum / aAvgPrecisionMap.size();
}

double IRPM::bDCG(const std::string& aQueryID, const sizet_vt& aRanking)
{    
    double lSum = 0;
    for(size_t i = 0; i < aRanking.size(); ++i)
    {
        const uint lScore = getScore(aQueryID, DocumentManager::getInstance().getDocument(aRanking[i]).getDocID());
        lSum += (lScore / std::log2(i + 1));
    }
    return lSum;
}

double IRPM::rDCG(const std::string& aQueryID, const sizet_vt& aRanking)
{
    double lSum = 0;
    for(size_t i = 0; i < aRanking.size(); ++i)
    {
        const uint lScore = getScore(aQueryID, DocumentManager::getInstance().getDocument(aRanking[i]).getDocID());
        const double lNumerator = std::pow(2, lScore) - 1;
        lSum += (lNumerator / std::log2(i + 1));
    }
    return lSum;
}

double IRPM::iDCG(const std::string& aQueryID)
{
    const scores_vt& lRelScores = getQueryScores(aQueryID);
    double lSum = 0;
    size_t i = 0;
    for(const RelScore& relScore : lRelScores)
    {
        const double lNumerator = std::pow(2, relScore.getScore()) - 1;
        lSum += (lNumerator / std::log2(i++ + 1));
    }
    return lSum;
}

double IRPM::nDCG(const std::string& aQueryID, const sizet_vt& aRanking, const bool aBDCG)
{
    double lDCG = (aBDCG) ? bDCG(aQueryID, aRanking) : rDCG(aQueryID, aRanking);
    return (lDCG / iDCG(aQueryID));
}

const IRPM::scores_vt& IRPM::getQueryScores(const std::string& aQueryID)
{ 
    try
    {
        return _queryScores.at(aQueryID);
    }
    catch (const std::out_of_range& ex) 
    {
        const std::string lErrMsg = std::string("No entry in evaluation scores map found for query ID '" + aQueryID + std::string("'"));
        TRACE(lErrMsg);
        throw InvalidArgumentException(FLF, lErrMsg);
    }
}

sizet_vt IRPM::getRelevantDocIDs(const std::string& aQueryID)
{
    const auto& lQueryScores = getQueryScores(aQueryID); 
    sizet_vt lRelevantDocIDs;
    lRelevantDocIDs.reserve(lQueryScores.size());
    for(const auto& score : lQueryScores)
    {
        try
        {
            lRelevantDocIDs.push_back(DocumentManager::getInstance().getDocument(score.getDocumentID()).getID());
        }
        catch(const InvalidArgumentException& ex) 
        {
            TRACE("Relevant document does not exist in the collection (can be ignored)");
        }
    } 
    return lRelevantDocIDs;
}

uint IRPM::getScore(const std::string& aQueryID, const std::string& aDocID)
{
    const scores_vt& lRelScores = getQueryScores(aQueryID);
    for(const RelScore& relScore : lRelScores)
    {
        if(relScore.getDocumentID() == aDocID){ return relScore.getScore(); } 
    }
    return 0;
}

Evaluation::Evaluation() : 
    _irpm(IR_PerformanceManager::getInstance()), 
    _evalResults(),
    _mode(""), 
    _queryName(""), 
    _evalPath(""), 
    _measureInstance(nullptr), 
    _time(0), 
    _started(false), 
    _cb(nullptr)
{
        _evalResults[modeToString(kVANILLA)].init(modeToString(kVANILLA));
        _evalResults[modeToString(kTIERED)].init(modeToString(kTIERED)); 
        _evalResults[modeToString(kTIEREDW2V)].init(modeToString(kTIEREDW2V)); 
        _evalResults[modeToString(kCLUSTER)].init(modeToString(kCLUSTER)); 
        _evalResults[modeToString(kCLUSTERW2V)].init(modeToString(kCLUSTERW2V)); 
        _evalResults[modeToString(kRANDOM)].init(modeToString(kRANDOM)); 
        _evalResults[modeToString(kRANDOMW2V)].init(modeToString(kRANDOMW2V)); 
}

void Evaluation::init(const CB& aControlBlock)
{
    if(!_cb)
    {
        _irpm.init(aControlBlock);
        _cb = &aControlBlock;
        _evalPath = std::string(_cb->evalPath()).append("eval/");
        fs::create_directory(_evalPath);
        TRACE(std::string("Evaluation: Initialized. Path set to '") + _evalPath + std::string("'"));
    }
}

void Evaluation::start(const IR_MODE aMode, const std::string& aQueryName)
{
    if(!_started)
    {
        _mode = modeToString(aMode);
        _queryName = aQueryName;
        _started = true;
        const std::string lTraceMsg = std::string("Evaluating performance of mode '") + _mode + std::string("' for query '") + _queryName + std::string("'");
        TRACE(lTraceMsg);
        _measureInstance = new Measure;
        _measureInstance->start();
    }
}

void Evaluation::stop()
{
    if(_started)
    {
        _measureInstance->stop();
        _time = _measureInstance->mTotalTime();
        delete _measureInstance;
        _measureInstance = nullptr;
        const std::string lTraceMsg = std::string("Time to process query '") + _queryName + std::string("' in mode '") + _mode + std::string("' : ") + std::to_string(_time);
        TRACE(lTraceMsg);
        getEvalResult(_mode).setTime(_queryName, _time);
        _mode = "";
        _queryName = "";
        _started = false;
    }
}

void Evaluation::evalIR(const IR_MODE aMode, const std::string& aQueryName, const pair_sizet_float_vt& aRanking)
{
    sizet_vt lRanking;
    lRanking.reserve(aRanking.size());
    //convert pair vector to ID only vector
    for(const auto& elem : aRanking)
    {
        lRanking.push_back(elem.first);
    }
    evalIR(aMode, aQueryName, lRanking);
}


void Evaluation::evalIR(const IR_MODE aMode, const std::string& aQueryName, const sizet_vt& aRanking)
{
    const std::string lMode = modeToString(aMode);
    EvalResults& lER = getEvalResult(lMode);
    std::string lTraceMsg = "";
    const std::string lMsgEnd(std::string(" (query: '") + aQueryName + std::string("', mode: '") + lMode + std::string("')"));
    lER.setAccuracy(aQueryName, _irpm.accuracy(aQueryName, aRanking));
    lTraceMsg = std::to_string(lER.getPerfAccuracy(aQueryName)) + std::string(" accuracy") + lMsgEnd;
    TRACE(lTraceMsg);

    lER.setPrecision(aQueryName, _irpm.precision(aQueryName, aRanking));
    lTraceMsg = std::to_string(lER.getPerfPrecision(aQueryName)) + std::string(" precision") + lMsgEnd;
    TRACE(lTraceMsg);
    
    lER.setRecall(aQueryName, _irpm.recall(aQueryName, aRanking));
    lTraceMsg = std::to_string(lER.getPerfRecall(aQueryName)) + std::string(" recall") + lMsgEnd;
    TRACE(lTraceMsg);
    
    lER.setFMeasure(aQueryName, _irpm.fMeasure(aQueryName, aRanking));
    lTraceMsg = std::to_string(lER.getPerfFMeasure(aQueryName)) + std::string(" F-Measure") + lMsgEnd;
    TRACE(lTraceMsg);
    
    lER.setAvgPrecision(aQueryName, _irpm.avgPrecision(aQueryName, aRanking));
    lTraceMsg = std::to_string(lER.getPerfAvgPrecision(aQueryName)) + std::string(" avg. precision") + lMsgEnd;
    TRACE(lTraceMsg);
    
    lER.setDCG(aQueryName, _irpm.nDCG(aQueryName, aRanking));
    lTraceMsg = std::to_string(lER.getPerfDCG(aQueryName)) + std::string(" nDCG") + lMsgEnd;
    TRACE(lTraceMsg);
    
    lER.setMAP(_irpm.meanAvgPrecision(lER.getPerfAvgPrecision()));
    lTraceMsg = std::to_string(lER.getPerfMAP()) + std::string(" MAP (mode: '") + lMode + std::string("')");
    TRACE(lTraceMsg);
}

void Evaluation::constructJSON(const str_set& aQueryNames)
{
    TRACE("Construct JSON object containing all evaluation results");
    json lModes = json::array();    
    for(const auto& [mode, results] : _evalResults)
    {
        json lMode = json::object();
        lMode["name"] = mode;
        json lQueryResults = json::array();
        for(const auto& query : aQueryNames)
        {
            try
            {
                json lQuery = json::object();
                lQuery["name"] = query;
                lQuery["perf_rnt"] = results.getPerfRuntime(query);
                lQuery["perf_acc"] = results.getPerfAccuracy(query);
                lQuery["perf_pre"] = results.getPerfPrecision(query);
                lQuery["perf_rec"] = results.getPerfRecall(query);
                lQuery["perf_fms"] = results.getPerfFMeasure(query);
                lQuery["perf_avp"] = results.getPerfAvgPrecision(query);
                lQuery["perf_dcg"] = results.getPerfDCG(query);
                lQueryResults.push_back(lQuery);
            }
            catch (const std::out_of_range& ex) 
            {
                const std::string lErrMsg = std::string("No evaluation found for query '" + query + std::string("'"));
                TRACE(lErrMsg);
            }
        }
        lMode["queries"] = lQueryResults;
        lMode["map"] = results.getPerfMAP();
        lModes.push_back(lMode);
    }
    std::time_t lCurrTime = std::time(nullptr);
    std::string lTime = std::string(std::ctime(&lCurrTime));
    std::string lPath = _evalPath;
    lPath.append(lTime.substr(0, lTime.size() - 1).append(".json"));
    std::ofstream lOutputFile(lPath.c_str(), std::ofstream::out);
    lOutputFile << lModes << std::endl;
    lOutputFile.close();
}

