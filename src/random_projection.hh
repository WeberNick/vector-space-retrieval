/*
 * @file    random_projection.hh
 * @author 	Alexander Weiß
 * @date    April 4, 2018
 * @brief 	Implementing the random projections to lower the dimensions
 *          of the TF-IDF vectors of documents
 * 
 * @section	DESCRIPTION docto_
 */

#pragma once

#include "types.hh"
#include "exception.hh"
#include "trace.hh"
#include "vec_util.hh"

#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

class RandomProjection {
  private:
    RandomProjection();
    RandomProjection(const RandomProjection&) = delete;
    RandomProjection(RandomProjection&&) = delete;
    RandomProjection& operator=(const RandomProjection&) = delete;
    RandomProjection& operator=(RandomProjection&&) = delete;
    ~RandomProjection() = default;

  public:
    static RandomProjection& getInstance();
    /**
     * Use random projections to reduce the number of dimensions of a vector
     *
     * @param vector original vector
     * @param hashFunc hash function to use to combine original vector and random vectors
     * @return boost::dynamic_bitset bitvector
     */
    boost::dynamic_bitset<> localitySensitiveHashProjection(std::vector<float>& vector, std::function<unsigned int(std::vector<float>&, std::vector<float>&)>);

  public:
    /**
     * @brief Get the random vectors
     * 
     * @return const float_vector_vt& the random vectors
     */
    inline const float_vector_vt& getRandomVectors() { return _randomVectors; }
    /**
     * @brief Get the dimensions
     * 
     * @return size_t the dimensions
     */
    inline size_t getDimensions() { return _dimension; };
    /**
     * @brief Get the orig vector size
     * 
     * @return size_t the orig vector size
     */
    inline size_t getOrigvectorSize() { return _origVectorSize; };
    /**
     * @brief Get the seed
     * 
     * @return uint the seed
     */
    inline uint getSeed() { return _seed; };

    /**
     * @brief Set the dimensions attribute, determines the dimension of the random projection vectors
     *
     * @param dimensions the dimensions
     * @return whether the dimension has been set
     */
    inline bool setDimensions(const size_t dimensions) {
        if (_dimension) {
            return false;
        } else {
            _dimension = dimensions;
            return true;
        }
    }
    /**
     * @brief Set the random vectors
     *
     * @param randomVectors the random vectors
     */
    inline void setRandomVectors(float_vector_vt randomVectors) { _randomVectors = randomVectors; }
    /**
     * @brief Set the orig vector size
     *
     * @param origVectorSize the orig vector size
     * @return bool indicating whether the orig size has been set
     */
    inline bool setOrigVectorSize(const size_t origVectorSize) {
        if (_origVectorSize) {
            return false;
        } else {
            _origVectorSize = origVectorSize;
            return true;
        }
    }
    
    /**
     * @brief Initialize control block and the random projection
     *
     * @param aCB the control block
     * @param origVectorSize the orig vector size
     */
    void init(const CB& aCB, const size_t origVectorSize); 

    /**
     * @brief Initialize the random vectors
     *
     * @return true
     * @return bool indicating whether the random vectors have been set
     */
    inline bool initRandomVectors() {
        if (_dimension) {
            for (size_t i = 0; i < _dimension; ++i) {
                _randomVectors.push_back(Util::generateRandomVector(_origVectorSize, -1, 1, _seed));
            }
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief reset the random projections and set a new vector size
     * 
     * @param origVectorSize the new vector size
     */
    inline void reset(const size_t origVectorSize) {
        _origVectorSize = origVectorSize;
        _randomVectors.clear();
    }

  private:
    const CB* _cb;

    uint            _seed;
    float_vector_vt _randomVectors;
    size_t          _dimension;
    size_t          _origVectorSize;
};
