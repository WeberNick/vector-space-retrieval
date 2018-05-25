/**
 *	@file 	document_manager.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  The document manager handles everything regarding the document collection. It parses the
 *          document collection and creates a document map
 *	@bugs 	Currently no bugs known
 *
 *	@section DESCRIPTION docto_
 */
#pragma once

#include "document.hh"
#include "exception.hh"
#include "file_util.hh"
#include "ir_util.hh"
#include "string_util.hh"
#include "trace.hh"
#include "types.hh"

#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class DocumentManager {
  private:
    DocumentManager();
    DocumentManager(const DocumentManager&) = delete;
    DocumentManager(DocumentManager&&) = delete;
    DocumentManager& operator=(const DocumentManager&) = delete;
    DocumentManager& operator=(DocumentManager&&) = delete;
    ~DocumentManager() = default;

  public:
    /**
     * @brief Get the document map
     *
     * @return doc_mt& the document map
     */
    inline const doc_mt& getDocumentMap() const { return _docs; }
    inline doc_mt& getDocumentMap() { return const_cast<doc_mt&>(static_cast<const DocumentManager&>(*this).getDocumentMap()); }

    /**
     * @brief Get the number of documents in the collection
     *
     * @return size_t number of docs in the collection
     */
    inline size_t getNoDocuments() const { return _docs.size(); }
    inline size_t getNoDocuments() { return static_cast<const DocumentManager&>(*this).getNoDocuments(); }

    /**
     * @brief Get the ids of all documents in the collection as a size_t vector
     *
     * @return sizet_vt& the ids
     */
    inline const sizet_vt& getIDs() const { return _docids; }
    inline sizet_vt& getIDs() { return const_cast<sizet_vt&>(static_cast<const DocumentManager&>(*this).getIDs()); }

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
            const std::string lErrMsg = std::string("The doc ID '" + std::to_string(aDocID) + std::string("' does not appear in the document collection"));
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
    inline Document& getDocument(size_t aDocID) {
        return const_cast<Document&>(static_cast<const DocumentManager&>(*this).getDocument(aDocID));
    }
    /**
     * @brief Get the Document object
     *
     * @param aDocID
     * @return const Document&
     */
    inline const Document& getDocument(const std::string& aDocID) const {
        try {
            return getDocument(_str_docid.at(aDocID));
        } catch (const std::out_of_range& ex) {
            const std::string lErrMsg = std::string("The doc ID ')" + aDocID + std::string("' does not appear in the document collection"));
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
    inline Document& getDocument(const std::string& aDocID) {
        return const_cast<Document&>(static_cast<const DocumentManager&>(*this).getDocument(aDocID));
    }

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
     * @brief Initialize control block and document manager
     *
     * @param aControlBlock the control block
     */
    void init(const CB& aControlBlock);

  private:
    inline void addDoc(const Document& aDoc) {
        _docs.try_emplace(aDoc.getID(), aDoc);
        _docids.push_back(aDoc.getID());
        _str_docid.insert(std::make_pair(aDoc.getDocID(), aDoc.getID()));
    }

  private:
    const control_block_t* _cb;
    const char _delimiter; // defined manually

    doc_mt _docs;
    sizet_vt _docids;
    str_sizet_mt _str_docid;
};
