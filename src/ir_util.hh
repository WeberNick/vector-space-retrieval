/*
 * @file    ir_util.hh
 * @author 	Nick Weber
 * @date    Mai 18, 2018
 * 
 * @section	DESCRIPTION docto_
 */
#pragma once

#include "exception.hh"
#include "posting_list.hh"
#include "string_util.hh"
#include "trace.hh"
#include "types.hh"

#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>
#include <stemming/english_stem.h>

namespace Util {

    /**
     * @brief Calculates the term frequency a term
     *
     * @param term count of the term
     * @param max maximum count of another term in a document
     * @return float
     */
    float calcTf(const float term, const float max);

    /**
     * @brief Calculates the IDF of a term
     *
     * @param term
     * @param max
     * @return float
     */
    float calcIdf(const float N, const float docs); 

    /**
     * @brief Calculates the tf-idf value
     *
     * @param tf the term frequency
     * @param idf  the inverse document frequency
     * @return the idf value
     */
    float calcTfIdf(const float tf, const float idf);
    
    /**
     * @brief Calculates the term frequency of a given term inside a given string vector
     *
     * @param term the term to calculate the frequency of
     * @param content The content vector of terms
     * @return the term frequency
     */
    float calcTf(const std::string& term, const std::vector<std::string>& content);

    /**
     * @brief Stems a string word based on the porter stemmer algorithm. Uses https://github.com/OleanderSoftware/OleanderStemmingLibrary
     *
     * @param sentence
     * @return
     */
    std::string stemPorter(const std::string& sentence); 

    /**
     * @brief Pop the front element of a vector, modify the vector accordingly
     *        (so it contains one element less - the front element) and return
     *        this front element
     * 
     * @tparam T the type of the vector
     * @param vec the vector to modify
     * @return T the front element
     */
    template <typename T>
    inline T pop_front(std::vector<T>& vec) {
        assert(!vec.empty());
        T elem = *vec.begin();
        vec.erase(vec.begin());
        return elem;
    }

    /**
     * @brief Calculate the tiers for a given number of tiers and a posting list
     *        input: (1, 0.5), (2, 0), (3, 0.45), ..
     *        output: {(0, <PLObj>: (1, 0.4), (3, 0.8), ..), (1, ..), ..}
     * 
     * @param aNumTiers the number of tiers
     * @param aPostingList the posting list
     * @return std::map<size_t, PostingList> the map of tier, posting list
     */
    std::map<size_t, PostingList> calculateTiers(const size_t aNumTiers, const PostingList& aPostingList);

    /**
     * @brief Merge two vectors by taking the intersection and writing to out
     *
     * @param first the first postinglist
     * @param second the second postinglist
     * @param out the result vector
     */
    void andPostingLists(sizet_vt& first, sizet_vt& second, sizet_vt& out);
    /**
     * @brief Merge vectors by taking the intersection and writing to out
     *
     * @param vecs containing the posting list vectors
     * @param out the result vector
     */
    void andPostingLists(std::vector<sizet_vt>& vecs, sizet_vt& out);
    /**
     * @brief Merge vectors by taking the union and writing to out
     * 
     * @param vecs containing the posting list vectors
     * @param out the result vector
     */
    void orPostingLists(std::vector<sizet_vt>& vecs, sizet_vt& out);
}
