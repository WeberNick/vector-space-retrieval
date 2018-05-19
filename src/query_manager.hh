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
        const sizet_vt& getQueryIDs(const QUERY_TYPE aQueryType);


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



    ////TODO docs
    /**
     * @brief Get the Queries object
     * 
     * @param aDocID 
     * @return doc_mt& 
     */
    //inline doc_mt& getQueriesForType(const std::string& aQueryType) {
        //try {
            //return _queries.at(aQueryType);
        //} catch (const std::out_of_range& ex) {
            //const std::string lErrMsg = std::string("The query type ')" + aQueryType + std::string("' does not exist"));
            //TRACE(lErrMsg);
            //throw InvalidArgumentException(FLF, lErrMsg);
        //}
    //}

    /**
     * @brief Creates a preprocessed document query out of a string
     *
     * @param query The raw query string
     * @return Document
     */
    //Document createQuery(std::string& query, const std::string& queryID = "query-0");

    /**
     * @brief Get the document map
     *
     * @return doc_mt& the document map
     */
    //inline const std::unordered_map<std::string, doc_mt>& getQueryMap() const { return _queries; }
    ////TODO docs
    /**
     * @brief Get the Document Map object
     * 
     * @return doc_mt& 
     */
    //inline std::unordered_map<std::string, doc_mt>& getQueryMap() { return const_cast<std::unordered_map<std::string, doc_mt>&>(static_cast<const DocumentManager&>(*this).getQueryMap()); }
    



