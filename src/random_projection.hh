/*********************************************************************
 * @file    query_execution_engine.hh
 * @author 	Alexander Weiß
 * @date    April 4, 2018
 * @brief 	Implementing the random projections to lower the dimensions
 *          of the TF-IDF vectors of documents
 * @bugs 	  Currently no bugs known 
 * @todos 	Currently no todos
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
     * @brief Implements a hash function which determines the value of an random projection vector at a given position
     * The implemented hash function is simple:
     *      if scalar_product(vec_a, vec_b) > 0 ? return 1 : return 0;
     *
     * @param vector
     * @return boost::dynamic_bitset<>
     */
    boost::dynamic_bitset<> localitySensitiveHashProjection(float_vt& vector, std::function<unsigned int(float_vt&, float_vt&)>);

  public:
    inline const float_vector_vt& getRandomVectors() { return _randomVectors; }
    inline size_t getDimensions() { return _dimension; };
    inline size_t getOrigvectorSize() { return _origVectorSize; };

    /**
     * @brief Set the Dimensions object, determines the dimension of the random projection vectors
     *
     * @param dimensions
     * @return true
     * @return false
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
     * @brief Set the Random Vectors object
     *
     * @param randomVectors
     */
    inline void setRandomVectors(float_vector_vt randomVectors) { _randomVectors = randomVectors; }

    /**
     * @brief Set the Orig Vector Size object
     *
     * @param origVectorSize
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
     * @brief Initializes the RandomProjection object with the control block
     *
     * @param aCB
     * @param origVectorSize
     * @return
     */
    void init(const CB& aCB, const size_t origVectorSize); 

    /**
     * @brief Initializes the random vectors
     *
     * @return true
     * @return bool indicating whether the random vectors have been set
     */
    inline bool initRandomVectors() {
        if (_dimension) {
            for (size_t i = 0; i < _dimension; ++i) {
                _randomVectors.push_back(Util::generateRandomVectorN(_origVectorSize));
            }
            return true;
        } else {
            return false;
        }
    }

  private:
    const CB* _cb;

    float_vector_vt _randomVectors;
    size_t _dimension;
    size_t _origVectorSize;
};
