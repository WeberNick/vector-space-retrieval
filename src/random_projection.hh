/**
 *	@file 	random_projection.hh
 *	@author	Alexander Weiß
 *	@brief  TODO
 *	@bugs 	Currently no bugs known
 *	@todos	Write @brief and DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
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
     * @brief Uses
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
     * @brief Set the Dimensions object
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
     * @brief Initilaizes the random vectors
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
