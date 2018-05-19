/*********************************************************************
 * @file    ir_util.hh
 * @author 	Nick Weber
 * @date    Mai 18, 2018
 * @brief 	
 * @bugs 	TBD
 * @todos 	TBD
 * 
 * @section	DESCRIPTION
 * TBD
 * 
 * @section USE
 * TBD
 ********************************************************************/
#pragma once

#include "types.hh"
#include "exception.hh"
#include "trace.hh"
#include "posting_list.hh"

#include <stemming/english_stem.h>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <map>

namespace Util
{

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
         * @brief Calculates the term frequency of a given term inside a given string
         *
         * @param term the term to calculate the frequency of
         * @param content the content string in which the term appears
         * @return the term frequency
         */
        //float calcTf(const std::string& term, const std::string& content);
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

        template <typename T>
        inline T pop_front(std::vector<T>& vec) {
            assert(!vec.empty());
            T elem = *vec.begin();
            vec.erase(vec.begin());
            return elem;
        }

        // input: (1, 0.5), (2, 0), (3, 0.45), ..
        // output: {(0, <PLObj>: (1, 0.4), (3, 0.8), ..), (1, ..), ..}
        // TODO docs
        /**
         * @brief
         *
         * @param aNumTiers
         * @param aPostingList
         * @return std::map<size_t, PostingList>
         */
        std::map<size_t, PostingList> calculateTiers(const size_t aNumTiers, const PostingList& aPostingList);

        /**
         * @brief
         *
         * @param first
         * @param second
         * @return sizet_vt
         */
        void andPostingLists(sizet_vt& first, sizet_vt& second, sizet_vt& out);

        /**
         * @brief
         *
         * @param vecs
         * @param out
         */
        void andPostingLists(std::vector<sizet_vt>& vecs, sizet_vt& out);

        void orPostingLists(std::vector<sizet_vt>& vecs, sizet_vt& out);
}
