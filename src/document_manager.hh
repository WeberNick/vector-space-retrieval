/**
 *	@file 	document_manager.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  The document manager handles everything regarding the document collection. It parses the
 *          document collection and creates a document map
 *	@bugs 	Currently no bugs known
 *	@todos	Write DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include "document.hh"
#include "exception.hh"
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
    void read(const std::string& aFile);
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
    inline doc_mt& getDocumentMap() { return _docs; }
    /**
     * @brief Get the number of documents in the collection
     *
     * @return size_t number of docs in the collection
     */
    inline size_t getNoDocuments() { return _docs.size(); }

    /**
     * @brief Get the document object with id aDocID
     *
     * @param aDocID the id of the document
     * @return Document& the document with id aDocID
     */
    Document& getDocument(size_t aDocID);

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
    void init(const control_block_t& aControlBlock, const std::string& aCollectionFile);

  private:
    const control_block_t* _cb;

    bool _init;
    std::string _collectionFile;
    const char _delimiter; // defined manually
    doc_mt _docs;
};
