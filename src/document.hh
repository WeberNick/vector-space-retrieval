#pragma once

#include "exception.hh"
#include "types.hh"

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
    void pprintTFMap();

  public:
    inline size_t getID() const { return _ID; }
    inline const std::string& getDocID() const { return _docID; }
    inline const string_vt& getContent() const { return _content; }
    inline void setTermTFMap(str_float_mt& termTFMap) { _term_tf_map = termTFMap; }
    inline const str_float_mt& getTermTFMap() const { return _term_tf_map; }
    inline float getTF(const std::string& aTerm) const {
        if (_term_tf_map.find(aTerm) != _term_tf_map.end())
            return _term_tf_map.at(aTerm);
        else
            return 0;
    }
    inline float_vt getTFIDFVector() { return _tf_idf_vec; }
    inline const void appendTFIDFValue(const float aVal) { _tf_idf_vec.push_back(aVal); }

  private:
    static size_t _documentCount;

    size_t _ID;                // e.g. 5
    std::string _docID;        // e.g. MED-123
    string_vt _content;        // e.g. [studi, run, ...]
    float _norm_length;        // normalization factor of _tf_idf_vec
    float_vt _tf_idf_vec;      // e.g. <0, 2, 1.5, 3, .84, ..>
    str_float_mt _term_tf_map; // stores TF values
};

using doc_ptr_vt = std::vector<const Document*>;
using doc_mt = std::map<size_t, Document>;
using doc_map_elem_t = std::pair<size_t, Document>;
using doc_map_iter_t = doc_mt::iterator;
