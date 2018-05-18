/*********************************************************************
 * @file    similarity_util.hh
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

#include "document.hh"

#include <bits/stl_algo.h>
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <cmath>
#include <string>

namespace Util
{

        /**
         * @brief calculate and return the length of the given vector
         *
         * @param vec the vector
         * @return float the length
         */
        inline double vectorLength(const float_vt& vec) {
            double magn = 0;
            for (size_t i = 0; i < vec.size(); ++i) {
                magn += pow(vec[i], 2);
            }
            return sqrt(magn);
        }

        /**
         * @brief Calculates the cosine similarity between \aTfIdf_a and \aTfIdf_b
         *
         * @param aTfIdf_a a tf-idf vector
         * @param aTfIdf_b a tf-idf vector
         * @return the cosine similarity
         */
        float calcCosSim(const float_vt& aTfIdf_a, const float_vt& aTfIdf_b);

        /**
         * @brief Wrapper method for \link Utility#StringOp#calcCosSim() calcCosSim() \endlink which accepts documents instead of the raw vector
         *
         * @param doc_a
         * @param doc_b
         * @return the cosine similarity
         */
        float calcCosSim(const Document& doc_a, const Document& doc_b);
        /**
         * @brief Calculates the cosine distance of two vectors
         *
         * @param aTF_IDF_a
         * @param aTF_IDF_b
         * @return
         */
        float calcCosDist(const float_vt& aTF_IDF_a, const float_vt& aTF_IDF_b);

        /**
         * @brief Calculates the cosine distance of two documents
         *
         * @param aTF_IDF_a
         * @param aTF_IDF_b
         * @return
         */
        float calcCosDist(const Document& doc_a, const Document& doc_b);

        /**
         * Returns the angular similarity between two docs
         *
         * @see https://en.wikipedia.org/wiki/Cosine_similarity#Angular_distance_and_similarity
         * @param aTfIdf_a a tf-idf vector
         * @param aTfIdf_b a tf-idf vector
         * @return
         */
        float calcAngularSimilarity(const float_vt& aTfIdf_a, const float_vt& aTfIdf_b);

        /**
         * Returns the angular similarity between two docs
         *
         * @param doc_a
         * @param doc_b
         * @return
         */
        float calcAngularSimilarity(const Document& doc_a, const Document& doc_b);

        /**
         * @brief Calculates the Hamming distance between two std::vector<bools> vectors
         *
         * @param vec_a
         * @param vec_b
         * @return
         */
        unsigned int calcHammingDist(std::vector<bool>& vec_a, std::vector<bool>& vec_b);

        /**
         * @brief Calculates the Hamming distance between two boost::dynamic_bitset sets
         *
         * @param vec_a
         * @param vec_b
         * @return
         */
        unsigned int calcHammingDist(const boost::dynamic_bitset<>& vec_a, const boost::dynamic_bitset<>& vec_b);

        /**
         * @brief Calculates the Hamming similarity between two boost::dynamic_bitset sets
         * (\vec_a.size() -  \calcHammingDist(\vec_a, \vec_b))
         *
         * @param vec_a
         * @param vec_b
         * @return
         */
        unsigned int calcHammingSim(boost::dynamic_bitset<>& vec_a, boost::dynamic_bitset<>& vec_b);

        /**
         * Calculates the angular similarity of two LSH vectors
         *
         * @see https://stackoverflow.com/questions/12952729/how-to-understand-locality-sensitive-hashing
         * @param vec_a
         * @param vec_b
         * @return
         */
        float calcAngSimHamming(std::vector<bool>& vec_a, std::vector<bool>& vec_b);

        /**
         * @brief Calculates the euclidean distance between \doc_a and \doc_b
         *
         * @param doc_a a document
         * @param doc_b a document
         * @return the euclidean distance as double
         */
        float calcEuclDist(const float_vt& doc_a, const float_vt& doc_b);

        /**
         * @brief Calculates the normalized euclidean distance between \doc_a and \doc_b
         *
         * @param doc_a a document
         * @param doc_b a document
         * @return the euclidean distance as double
         */
        float calcEuclDistNormalized(float_vt& doc_a, float_vt& doc_b);
}
