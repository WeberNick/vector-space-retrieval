#include "evaluation.hh"

Evaluation::Evaluation() : 
    _mode(""), _queryName(""), _evalMap(), _evalPath(""), _measureInstance(nullptr), _time(0), _started(false), _cb(nullptr)
{}

void Evaluation::init(const CB& aControlBlock)
{
    if(!_cb)
    {
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
        _evalMap[_mode][_queryName] = _time;
        _mode = "";
        _queryName = "";
        _started = false;
    }
}

void Evaluation::evalIR()
{
    //todo: compare query results with gold standard
}

void Evaluation::constructJSON()
{
    json lModes = json::array();    
    for(const auto& [mode, qMap] : _evalMap)
    {
        json lMode = json::object();
        lMode["name"] = mode;
        json lQueryResults = json::array();
        for(const auto& [query, time] : qMap)
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

