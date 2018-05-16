//
// Created by Alexander Weiß on 27.04.18.
//

#pragma once

#include "types.hh"
#include "utility.hh"

#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using str_float_vt_mt = std::map<std::string, float_vt >;
using word_embedding_map_elem_t = std::pair<std::string, float_vt >;

class WordEmbeddings {
  private:
    explicit WordEmbeddings();
    WordEmbeddings(const WordEmbeddings&) = delete;
    WordEmbeddings(WordEmbeddings&&) = delete;
    WordEmbeddings& operator=(const WordEmbeddings&) = delete;
    WordEmbeddings& operator=(WordEmbeddings&&) = delete;
    ~WordEmbeddings();

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
     * @brief Get he number of word embeddings
     * @return
     */
    inline size_t getNoWordEmbeddings() { return (*_wordEmbeddings).size(); };

    /**
     * @Get the word embedding vector of one word
     * @param word
     * @return
     */
     float_vt& getWordEmbeddings(std::string &word);

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
     * @param aControlBlock
     */
    void init(const control_block_t& aControlBlock);

  private:
    const control_block_t* _cb;
    bool _init;

    std::string _modelFile;
    str_float_vt_mt* _wordEmbeddings = new str_float_vt_mt();
};
