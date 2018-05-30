/*
 * @file    word_embeddings.hh
 * @author 	Alexander Weiß
 * @date    April 27, 2018
 * @brief 	Implements the word embeddings which are used as additional features to
 *          improve the retrieval results
 * 
 * @section	DESCRIPTION docto_
 */

#pragma once

#include "types.hh"
#include "exception.hh"
#include "trace.hh"
#include "string_util.hh"

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <memory>
#include <map>

using str_float_vt_mt = std::map<std::string, float_vt>;
using word_embedding_map_elem_t = std::pair<std::string, float_vt>;

class WordEmbeddings {
  private:
    explicit WordEmbeddings();
    WordEmbeddings(const WordEmbeddings&) = delete;
    WordEmbeddings(WordEmbeddings&&) = delete;
    WordEmbeddings& operator=(const WordEmbeddings&) = delete;
    WordEmbeddings& operator=(WordEmbeddings&&) = delete;
    ~WordEmbeddings() = default;

  private:
    /**
     * @brief Read an word embedding model file
     *
     * @param aFile the model file to read
     */
    void read(const std::string& aFile);

    /**
     * @brief Insert element into manager (by std::pair element)
     *
     * @param aElement element to be inserted
     * @return true if insertion was successful
     * @return false if insertion failed
     */
    bool insert(const word_embedding_map_elem_t& aElement);

  public:
    /**
     * @brief Get the word embeddings map
     *
     * @return str_float_vt_mt&
     */
    inline str_float_vt_mt& getWordEmbeddingsMap() { return *_wordEmbeddings; };

    /**
     * @brief Get the number of word embeddings
     * @return
     */
    inline size_t getNoWordEmbeddings() { return (*_wordEmbeddings).size(); };

    /**
     * @Get the word embedding vector of one word
     * @param word
     * @return
     */
    float_vt& getWordEmbeddings(const std::string& word);

    /**
     * @brief Calculates the wordEmbeddings vector by getting all word2vecs for all words in the doc and averaging them
     *
     * @param doc_content
     * @param out
     */
    void calcWordEmbeddingsVector(const string_vt& doc_content, float_vt& out);

    /**
     * @brief Get the Instance object
     *
     * @return DocumentManager&
     */
    inline static WordEmbeddings& getInstance() {
        static WordEmbeddings lInstance;
        return lInstance;
    }
    /**
     * @brief
     *
     * @param aControlBlock the control block
     */
    void init(const control_block_t& aControlBlock);

  private:
    const control_block_t* _cb;

    std::string _modelFile;
    std::shared_ptr<str_float_vt_mt> _wordEmbeddings;
};
