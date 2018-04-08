#pragma once

#include "types.hh"

#include <map>
#include <string>
#include <vector>

class Document {
  public:
    explicit Document(const std::string& aDocID, const std::string& aContent);
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
    static size_t _documentCount;
    size_t _ID;           // e.g. 5
    std::string _docID;   // e.g. MED-123
    std::string _content; // e.g. studi run [...]
    int_map* _tf;         // stores calcTF values
};
