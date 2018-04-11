#pragma once

#include "types.hh"

#include <map>
#include <string>
#include <vector>
#include <utility>

class Document {
  public:
    explicit Document(const std::string& aDocID, const string_vt& aContent);
    explicit Document() = delete;
    //Document(const Document&) = delete;
    //Document(Document&&) = delete;
    //Document& operator=(Document&&) = delete;
    ~Document();
  private:
    Document& operator=(const Document& doc);

  public:
    inline size_t getID() { return _ID; }
    inline const std::string& getDocID() { return _docID; }
    inline const string_vt& getContent() { return _content; }
    inline uint getTF(const std::string& aTerm) { return _tf->find(aTerm)->second; }

  private:
    static size_t _documentCount;

    size_t _ID;         // e.g. 5
    std::string _docID; // e.g. MED-123
    string_vt _content; // e.g. [studi, run, ...]
    str_int_mt* _tf;    // stores TF values
};

typedef std::vector<const Document*> doc_ptr_vt;
typedef std::map<size_t, Document> doc_mt;
typedef std::pair<size_t, Document> doc_map_elem_t;
typedef doc_mt::iterator doc_map_iter_t;