#include "query_manager.hh"
#include "measure.hh"

QueryManager::QueryType::QueryType(const std::string& aType) :
    _qType(aType),
    _qMap()
{}
                
Document QueryManager::QueryType::createQueryDoc(const str_set& aStopwords, std::string& aContent, const bool aPreprocess, const std::string aQueryID)
{
    string_vt content;

    if (aPreprocess) {
        content = Util::preprocess(aContent, aStopwords);
    } else {
        Util::splitStringBoost(aContent, ' ', content);
    }

    Document lQueryDoc(aQueryID, content);

    const string_vt& con = lQueryDoc.getContent(); // start build docTermTFMap
    str_int_mt tf_counts;
    str_float_mt tf_out;

    for (const std::string& term : con) {
        ++tf_counts[term];
    }

    int maxFreq = Util::getMaxWordFrequency(con);
    for (const auto& [term, count] : tf_counts) { // this loops through the distinct terms of this document
        tf_out[term] = Util::calcTf(count, maxFreq);
    }
    lQueryDoc.setTermTfMap(tf_out); // end build docTermTFMap

    IndexManager::getInstance().buildTfIdfVector(lQueryDoc);
    IndexManager::getInstance().buildWordEmbeddingsVector(lQueryDoc);
    IndexManager::getInstance().buildRandProjVector(lQueryDoc);

    return lQueryDoc;
}

void QueryManager::QueryType::init(const std::string& aPath, const char aDelimiter)
{
    TRACE(std::string("QueryManager: Start reading the query collection and creating Document objects for query type '") + _qType  + std::string("'"));
    const std::string lFilePath = aPath + std::string("q-") + _qType + std::string(".queries");
    string_vvt lFileContent;
    Util::readIn(lFilePath, aDelimiter, lFileContent);
    //std::cout << "Build Vecs for QueryType: " << lFilePath << std::endl;
    for(auto& line : lFileContent)
    {
        const std::string& lQueryID = line.at(0);
        std::string& lQueryContent = line.at(1);
        addDoc(lQueryID, lQueryContent);
    }
    TRACE("QueryManager: Finished reading the query collection");
}

QueryManager::QueryManager() :
    _cb(nullptr), 
    _delimiter('~'), 
    _stopwords(),
    _qAll("all"), 
    _qNTT("nontopictitles"), 
    _qTitles("titles"), 
    _qVidDesc("viddesc"), 
    _qVidTitles("vidtitles")
{}

void QueryManager::init(const CB& aControlBlock)
{
    if(!_cb)
    {
        _cb = &aControlBlock;
        string_vvt lStopwordFile;
        Util::readIn(_cb->stopwordPath(), ',', lStopwordFile); //read in stopwords
        try
        {
            const string_vt& lStopwords = lStopwordFile.at(0);
            _stopwords = str_set(lStopwords.cbegin(), lStopwords.cend());
        }
        catch (const std::out_of_range& ex) 
        {
            const std::string lErrMsg = std::string("Read in of stopwords failed, cannot recover: ") + std::string(ex.what());
            TRACE(lErrMsg);
            throw;
        }

        const std::string& lQueryPath = _cb->queryPath();
        _qAll.init(lQueryPath, _delimiter);
        _qNTT.init(lQueryPath, _delimiter);
        _qTitles.init(lQueryPath, _delimiter);
        _qVidDesc.init(lQueryPath, _delimiter);
        _qVidTitles.init(lQueryPath, _delimiter);
    }
}


