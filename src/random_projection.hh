//
// Created by Alexander Weiß on 31.03.18.
//

#pragma once

#include "types.hh"
#include "utility.hh"
#include <Eigen/Dense>
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
    const Eigen::MatrixXf projectMatrix();
    boost::dynamic_bitset<> localitySensitiveHashProjection(std::vector<float>& vector, std::function<unsigned int(std::vector<float>&, std::vector<float>&)>);

  private:
    const int dimension(int& sample, float eps = 0.1);
    Eigen::MatrixXf createRandomMatrix(int rows, int cols, bool JLT, double eps = 0.1, std::string projection = "gaussian");

  public:
    inline const float_vector_vt& getRandomVectors() { return _randomVectors; }
    inline const size_t getDimensions() { return _dimension; };
    inline const size_t getOrigvectorSize() { return _origVectorSize; };

    inline bool setDimensions(const size_t dimensions) {
        if (_dimension) {
            return false;
        } else {
            _dimension = dimensions;
            return true;
        }
    }

    inline bool setOrigVectorSize(const size_t origVectorSize) {
        if (_origVectorSize) {
            return false;
        } else {
            _origVectorSize = origVectorSize;
            return true;
        }
    }

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
    float_vector_vt _randomVectors;
    size_t _dimension;
    size_t _origVectorSize;
};
