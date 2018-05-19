#include "query_manager.hh"

QueryManager::QueryManager() :
    _cb(nullptr), 
    _delimiter('~'), 
    _qTypes({"all", "nontopictitles", "titles", "viddesc", "vidtitles"}), 
    _queryPath(""),
    _qAll(), _qNTT(), _qTitles(), _qVidDesc(), _qVidTitles()
{}

void QueryManager::init(const CB& aControlBlock)
{
    if(!_cb)
    {
        _cb = &aControlBlock;
        _queryPath = _cb->queryPath();
    }
}
