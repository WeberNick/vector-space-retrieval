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
        std::ifstream file(_cb->relevanceScoresPath());
        std::string line;
        while (std::getline(file, line)) {
            string_vt parts;
            Utility::StringOp::splitStringBoost(line, '~', parts);
            const std::string qID = parts[0];
            const std::string dID = parts[1];
            const uint score = std::stoul(parts[2]);
            _queryScores[qID].emplace_back(qID, dID, score);
        }
    }
}

void IRPM::tp_tn_fp_fn(const std::string& aQueryID, const sizet_vt& aRanking, size_t& aTP, size_t& aTN, size_t& aFP, size_t& aFN)
{
    const DocumentManager& lDocMngr = DocumentManager::getInstance();

    sizet_vt lDocIDs = lDocMngr.getIDs(); //ids of the doc collection
    sizet_vt lRetrieved = aRanking; //ids of the retrieved docs
    sizet_vt lNotRetrieved = Utility::VecUtil::difference(lDocIDs, lRetrieved); //ids of the not retrieved docs

    const auto& lQueryScores = getQueryScores(aQueryID); //rel score objects for the query

    sizet_vt lRelevant; //ids of the relevant docs
    lRelevant.reserve(lQueryScores.size()); //reserve enough memory
    //search for all doc IDs relevant
    for(const auto& relScore : lQueryScores)
    {
        lRelevant.push_back(lDocMngr.getDocument(relScore.getDocumentID()).getID());    
    }
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
    const auto& lQueryScores = getQueryScores(aQueryID); //rel score objects for the query
    sizet_vt lRelevant; //ids of the relevant docs
    lRelevant.reserve(lQueryScores.size()); //reserve enough memory
    //search for all doc IDs relevant
    for(const auto& relScore : lQueryScores)
    {
        lRelevant.push_back(DocumentManager::getInstance().getDocument(relScore.getDocumentID()).getID());    
    }
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
    return lSum / 1; //todo
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
        _evalResults.at(_mode).setTime(_queryName, _time);
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
    EvalResults& lER = _evalResults.at(modeToString(aMode));
    lER.setAccuracy(aQueryName, _irpm.accuracy(aQueryName, lRanking));
    lER.setPrecision(aQueryName, _irpm.precision(aQueryName, lRanking));
    lER.setRecall(aQueryName, _irpm.recall(aQueryName, lRanking));
    lER.setFMeasure(aQueryName, _irpm.fMeasure(aQueryName, lRanking));
    lER.setAvgPrecision(aQueryName, _irpm.avgPrecision(aQueryName, lRanking));
    lER.setDCG(aQueryName, _irpm.nDCG(aQueryName, lRanking));
    lER.setMAP(_irpm.meanAvgPrecision(lER.getPerfAvgPrecision()));
}

void Evaluation::constructJSON()
{
    json lModes = json::array();    
    for(const auto& [mode, results] : _evalResults)
    {
        json lMode = json::object();
        lMode["name"] = mode;
        json lQueryResults = json::array();
        const auto& lPerfRuntime = results.getPerfRuntime(); 
        const auto& lPerfAccuracy = results.getPerfAccuracy(); 
        const auto& lPerfPrecision = results.getPerfPrecision(); 
        const auto& lPerfRecall = results.getPerfRecall(); 
        const auto& lPerfFMeasure = results.getPerfFMeasure(); 
        const auto& lPerfAvgPrecision = results.getPerfAvgPrecision(); 
        const auto& lPerfDCG = results.getPerfDCG(); 
        for(const auto& [query, time] : lPerfRuntime)
        {
            json lQuery = json::object();
            lQuery["name"] = query;
            lQuery["time"] = time;
            lQueryResults.push_back(lQuery);
        }
        lMode["queries"] = lQueryResults;
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

