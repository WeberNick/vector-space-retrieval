#pragma once

#include "document.hh"
#include "index_manager.hh"
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
    explicit DocumentManager(const std::string& aPath);
    explicit DocumentManager() = delete;
    DocumentManager(const DocumentManager&) = delete;
    DocumentManager(DocumentManager&&) = delete;
    DocumentManager& operator=(const DocumentManager&) = delete;
    DocumentManager& operator=(DocumentManager&&) = delete;
    ~DocumentManager();

  public:
    static void createInstance(const std::string& aPath);
    static void destroyInstance();
    static DocumentManager* getInstance();

  private:
    /* start the scan for files at the root directory and add all found docs to map */
    void read(const std::string& aFile);

  public:  /* Wrapper functions for the respective map container calls */
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
    inline const DocumentMap& getDocumentMap() { return _docs; }
    inline size_t getNoDocuments() { return _docs.size(); }
    inline size_t getCurrID(){ return _countID; }
    // setter, if needed
  
  private:
    static DocumentManager* _instance;
    static bool _isCreated;
    static size_t _countID;
    static std::string _collectionFile;

    DocumentMap _docs;
    IndexManager _indexMgr;
};
