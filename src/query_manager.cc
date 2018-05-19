#include "query_manager.hh"

QueryManager::QueryType::QueryType(const std::string& aType) :
    _qType(aType),
    _qMap(),
    _qIDs(),
    _qStrIDs()
{}
                
Document QueryManager::QueryType::createQueryDoc(const string_vt& aStopwords, std::string& aContent, const std::string aQueryID)
{
    Document lQueryDoc(aQueryID, Util::preprocess(aContent, aStopwords));

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

void QueryManager::QueryType::init(const string_vt& aStopwords, const std::string& aPath, const char aDelimiter)
{
    TRACE(std::string("QueryManager: Start reading the query collection and creating Document objects for query type '") + _qType  + std::string("'"));
    const std::string lFilePath = aPath + std::string("q-") + _qType + std::string(".queries");
    string_vvt lFileContent;
    Util::readIn(lFilePath, aDelimiter, lFileContent);
    for(auto& line : lFileContent)
    {
        const std::string& lQueryID = line.at(0);
        std::string& lQueryContent = line.at(1);
        addDoc(QueryType::createQueryDoc(aStopwords, lQueryContent, lQueryID));
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
        string_vvt lStopwords;
        Util::readIn(_cb->stopwordPath(), ',', lStopwords); //read in stopwords
        try
        {
            _stopwords = lStopwords.at(0);
        }
        catch (const std::out_of_range& ex) 
        {
            const std::string lErrMsg = std::string("Read in of stopwords failed, cannot recover: ") + std::string(ex.what());
            TRACE(lErrMsg);
            throw;
        }

        const std::string& lQueryPath = _cb->queryPath();
        //_qAll.init(_stopwords, lQueryPath, _delimiter);
        //_qNTT.init(_stopwords, lQueryPath, _delimiter);
        _qTitles.init(_stopwords, lQueryPath, _delimiter);
        //_qVidDesc.init(_stopwords, lQueryPath, _delimiter);
        //_qVidTitles.init(_stopwords, lQueryPath, _delimiter);
    }
}
 
const doc_mt& QueryManager::getQueryMap(const QUERY_TYPE aQueryType) const 
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

doc_mt& QueryManager::getQueryMap(const QUERY_TYPE aQueryType)
{
    switch(aQueryType)
    {
        case kALL:
            return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        case kNTT:
            return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        case kTITLES:
            return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        case kVIDDESC:
            return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        case kVIDTITLES:
            return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueryMap(aQueryType));
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}

const sizet_vt& QueryManager::getQueryIDs(const QUERY_TYPE aQueryType) const
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

const sizet_vt& QueryManager::getQueryIDs(const QUERY_TYPE aQueryType)
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

const Document& QueryManager::getQuery(const QUERY_TYPE aQueryType, size_t aQueryID) const
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
        const std::string lErrMsg = std::string("No query found for the ID '") + std::to_string(aQueryID) + std::string("'");
        TRACE(lErrMsg);
        throw InvalidArgumentException(FLF, lErrMsg);
    }
}

const Document& QueryManager::getQuery(const QUERY_TYPE aQueryType, size_t aQueryID)
{
    switch(aQueryType)
    {
        case kALL:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        case kNTT:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        case kTITLES:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        case kVIDDESC:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        case kVIDTITLES:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}

const Document& QueryManager::getQuery(const QUERY_TYPE aQueryType, const std::string& aQueryID) const
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

const Document& QueryManager::getQuery(const QUERY_TYPE aQueryType, const std::string& aQueryID)
{
    switch(aQueryType)
    {
        case kALL:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        case kNTT:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        case kTITLES:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        case kVIDDESC:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        case kVIDTITLES:
            return static_cast<const QueryManager&>(*this).getQuery(aQueryType, aQueryID);
            break;
        default: 
            throw SwitchException(FLF); 
            break;
    }
}
