/*
 * @file    vec_util.hh
 * @author 	Nick Weber
 *          Alexander Weiß
 *          Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 * @date    Mai 18, 2018
 * @section	DESCRIPTION doc_to
 */
#pragma once

#include "types.hh"
#include "exception.hh"
#include "trace.hh"
#include "measure.hh"

#include <vector>
#include <iterator>
#include <algorithm>
#include <random>
#include <functional>

namespace Util {
    /**
     * @brief Create a vector consisting of the set difference of two vectors
     *
     * @param v1 the first vector
     * @param v2 the second vector
     * @return a vector containing the set difference of both input vectors
     * @optionaltodo implement function as template for generic vector types if needed
     */
    sizet_vt difference(const sizet_vt& v1, const sizet_vt& v2);
    // template<typename T> std::vector<T> difference(std::vector<T>& v1, std::vector<T>& v2);

    /**
     * @brief Create a vector consisting of the set intersection of two vectors
     * @param v1 the first vector
     * @param v2 the second vector
     * @return a vector containing the set intersection of both input vectors
     * @optionaltodo implement function as template for generic vector types if needed
     */
    sizet_vt intersection(const sizet_vt& v1, const sizet_vt& v2);
    // template<typename T> std::vector<T> intersection(std::vector<T>& v1, std::vector<T>& v2);

    /**
     * @brief Calculate the number of different elements in both input vectors
     * @param v1 the first vector
     * @param v2 the second vector
     * @return the number of different elements
     * @optionaltodo implement function as template for generic vector types if needed
     */
    size_t numberOfDifferences(const sizet_vt& v1, const sizet_vt& v2);
    // template<typename T> size_t numberOfDifferences(std::vector<T>& v1, std::vector<T>& v2);

    /**
     * @brief Calculate the number of equal elements in both input vectors
     * @param v1 the first vector
     * @param v2 the second vector
     * @return the number of equal elements
     * @optionaltodo implement function as template for generic vector types if needed
     */
    size_t numberOfIntersections(const sizet_vt& v1, const sizet_vt& v2);
    // template<typename T> size_t numberOfIntersections(std::vector<T>& v1, std::vector<T>& v2);

    /**
     * @brief Generates a random vector of floats with the given dimension and number in the range of min - max
     *
     * @param dimension dimension of the created vector
     * @param min minimum number value
     * @param max maximum number value
     * @param seed seed value for the random number generator
     * @return
     */
    float_vt generateRandomVector(size_t dimension, int min, int max, uint& seed);

    /**
     * @brief Appends the second vector to the first one and returns a copy of a new combined vector
     *
     * @param a first vector
     * @param b second vector
     * @return float_vt copy of the result
     */
    float_vt combineVectors(const float_vt& a, const float_vt& b);

    /**
     * @brief Calculates the dot product of two vectors
     *
     * @param a first vector
     * @param b second vector
     * @return the scalar product
     */
    template <typename T>
    inline double scalar_product(std::vector<T> const& a, std::vector<T> const& b) {
        if (a.size() != b.size()) {
            const std::string traceMsg = "Vectors do not have the same size";
            TRACE(traceMsg);
            throw VectorException(FLF, traceMsg);
        }
        return std::inner_product(a.begin(), a.end(), b.begin(), 0.0);
    }

    /**
     * @brief the random projection hash function, returning whether the scalar product
     *        between origVec and randVec is >= 0 (this will result in a 1 in the random projection vector)
     * 
     * @param origVec the original vector
     * @param randVec the random projection vector
     * @return whether the dot product is >= 0
     */
    inline bool randomProjectionHash(std::vector<float>& origVec, std::vector<float>& randVec) {
        if (origVec.size() != randVec.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors are not the same size");

        double dot = scalar_product(origVec, randVec);
        return dot >= 0;
    }
}
