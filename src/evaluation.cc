#include "evaluation.hh"
#include "utility.hh"

Evaluation::IR_PerformanceManager::RelScore::RelScore(const std::string& aQueryID, const std::string& aDocID, const uint aScore) :
    _qID(aQueryID), _dID(aDocID), _score(aScore)
{}

Evaluation::IR_PerformanceManager::IR_PerformanceManager() : 
    _queryScores(), _cb(nullptr)
{}
                
void Evaluation::IR_PerformanceManager::init(const CB& aControlBlock)
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

uint Evaluation::IR_PerformanceManager::getScore(const std::string& aQueryID, const std::string& aDocID)
{
    const scores_vt& lRelScores = getQueryScores(aQueryID);
    for(const RelScore& relScore : lRelScores)
    {
        if(relScore.getDocumentID() == aDocID){ return relScore.getScore(); } 
    }
    return 0;
}

double Evaluation::IR_PerformanceManager::bDCG(const std::string& aQueryID, const pair_sizet_float_vt& aRanking)
{    
    double lSum = 0;
    for(size_t i = 0; i < aRanking.size(); ++i)
    {
        const uint lScore = getScore(aQueryID, DocumentManager::getInstance().getDocument(aRanking[i].first).getDocID());
        lSum += (lScore / std::log2(i + 1));
    }
    return lSum;
}

double Evaluation::IR_PerformanceManager::rDCG(const std::string& aQueryID, const pair_sizet_float_vt& aRanking)
{
    double lSum = 0;
    for(size_t i = 0; i < aRanking.size(); ++i)
    {
        const uint lScore = getScore(aQueryID, DocumentManager::getInstance().getDocument(aRanking[i].first).getDocID());
        const double lNumerator = std::pow(2, lScore) - 1;
        lSum += (lNumerator / std::log2(i + 1));
    }
    return lSum;
}

double Evaluation::IR_PerformanceManager::iDCG(const std::string& aQueryID)
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

double Evaluation::IR_PerformanceManager::nDCG(const std::string& aQueryID, const pair_sizet_float_vt& aRanking, const bool aBDCG = true)
{
    double lDCG = (aBDCG) ? bDCG(aQueryID, aRanking) : rDCG(aQueryID, aRanking);
    return (lDCG / iDCG(aQueryID));
}

Evaluation::EvalResults::EvalResults() :
    _mode(""), _perfRT(), _perfIR()
{}

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

void Evaluation::evalIR(const IR_MODE aMode, const std::string& aQueryName, const std::vector<std::pair<size_t, float>>& aRanking)
{
    //todo: compare query results with gold standard
   
}

void Evaluation::constructJSON()
{
    json lModes = json::array();    
    for(const auto& [mode, results] : _evalResults)
    {
        json lMode = json::object();
        lMode["name"] = mode;
        json lQueryResults = json::array();
        const auto& lPerfRT = results.getPerfRT(); 
        const auto& lPerfIR = results.getPerfIR(); 
        for(const auto& [query, time] : lPerfRT)
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

