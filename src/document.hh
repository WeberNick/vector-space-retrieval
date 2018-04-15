#pragma once

#include "types.hh"
#include "exception.hh"

#include <map>
#include <string>
#include <utility>
#include <vector>

class Document {

  public:
    explicit Document(const std::string& aDocID, const string_vt& aContent);
    explicit Document(const Document&);
    explicit Document() = delete;
    Document(Document&&) = delete;
    Document& operator=(const Document& doc) = delete;
    Document& operator=(Document&&) = delete;
    ~Document();

  private:
    void buildTFMap();

  public:
    inline size_t getID() const { return _ID; }
    inline const std::string& getDocID() const { return _docID; }
    inline const string_vt& getContent() const { return _content; }
    inline const str_float_mt& getTermTFMap() const { return _term_tf_map; }
    inline float getTF(const std::string& aTerm) const {
        if (_term_tf_map.find(aTerm) != _term_tf_map.end())
            return _term_tf_map.at(aTerm);
        else
            throw InvalidArgumentException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "The term " + aTerm + " doest not appear in this document.");
    }

  private:
    static size_t _documentCount;

    size_t _ID;                // e.g. 5
    std::string _docID;        // e.g. MED-123
    string_vt _content;        // e.g. [studi, run, ...]
    str_float_mt _term_tf_map; // stores TF values
};

using doc_ptr_vt = std::vector<const Document*>;
using doc_mt = std::map<size_t, Document>;
using doc_map_elem_t = std::pair<size_t, Document>;
using doc_map_iter_t = doc_mt::iterator;
