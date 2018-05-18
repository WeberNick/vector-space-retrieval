/**
 *	@file 	document_manager.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  The document manager handles everything regarding the document collection. It parses the
 *          document collection and creates a document map
 *	@bugs 	Currently no bugs known
 *	@todos	Write DESCRIPTION, implement a datastructure with mapping from doc string id to doc size_t id
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include "document.hh"
#include "exception.hh"
#include "trace.hh"
#include "types.hh"

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class DocumentManager {
  private:
    explicit DocumentManager();
    DocumentManager(const DocumentManager&) = delete;
    DocumentManager(DocumentManager&&) = delete;
    DocumentManager& operator=(const DocumentManager&) = delete;
    DocumentManager& operator=(DocumentManager&&) = delete;
    ~DocumentManager();

  private:
    /**
     * @brief Start the scan for files at the root directory and add all found docs to map
     *
     * @param aFile the collection file to read
     */
    void readDocs(const std::string& aFile);
    //TODO docs
    /**
     * @brief 
     * 
     * @param aFile 
     */
    void readQueries(const string_vt& aQueryTypes);
    /**
     * @brief Insert element into manager (by std::pair element)
     *
     * @param aElement element to be inserted
     * @return true if insertion was successful
     * @return false if insertion failed
     */
    bool insert(const doc_map_elem_t& aElement);
    /**
     * @brief Insert element into manager (by values)
     *
     * @param aKey the key of the map entry
     * @param aDocument the value of the map entry
     * @return true if insertion was successful
     * @return false if insertion failed
     */
    bool insert(const size_t aKey, const Document& aDocument);
    /**
     * @brief Find a document and return an iterator
     *
     * @param aKey the key to find
     * @return doc_map_iter_t the iterator of the found document with key aKey
     */
    doc_map_iter_t find(const size_t aKey);
    /**
     * @brief Erase document by key
     *
     * @param aKey the key to erase
     */
    void erase(const size_t aKey);
    /**
     * @brief Erase document by iterator
     *
     * @param aIterator the iterator for erasing the document
     */
    void erase(const doc_map_iter_t aIterator);

  public:
    /**
     * @brief Get the document map
     *
     * @return doc_mt& the document map
     */
    inline const doc_mt& getDocumentMap() const { return _docs; }
    //TODO docs
    /**
     * @brief Get the Document Map object
     * 
     * @return doc_mt& 
     */
    inline doc_mt& getDocumentMap() { return const_cast<doc_mt&>(static_cast<const DocumentManager&>(*this).getDocumentMap()); }
    /**
     * @brief Get the number of documents in the collection
     *
     * @return size_t number of docs in the collection
     */
    inline size_t getNoDocuments() const { return _docs.size(); }
    //TODO docs
    /**
     * @brief Get the No Documents object
     * 
     * @return size_t 
     */
    inline size_t getNoDocuments() { return static_cast<const DocumentManager&>(*this).getNoDocuments(); }
    /**
     * @brief Get the ids of all documents in the collection as a size_t vector
     *
     * @return sizet_vt& the ids
     */
    inline const sizet_vt& getIDs() const { return _docids; }
    //TODO docs
    /**
     * @brief 
     * 
     * @return sizet_vt& 
     */
    inline sizet_vt& getIDs() { return const_cast<sizet_vt &>(static_cast<const DocumentManager&>(*this).getIDs()); }

    /**
     * @brief Creates a preprocessed document query out of a string
     *
     * @param query The raw query string
     * @return Document
     */
    Document createQuery(std::string& query, const std::string& queryID = "query-0");

    /**
     * @brief Get the document object with id aDocID
     *
     * @param aDocID the id of the document
     * @return Document& the document with id aDocID
     */
    inline const Document& getDocument(size_t aDocID) const {
        try {
            return _docs.at(aDocID);
        } catch (const std::out_of_range& ex) {
            const std::string lErrMsg("This docID does not appear in the document collection.");
            TRACE(lErrMsg);
            throw InvalidArgumentException(FLF, lErrMsg);
        }
    }
    /**
     * @brief Get the Document object
     * 
     * @param aDocID 
     * @return Document& 
     */
    inline Document& getDocument(size_t aDocID) { return const_cast<Document&>(static_cast<const DocumentManager&>(*this).getDocument(aDocID)); }
    /**
     * @brief Get the Document object
     * 
     * @param aDocID 
     * @return const Document& 
     */
    inline const Document& getDocument(const std::string& aDocID) const { return getDocument(_str_docid.at(aDocID)); } 
    /**
     * @brief Get the Document object
     * 
     * @param aDocID 
     * @return Document& 
     */
    inline Document& getDocument(const std::string& aDocID) { return const_cast<Document&>(static_cast<const DocumentManager&>(*this).getDocument(aDocID)); }

    //TODO docs
    /**
     * @brief Get the Queries object
     * 
     * @param aDocID 
     * @return doc_mt& 
     */
    inline doc_mt& getQueriesForType(const std::string& aQueryType) {
        try {
            return _queries.at(aQueryType);
        } catch (const std::out_of_range& ex) {
            const std::string lErrMsg("This query type does not exist.");
            TRACE(lErrMsg);
            throw InvalidArgumentException(FLF, lErrMsg);
        }
    }
    //TODO docs
    /**
     * @brief Get the Query Types object
     * 
     * @return string_vt& 
     */
    inline string_vt& getQueryTypes() { return _queryTypes; }

    /**
     * @brief Get the Instance object
     *
     * @return DocumentManager&
     */
    inline static DocumentManager& getInstance() {
        static DocumentManager lInstance;
        return lInstance;
    }
    /**
     * @brief
     *
     * @param aControlBlock
     */
    void init(const control_block_t& aControlBlock);

  private:
    const control_block_t*                    _cb;
    bool                                      _init;
    const char                                _delimiter; // defined manually

    std::string                               _collectionFile;
    string_vt                                 _queryTypes;
    
    sizet_vt                                  _docids;
    doc_mt                                    _docs;
    str_sizet_mt                              _str_docid;

    std::unordered_map<std::string, sizet_vt> _queryids;
    std::unordered_map<std::string, doc_mt>   _queries;
};
