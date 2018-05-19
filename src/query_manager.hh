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

/**
 * @brief Singleton Class managing and maintaining all query documents for each query types:
 *                  all, non-topic-titles, titles, video descriptions and video titles
 */
class QueryManager
{
    private:
        /**
         * @brief This nested class represents one query type and maintains the query objects, their internal
         *        interger ID and the string ID. This class is not visible to the outside and all acceses to
         *        the query types data is handled via wrapper function calls of the QueryManager
         */
        class QueryType
        {
            public:
                /* Constructors, assignment operators and destructor */
                explicit QueryType(const std::string& aType);
                explicit QueryType(const QueryType&) = delete;
                explicit QueryType(QueryType&&) = delete;
                QueryType& operator=(const QueryType&) = delete;
                QueryType& operator=(QueryType&&) = delete;
                ~QueryType() = default;

            public:
                /**
                 * @brief Creates a query document from the given content and with the given ID.
                 *        Before creating the document, the content is preprocessed.
                 * @param aStopwords    the stopwords to use for preprocessing
                 * @param aContent      the content of the query
                 * @param aQueryID      the textual query ID to assign
                 * @return the created document
                 */
                static Document createQueryDoc(const string_vt& aStopwords, std::string& aContent, const std::string aQueryID = "query-0");

                /**
                 * @brief Initializes the query type object. In this procedure the content of the query file
                 *        is read into the in-memory data structure (ID->Document Map) of the query type object
                 * @param aStopwords    the stopwords used for preprocessing
                 * @param aPath         the path to the query file from which to read the content into main memory
                 * @param aDelimiter    the delimiter to split the file content with
                 */
                void init(const string_vt& aStopwords, const std::string& aPath, const char aDelimiter);

            public:
                /**
                 * @brief Getter for the string representation of the query type
                 * @return the query type name as string
                 */
                inline const std::string&   getTypeName() const { return _qType; }
                inline const std::string&   getTypeName() { return _qType; }

                /**
                 * @brief Getter for the he document map containing all queries as documents
                 * @return Return the ID->Document map for this type where the query documents are stored in
                 */
                inline const doc_mt&        getQueryMap() const { return _qMap; }
                inline const doc_mt&        getQueryMap() { return _qMap; }

                /**
                 * @brief Getter for the internal (size_t) query IDs
                 * @return vector containing all query IDs of this query typee
                 */
                inline const sizet_vt&      getQueryIDs() const { return _qIDs; }
                inline const sizet_vt&      getQueryIDs() { return _qIDs; }

                /**
                 * @brief Getter for the string ID to size_t ID map
                 * @return returns the map containing all mappings from string ID to size_t ID
                 */
                inline const str_sizet_mt&  getStrToIDs() const { return _qStrIDs; }
                inline const str_sizet_mt&  getStrToIDs() { return _qStrIDs; }

                /**
                 * @brief Getter for the query document associated with the given internal size_t ID
                 * @return the query document representation
                 */
                inline const Document&      getQuery(size_t aID) const { return _qMap.at(aID); }
                inline const Document&      getQuery(size_t aID) { return _qMap.at(aID); }

                /**
                 * @brief Getter for the query document associated with the given string ID
                 * @return the query document representation
                 */
                inline const Document&      getQuery(const std::string& aID) const { return getQuery(_qStrIDs.at(aID)); }
                inline const Document&      getQuery(const std::string& aID) { return getQuery(_qStrIDs.at(aID)); }

                /**
                 * @brief Adds the given document to all the data structures maintained by the query type:
                 *        -Add document to ID->Document Map
                 *        -Add internal size_t ID to the vector of query IDs
                 *        -Add string ID to the mapping of string ID to internal size_t ID
                 */
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
        }; //end QueryType class scope

    private:
        /* Constructors, assignment operators and destructor */
        QueryManager();
        explicit QueryManager(const QueryManager&) = delete;
        explicit QueryManager(QueryManager&&) = delete;
        QueryManager& operator=(const QueryManager&) = delete;
        QueryManager& operator=(QueryManager&&) = delete;
        ~QueryManager() = default;

    public:
        /** 
         * @brief Initializes the single QueryManager's object with data from the control block (see types.hh)
         * @param aControlBlock the control block containing all run time specific data
         */
        void init(const CB& aControlBlock);

        /**
         * @brief This getInstance() call is the only way to access the single instance of the QueryManager's object
         * @return the single instance of the QueryManager object
         */
        static QueryManager& getInstance()
        {
            static QueryManager lInstance;
            return lInstance;
        }

        /**
         * @brief Wrapper call for the corresponding static createQueryDoc(...) function inside class QueryTypes
         * @param aStopwords    the stopwords to use for preprocessing
         * @param aContent      the content of the query
         * @param aQueryID      the textual query ID to assign
         * @return the created document
         */
        inline Document createQueryDoc(std::string& aContent){ return QueryType::createQueryDoc(_stopwords, aContent); }
    
    public:
        /**
         * @brief Getter call to get access to the query document map for this type
         * @param aQueryType enum representation of the query type
         * @return Return the ID->Document map for this type where the query documents are stored in
         */
        const doc_mt& getQueryMap(const QUERY_TYPE aQueryType) const;
        doc_mt& getQueryMap(const QUERY_TYPE aQueryType);

        /**
         * @brief Getter call to get access to the internal (size_t) query IDs for this type
         * @param aQueryType enum representation of the query type
         * @return vector containing all query IDs of this query typee
         */
        const sizet_vt& getQueryIDs(const QUERY_TYPE aQueryType) const;
        const sizet_vt& getQueryIDs(const QUERY_TYPE aQueryType);

        /**
         * @brief Getter for the query document associated with the given internal size_t ID
         * @param aQueryType enum representation of the query type
         * @param aQueryID the internal size_t ID representing the query
         * @return the query document representation
         */
        const Document& getQuery(const QUERY_TYPE aQueryType, size_t aQueryID) const;
        const Document& getQuery(const QUERY_TYPE aQueryType, size_t aQueryID);

        /**
         * @brief Getter for the query document associated with the given string ID
         * @param aQueryType enum representation of the query type
         * @param aQueryID the string ID representing the query
         * @return the query document representation
         */
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
