//
// Created by Alexander Weiß on 31.03.18.
//

#pragma once

<<<<<<< develop

#include <string>
#include <Eigen/Dense>
namespace random_projection {
=======
#include "types.hh"
#include "utility.hh"
#include <lib/eigen/Eigen/Dense>
#include <string>
>>>>>>> Transfer the random_projection workflow into a class

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
    std::vector<float> localiltySensitveHashProjection(std::vector<float>& vector, float (*hashFunc)(std::vector<float>&, std::vector<float>&));

  private:
    const int dimension(int& sample, float eps = 0.1);
    Eigen::MatrixXf createRandomMatrix(int rows, int cols, bool JLT, double eps = 0.1, std::string projection = "gaussian");
    const double rand_normal(double mean, double stddev);

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
            for (int i = 0; i < _dimension; ++i) {
                _randomVectors.push_back(Utility::generateRandomVector(_origVectorSize, 0, 1));
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