const std::string& QueryManager::getTypeName(const QUERY_TYPE aQueryType) const
{
    switch(aQueryType)
    {
        case kALL:
            return _qAll.getTypeName();
            break;
        case kNTT:
            return _qNTT.getTypeName();
            break;
        case kTITLES:
            return _qTitles.getTypeName();
            break;
        case kVIDDESC:
            return _qVidDesc.getTypeName();
            break;
        case kVIDTITLES:
            return _qVidTitles.getTypeName();
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}

const std::string& QueryManager::getTypeName(const QUERY_TYPE aQueryType)
{
    switch(aQueryType)
    {
        case kALL:
            return static_cast<const QueryManager&>(*this).getTypeName(aQueryType);
            break;
        case kNTT:
            return static_cast<const QueryManager&>(*this).getTypeName(aQueryType);
            break;
        case kTITLES:
            return static_cast<const QueryManager&>(*this).getTypeName(aQueryType);
            break;
        case kVIDDESC:
            return static_cast<const QueryManager&>(*this).getTypeName(aQueryType);
            break;
        case kVIDTITLES:
            return static_cast<const QueryManager&>(*this).getTypeName(aQueryType);
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}

const str_str_mt& QueryManager::getQueryMap(const QUERY_TYPE aQueryType) const 
{
    switch(aQueryType)
    {
        case kALL:
            return _qAll.getQueryMap();
            break;
        case kNTT:
            return _qNTT.getQueryMap();
            break;
        case kTITLES:
            return _qTitles.getQueryMap();
            break;
        case kVIDDESC:
            return _qVidDesc.getQueryMap();
            break;
        case kVIDTITLES:
            return _qVidTitles.getQueryMap();
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}

str_str_mt& QueryManager::getQueryMap(const QUERY_TYPE aQueryType)
{
    switch(aQueryType)
    {
        case kALL:
            return const_cast<str_str_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        case kNTT:
            return const_cast<str_str_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        case kTITLES:
            return const_cast<str_str_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        case kVIDDESC:
            return const_cast<str_str_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        case kVIDTITLES:
            return const_cast<str_str_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}

const string_vt& QueryManager::getQueryIDs(const QUERY_TYPE aQueryType) const
{
    switch(aQueryType)
    {
        case kALL:
            return _qAll.getQueryIDs();
            break;
        case kNTT:
            return _qNTT.getQueryIDs();
            break;
        case kTITLES:
            return _qTitles.getQueryIDs();
            break;
        case kVIDDESC:
            return _qVidDesc.getQueryIDs();
            break;
        case kVIDTITLES:
            return _qVidTitles.getQueryIDs();
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}

const string_vt& QueryManager::getQueryIDs(const QUERY_TYPE aQueryType)
{
    switch(aQueryType)
    {
        case kALL:
            return static_cast<const QueryManager&>(*this).getQueryIDs(aQueryType);
            break;
        case kNTT:
            return static_cast<const QueryManager&>(*this).getQueryIDs(aQueryType);
            break;
        case kTITLES:
            return static_cast<const QueryManager&>(*this).getQueryIDs(aQueryType);
            break;
        case kVIDDESC:
            return static_cast<const QueryManager&>(*this).getQueryIDs(aQueryType);
            break;
        case kVIDTITLES:
            return static_cast<const QueryManager&>(*this).getQueryIDs(aQueryType);
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}

const std::string& QueryManager::getQueryContent(const QUERY_TYPE aQueryType, const std::string& aQueryID) const
{
    try
    {
        switch(aQueryType)
        {
            case kALL:
                return _qAll.getQuery(aQueryID);
                break;
            case kNTT:
                return _qNTT.getQuery(aQueryID);
                break;
            case kTITLES:
                return _qTitles.getQuery(aQueryID);
                break;
            case kVIDDESC:
                return _qVidDesc.getQuery(aQueryID);
                break;
            case kVIDTITLES:
                return _qVidTitles.getQuery(aQueryID);
                break;
            default: 
                throw SwitchException(FLF); 
                break;
        }
    }
    catch (const std::out_of_range& ex) 
    {
        const std::string lErrMsg = std::string("No query found for the ID '") + aQueryID + std::string("'");
        TRACE(lErrMsg);
        throw InvalidArgumentException(FLF, lErrMsg);
    }
}

const std::string& QueryManager::getQueryContent(const QUERY_TYPE aQueryType, const std::string& aQueryID)
{
    switch(aQueryType)
    {
        case kALL:
            return static_cast<const QueryManager&>(*this).getQueryContent(aQueryType, aQueryID);
            break;
        case kNTT:
            return static_cast<const QueryManager&>(*this).getQueryContent(aQueryType, aQueryID);
            break;
        case kTITLES:
            return static_cast<const QueryManager&>(*this).getQueryContent(aQueryType, aQueryID);
            break;
        case kVIDDESC:
            return static_cast<const QueryManager&>(*this).getQueryContent(aQueryType, aQueryID);
            break;
        case kVIDTITLES:
            return static_cast<const QueryManager&>(*this).getQueryContent(aQueryType, aQueryID);
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}
