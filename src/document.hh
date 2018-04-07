#pragma once

#include "types.hh"

#include <vector>
#include <map>
#include <string>

class Document {
    public:
        typedef std::vector<Document> doc_vt;
        typedef std::vector<const Document&> doc_ref_vt;
        typedef std::vector<const Document*> doc_ptr_vt;

  public:
    explicit Document(const size_t aDocID, const std::string& aPath);
    explicit Document() = delete;
    Document(const Document&) = delete;
    Document(Document&&) = delete;
    Document& operator=(const Document&) = delete;
    Document& operator=(Document&&) = delete;
    ~Document();

  public:
    // todo: helper function to process content of a document

  public:
    inline size_t getID() { return _ID; }
    inline std::string getContent() { return _content; }

  private:
    size_t _ID;
    std::string _content;
    float_map* _tfidf; // stores tfidf values
};
