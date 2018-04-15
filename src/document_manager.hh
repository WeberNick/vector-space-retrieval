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
    /* start the scan for files at the root directory and add all found docs to map */
    void read(const std::string& aFile);
    /* insert element into manager (by std::pair element) */
    bool insert(const doc_map_elem_t& aElement);
    /* insert element into manager (by values) */
    bool insert(const size_t aKey, const Document& aDocument);
    /* find element in manager */
    doc_map_iter_t find(const size_t aKey);
    /* erase by key */
    void erase(const size_t aKey);
    /* erase by iterator */
    void erase(const doc_map_iter_t aIterator);

  public:
    inline static DocumentManager& getInstance() {
        static DocumentManager lInstance;
        return lInstance;
    }
    void init(const control_block_t& aControlBlock);
    void init();

    /**
     * @brief Get the Document Map object
     *
     * @return const doc_mt&
     */
    inline doc_mt& getDocumentMap() { return _docs; }
    inline Document& getDocument(size_t aDocID) {
        if(_docs.find(aDocID) != _docs.end())
          return _docs.at(aDocID);
        else
          throw InvalidArgumentException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "This docID does not appear in the document collection.");
    }
    inline size_t getNoDocuments() { return _docs.size(); }
    inline size_t getCurrID() { return _countID; }

  private:
    static size_t _countID;

    const control_block_t* _cb;

    const std::string _collectionFile; // defined manually
    const char _delimiter;             // defined manually
    doc_mt _docs;
};
