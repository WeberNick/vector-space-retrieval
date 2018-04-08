#pragma once

#include "types.hh"

#include <vector>
#include <map>
#include <string>
#include <vector>

class Document {
  public:
    explicit Document(const size_t aID, const std::string& aDocID, const std::string& aContent);
    explicit Document() = delete;
    Document(const Document&) = delete;
    Document(Document&&) = delete;
    Document& operator=(const Document&) = delete;
    Document& operator=(Document&&) = delete;
    ~Document();

  public:
    inline size_t getID() { return _ID; }
    inline const std::string& getDocID() { return _docID; }
    inline const std::string& getContent() { return _content; }
    inline uint getTF(const std::string& aTerm) { return _tf->find(aTerm)->second; }

  private:
    size_t _ID;           // e.g. 5
    std::string _docID;   // e.g. MED-123
    std::string _content; // e.g. studi run [...]
    int_map* _tf;         // stores calcTF values
};

typedef std::vector<Document> doc_vt;
typedef std::vector<const Document&> doc_ref_vt;
typedef std::vector<const Document*> doc_ptr_vt;
typedef std::map<size_t, Document> doc_mt;
typedef std::pair<size_t, Document> doc_map_elem_t;
typedef doc_mt::iterator doc_map_iter_t;
// typedef std::pair<DocMapIterator, bool> ...;

