#pragma once

#include "document.hh"
#include "index_manager.hh"
#include "types.hh"

#include <map>
#include <string>

class DocumentManager {
  public:
    typedef std::map<size_t, Document> doc_mt;
    typedef std::pair<size_t, Document> DocumentMapElem;
    typedef doc_mt::iterator DocMapIterator;
    // typedef std::pair<DocMapIterator, bool> ...;

  public:
    explicit DocumentManager(const std::string& aPath);
    explicit DocumentManager() = delete;
    DocumentManager(const DocumentManager&) = delete;
    DocumentManager(DocumentManager&&) = delete;
    DocumentManager& operator=(const DocumentManager&) = delete;
    DocumentManager& operator=(DocumentManager&&) = delete;
    ~DocumentManager();

  private:
    /* start the scan for files at the root directory and add all found docs to map */
    void read(const std::string& aFile);
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
    inline const doc_mt& getDocuments() { return _docs; }
    inline size_t getNoDocuments() { return _docs.size(); }

    inline static void createInstance(const std::string& aPath) {
        if (!DocumentManager::_isCreated) {
            DocumentManager::_isCreated = true;
            DocumentManager::_instance = new DocumentManager(aPath);
        }
    }

    inline static void destroyInstance() {
        if (DocumentManager::_isCreated) { delete DocumentManager::_instance; }
    }

    inline static DocumentManager* getInstance() {
        if (DocumentManager::_isCreated) {
            return DocumentManager::_instance;
        } else {
            return NULL;
        }
    }

  private:
    static DocumentManager* _instance;
    static const char _delimiter;
    static bool _isCreated;
    static size_t _countID;
    static std::string _collectionFile;

    IndexManager _indexMgr;
    doc_mt _docs;
};
