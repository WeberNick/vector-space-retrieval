/**
 *	@file 	document.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  Implements the document
 *	@bugs 	Currently no bugs known
 *	@todos	Write DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include "exception.hh"
#include "types.hh"

#include <boost/dynamic_bitset.hpp>
#include <map>
#include <string>
#include <utility>
#include <vector>

class Document {
  public:
    explicit Document(const std::string& aDocID, const string_vt& aContent);
    explicit Document(const Document&);
    explicit Document() = delete;
    Document(Document&&) = default;
    Document& operator=(const Document& doc) = delete;
    Document& operator=(Document&&) = delete;
    ~Document();

  public:
    /**
     * @brief Get the number of created documents
     *
     * @return size_t document count
     */
    inline static size_t getDocumentCount() { return Document::_documentCount; }

    /**
     * @brief Get the id of the document
     *
     * @return size_t the id
     */
    inline size_t getID() const { return _ID; }
    /**
     * @brief Get the docID of the document (e.g. "MED-123")
     *
     * @return const std::string& the docID
     */
    inline const std::string& getDocID() const { return _docID; }
    /**
     * @brief Get the content of the document
     *
     * @return const string_vt& the content
     */
    inline const string_vt& getContent() const { return _content; }
    /**
     * @brief Get the term to term frequency map of the document
     *
     * @return const str_float_mt& the term tf map
     */
    inline str_float_mt& getTermTfMap() { return _term_tf_map; }
    /**
     * @brief Get the term to term frequency map of the document
     *
     * @return const str_float_mt& the term tf map
     */
    inline const str_float_mt& getTermTfMap() const { return _term_tf_map; }
    /**
     * @brief Get the tf idf vector of the document
     *
     * @return const float_vt& the tf idf vector
     */
    inline float_vt& getTfIdfVector() { return _tf_idf_vec; }
    /**
     * @brief Get the tf idf vector of the document
     *
     * @return const float_vt& tf idf vector
     */
    inline const float_vt& getTfIdfVector() const { return _tf_idf_vec; }
    /**
     * @brief Get the tf idf vector of the document
     *
     * @return const float_vt& the tf idf vector
     */
    inline float_vt& getWordEmbeddingsVector() { return _wordembeddings_vec; }
    /**
     * @brief Get the tf idf vector of the document
     *
     * @return const float_vt& tf idf vector
     */
    inline const float_vt& getWordEmbeddingsVector() const { return _wordembeddings_vec; }
    /**
     * @brief Get the random projection vector of the document
     *
     * @return boost::dynamic_bitset<>& the random projection vector
     */
    inline boost::dynamic_bitset<>& getRandProjTiVec() { return _rand_proj_ti_vec; }
    /**
     * @brief Get the random projection vector of the document
     *
     * @return boost::dynamic_bitset<>& the random projection vector
     */
    inline const boost::dynamic_bitset<>& getRandProjTiVec() const { return _rand_proj_ti_vec; }
    /**
     * @brief Get the random projection vector of the document
     *
     * @return boost::dynamic_bitset<>& the random projection vector
     */
    inline boost::dynamic_bitset<>& getRandProjWeVec() { return _rand_proj_we_vec; }
    /**
     * @brief Get the random projection vector of the document
     *
     * @return boost::dynamic_bitset<>& the random projection vector
     */
    inline const boost::dynamic_bitset<>& getRandProjWeVec() const { return _rand_proj_we_vec; }
    /**
     * @brief Get the normalization factor of the document
     *
     * @return float the norm length
     */
    inline float getNormLength() const { return _norm_length; }

    /**
     * @brief Get the tf of aTerm in this document
     *
     * @param aTerm the term for which the tf is returned
     * @return float the term frequency of aTerm
     */
    float getTf(const std::string& aTerm) const;

    /**
     * @brief Set the term tf map
     *
     * @param termTFMap the map to set
     */
    inline void setTermTfMap(str_float_mt& termTFMap) { _term_tf_map = termTFMap; }
    /**
     * @brief Set the normalization factor of this document
     *
     * @param nl the normalization factor
     */
    inline void setNormLength(float nl) { _norm_length = nl; }
    /**
     * @brief Set the tfidf vector of this document
     *
     * @param vec the tfidf vector
     */
    inline void setTfIdfVector(const float_vt& vec) { _tf_idf_vec = vec; }
    /**
     * @brief Set the tfidf vector of this document
     *
     * @param vec the tfidf vector
     */
    inline void setWordEmbeddingsVector(const float_vt& vec) { _wordembeddings_vec = vec; }
    // TODO docs
    /**
     * @brief Set the Rand Proj Vec object
     *
     * @param rand_proj
     */
    inline void setRandProjVec(const boost::dynamic_bitset<>& rand_proj) { _rand_proj_ti_vec = rand_proj; }

    /**
     * @brief Override operator<< for pretty printing a Document object
     *
     * @param strm the output stream
     * @param doc the Document Object
     * @return std::ostream& the output stream to be returned
     */
    friend std::ostream& operator<<(std::ostream& strm, const Document& doc);

  private:
    static size_t _documentCount;

    size_t _ID;                                // e.g. 5
    std::string _docID;                        // e.g. MED-123
    string_vt _content;                        // e.g. [studi, run, fish, ...]
    str_float_mt _term_tf_map;                 // stores TF values
    float_vt _tf_idf_vec;                      // e.g. <0, 2, 1.5, 3, .84, ..>
    float_vt _wordembeddings_vec;           
    boost::dynamic_bitset<> _rand_proj_ti_vec; // e.g. <0, 1, 1, 1, 0, 1, ..>
    boost::dynamic_bitset<> _rand_proj_we_vec; // e.g. <0, 1, 1, 1, 0, 1, ..>
    float _norm_length;                        // normalization factor of _tf_idf_vec
};

using doc_ptr_vt = std::vector<const Document*>;
using doc_mt = std::map<size_t, Document>;
using doc_map_elem_t = std::pair<size_t, Document>;
using doc_map_iter_t = doc_mt::iterator;
