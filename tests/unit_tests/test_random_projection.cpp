#include "random_projection.hh"
#include "gtest/gtest.h"

unsigned int sampleHashing(std::vector<float>& origVec, std::vector<float>& randVec) {
    if (origVec.size() != randVec.size())
        throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors are not the same size");

    double dot = Utility::scalar_product(origVec, randVec);
    if (dot > 0.75) {
        return 1;
    } else {
        return 0;
    }
}

TEST(RandomProjection, Dimension_Estimation_Equals_Test) {

    std::vector<std::vector<float>> randomVectors = { { 0.33, 0.33, 0.42, 0.12, 0.2, 0.34, 0.58, 0.19, 0.07, 0.24 },
                                                      { 0.29, 0.16, 0.38, 0.48, 0.43, 0.11, 0.12, 0.33, 0.03, 0.44 },
                                                      { 0.01, 0.17, 0.11, 0.27, 0.23, 0.37, 0.35, 0.48, 0.54, 0.24 },
                                                      { 0.09, 0.05, 0.39, 0.25, 0.45, 0.48, 0.04, 0.45, 0.35, 0.12 },
                                                      { 0.13, 0.17, 0.4, 0.4, 0.07, 0.4, 0.35, 0.39, 0.44, 0.06 } };

    RandomProjection::getInstance().setDimensions(5);
    RandomProjection::getInstance().setOrigVectorSize(10);
    RandomProjection::getInstance().setRandomVectors(randomVectors);

    std::vector<float> d1 = { 0.17, 0.21, 0.35, 0.44, 0.49, 0.39, 0.09, 0.07, 0.37, 0.24 };
    boost::dynamic_bitset<> doc_1_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d1, sampleHashing);
    boost::dynamic_bitset<> compare(std::string("11110"));

    EXPECT_EQ(compare, doc_1_proj);
}
