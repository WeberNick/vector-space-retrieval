#pragma once

#include "document.hh"
#include "exception.hh"
#include "index_manager.hh"
#include "types.hh"
#include "utility.hh"

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

  public:
    static DocumentManager& getInstance();

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
    /**
     * @brief Get the Document Map object
     *
     * @return const doc_mt&
     */
    inline const doc_mt& getDocumentMap() { return _docs; }
    inline size_t getNoDocuments() { return _docs.size(); }
    inline size_t getCurrID() { return _countID; }

  private:
    static size_t _countID;

    const std::string _collectionFile; // defined manually
    const char _delimiter;             // defined manually
    doc_mt _docs;
};
