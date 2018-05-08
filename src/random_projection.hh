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
#include "utility.hh"
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <string>

class RandomProjection {
  private:
    explicit RandomProjection();
    ~RandomProjection();

  public:
    RandomProjection(const RandomProjection&) = delete;
    RandomProjection(RandomProjection&&) = delete;
    RandomProjection& operator=(const RandomProjection&) = delete;
    RandomProjection& operator=(RandomProjection&&) = delete;

  public:
    static RandomProjection& getInstance();
    /**
     * @brief Uses
     *
     * @param vector
     * @return boost::dynamic_bitset<>
     */
    boost::dynamic_bitset<> localitySensitiveHashProjection(std::vector<float>& vector, std::function<unsigned int(std::vector<float>&, std::vector<float>&)>);

  public:
    inline const float_vector_vt& getRandomVectors() { return _randomVectors; }
    inline const size_t getDimensions() { return _dimension; };
    inline const size_t getOrigvectorSize() { return _origVectorSize; };

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
    inline void init(const control_block_t& aCB, const size_t origVectorSize) {
        if (!_init) {
            _cb = &aCB;
            _dimension = _cb->_noDimensions;

            if (_dimension == 0) throw "Random projection dimension equals 0, must be > 0 ";

            setOrigVectorSize(origVectorSize);
            initRandomVectors();
            _init = true;
        }
    }

    /**
     * @brief Initilaizes the random vectors
     *
     * @return true
     * @return bool indicating whether the random vectors have been set
     */
    inline bool initRandomVectors() {
        if (_dimension) {
            for (size_t i = 0; i < _dimension; ++i) {
                _randomVectors.push_back(Utility::generateRandomVectorN(_origVectorSize));
            }
            return true;
        } else {
            return false;
        }
    }

  private:
    const control_block_t* _cb;

    bool _init;
    float_vector_vt _randomVectors;
    size_t _dimension;
    size_t _origVectorSize;
};
