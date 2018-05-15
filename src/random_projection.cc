#include "random_projection.hh"


RandomProjection::~RandomProjection() = default;

RandomProjection::RandomProjection() {}

RandomProjection& RandomProjection::getInstance() {
    static RandomProjection lInstance;
    return lInstance;
}

/**
 * Use random projections to reduce the number of dimensions of a vector
 *
 * @param vector original vector
 * @param hashFunc hash function to use to combine original vector and random vectors
 * @return boost::dynamic_bitset bitvector
 */
boost::dynamic_bitset<> RandomProjection::localitySensitiveHashProjection(std::vector<float>& vector,
                                                                          std::function<unsigned int(std::vector<float>&, std::vector<float>&)> hashFunc) {

    boost::dynamic_bitset<> result(_dimension);
    for (size_t j = 0; j < _dimension; ++j) {
        result[j] = hashFunc(vector, _randomVectors[j]);
    }

    return result;
}