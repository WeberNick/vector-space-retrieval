/*********************************************************************
 * @file    query_manager.hh
 * @author 	Nick Weber
 * @date    Mai 19, 2018
 * @brief 	Class maintaining all query types and providing an interface
 *          to get access to all query objects
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
#include "exception.hh"
#include "trace.hh"
#include "file_util.hh"
#include "document.hh"
#include "index_manager.hh"

#include <unordered_map>
#include <utility>
#include <array>
#include <string>

class QueryManager
{
    private:
        class QueryType
        {
            public:
                explicit QueryType(const std::string& aType);
                explicit QueryType(const QueryType&) = delete;
                explicit QueryType(QueryType&&) = delete;
                QueryType& operator=(const QueryType&) = delete;
                QueryType& operator=(QueryType&&) = delete;
                ~QueryType() = default;
                static Document createQueryDoc(const string_vt& aStopwords, std::string& aContent, const std::string aQueryID = "query-0");
                void init(const string_vt& aStopwords, const std::string& aPath, const char aDelimiter);

            public:
                //const getter
                inline const std::string&   getTypeName() const { return _qType; }
                inline const doc_mt&        getQueryMap() const { return _qMap; }
                inline const sizet_vt&      getQueryIDs() const { return _qIDs; }
                inline const str_sizet_mt&  getStrToIDs() const { return _qStrIDs; }
                inline const Document&      getQuery(size_t aID) const { return _qMap.at(aID); }
                inline const Document&      getQuery(const std::string& aID) const { return getQuery(_qStrIDs.at(aID)); }
                //getter
                inline const std::string&   getTypeName() { return _qType; }
                inline const doc_mt&        getQueryMap() { return _qMap; }
                inline const sizet_vt&      getQueryIDs() { return _qIDs; }
                inline const str_sizet_mt&  getStrToIDs() { return _qStrIDs; }
                inline const Document&      getQuery(size_t aID) { return _qMap.at(aID); }
                inline const Document&      getQuery(const std::string& aID) { return getQuery(_qStrIDs.at(aID)); }
                inline void addDoc(const Document& aDoc)
                { 
                    _qMap.try_emplace(aDoc.getID(), aDoc); 
                    _qIDs.push_back(aDoc.getID());
                    _qStrIDs.insert(std::make_pair(aDoc.getDocID(), aDoc.getID()));
                }

            private:
                const std::string   _qType;
                doc_mt              _qMap;
                sizet_vt            _qIDs;
                str_sizet_mt        _qStrIDs;
        };

    private:
        QueryManager();
        explicit QueryManager(const QueryManager&) = delete;
        explicit QueryManager(QueryManager&&) = delete;
        QueryManager& operator=(const QueryManager&) = delete;
        QueryManager& operator=(QueryManager&&) = delete;
        ~QueryManager() = default;

    public:
        void init(const CB& aControlBlock);
        static QueryManager& getInstance()
        {
            static QueryManager lInstance;
            return lInstance;
        }
        inline Document createQueryDoc(std::string& aContent){ return QueryType::createQueryDoc(_stopwords, aContent); }
    
    public:
        //getter
        const doc_mt& getQueryMap(const QUERY_TYPE aQueryType) const;
        doc_mt& getQueryMap(const QUERY_TYPE aQueryType);
        const sizet_vt& getQueryIDs(const QUERY_TYPE aQueryType) const;
        const sizet_vt& getQueryIDs(const QUERY_TYPE aQueryType);
        const Document& getQuery(const QUERY_TYPE aQueryType, size_t aQueryID) const;
        const Document& getQuery(const QUERY_TYPE aQueryType, size_t aQueryID);
        const Document& getQuery(const QUERY_TYPE aQueryType, const std::string& aQueryID) const;
        const Document& getQuery(const QUERY_TYPE aQueryType, const std::string& aQueryID);



    private:
        const CB*   _cb;
        const char  _delimiter;
        string_vt   _stopwords;
        QueryType   _qAll;      //q-all
        QueryType   _qNTT;      //q-non topic titles
        QueryType   _qTitles;   //q-titles
        QueryType   _qVidDesc;  //q-vid-desc
        QueryType   _qVidTitles;//q-vid-titles
};


