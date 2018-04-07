#pragma once

#include "document.hh"
#include "inverted_index.hh"
#include "tiered_index.hh"
#include "types.hh"

#include <map>
#include <string>

class DocumentManager {
  public:
    typedef std::map<size_t, Document> DocumentMap;
    typedef std::pair<size_t, Document> DocumentMapElem;
    typedef DocumentMap::iterator DocMapIterator;
    // typedef std::pair<DocMapIterator, bool> ...;

  public:
    explicit DocumentManager(const std::string& aFile);
    explicit DocumentManager() = delete;
    DocumentManager(const DocumentManager&) = delete;
    DocumentManager(DocumentManager&&) = delete;
    DocumentManager& operator=(const DocumentManager&) = delete;
    DocumentManager& operator=(DocumentManager&&) = delete;
    ~DocumentManager();

  private:
    /* start the scan for files at the root directory and add all found docs to map */
    void read(const std::string& aFile);

  public:
    /* The following are just wrapper functions for the respective map container calls */

    /* insert element into manager (by std::pair element) */
    bool insert(const DocumentMapElem& aElement);
    /* insert element into manager (by values) */
    bool insert(const size_t aKey, const Document& aDocument);
    /* find element in manager */
    DocMapIterator find(const size_t aKey);
    /* erase by key */
    bool erase(const size_t aKey);
    /* erase by iterator */
    bool erase(const DocMapIterator aIterator);

  public:
    // getter
    inline const DocumentMap& getDocumentManager() { return _docs; }
    inline size_t getNoDocuments() { return _docs.size(); }
    inline size_t getCurrID(){ return _countID; }
    // setter, if needed

  private:
    static size_t _countID;
    static std::string _dataDir;
    float_map _idf; // stores idf values
    DocumentMap _docs;
    InvertedIndex _invertedIndex;
    TieredIndex _tieredIndex;
};
