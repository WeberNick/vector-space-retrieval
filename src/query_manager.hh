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

#include <unordered_map>
#include <array>

class QueryManager
{
    private:
        class QueryType
        {
            doc_mt          _qMap;
            sizet_vt        _qIDs;
            str_sizet_mt    _qStrID;

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
    
    public:
        //getter
        inline const doc_mt& getQueriesAll() const { return _qAll; }
        inline doc_mt& getQueriesAll() { return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueriesAll()); }
        inline const doc_mt& getQueriesNTT() const { return _qNTT; }
        inline doc_mt& getQueriesNTT() { return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueriesNTT()); }
        inline const doc_mt& getQueriesTitles() const { return _qTitles; }
        inline doc_mt& getQueriesTitles() { return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueriesTitles()); }
        inline const doc_mt& getQueriesVidDesc() const { return _qVidDesc; }
        inline doc_mt& getQueriesVidDesc() { return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueriesVidDesc()); }
        inline const doc_mt& getQueriesVidTitles() const { return _qVidTitles; }
        inline doc_mt& getQueriesVidTitles() { return const_cast<doc_mt&>(static_cast<const QueryManager&>(*this).getQueriesVidTitles()); }
        
    private:
        const CB*   _cb;
        const char  _delimiter;
        const std::array<std::string, 5> _qTypes;
        std::string _queryPath;
        doc_mt      _qAll;      //q-all
        doc_mt      _qNTT;      //q-non topic titles
        doc_mt      _qTitles;   //q-titles
        doc_mt      _qVidDesc;  //q-vid-desc
        doc_mt      _qVidTitles;//q-vid-titles


};

