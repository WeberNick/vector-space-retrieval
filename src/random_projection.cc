#include "random_projection.hh"

RandomProjection::RandomProjection() :
    _cb(nullptr), _randomVectors(), _dimension(0), _origVectorSize(0)
{}

RandomProjection& RandomProjection::getInstance() {
    static RandomProjection lInstance;
    return lInstance;
}
    
void RandomProjection::init(const control_block_t& aCB, const size_t origVectorSize) {
        if (!_cb) {
            _cb = &aCB;
            _dimension = _cb->_noDimensions;

            if (_dimension == 0) throw "Random projection dimension equals 0, must be > 0 ";

            setOrigVectorSize(origVectorSize);
            initRandomVectors();
        }
    }

/**
 * Use random projections to reduce the number of dimensions of a vector
 *
 * @param vector original vector
 * @param hashFunc hash function to use to combine original vector and random vectors
 * @return boost::dynamic_bitset bitvector
 */
boost::dynamic_bitset<> RandomProjection::localitySensitiveHashProjection(float_vt& vector,
                                                                          std::function<unsigned int(float_vt&, float_vt&)> hashFunc) {

    boost::dynamic_bitset<> result(_dimension);
    for (size_t j = 0; j < _dimension; ++j) {
        result[j] = hashFunc(vector, _randomVectors[j]);
    }

    return result;
}
