#include "utility.hh"
#include "gtest/gtest.h"

TEST(SimilarityMeasures, Euclidean_Distance_Equals_Test) {

    std::vector<float> doc_a = { 1, 3, 5, 8, 100, 100 };
    std::vector<float> doc_b = { 2, 4, 5, 1, 2, 0 };
    EXPECT_FLOAT_EQ(140.196289, Utility::SimilarityMeasures::calcEuclDist(doc_a, doc_b));
}

TEST(SimilarityMeasures, Euclidean_Distance_Normalized_Equals_Test) {

    std::vector<float> doc_a = { 1, 3, 5, 8, 100, 100 };
    std::vector<float> doc_b = { 2, 4, 5, 1, 2, 0 };
    EXPECT_FLOAT_EQ(1.22768807, Utility::SimilarityMeasures::calcEuclDistNormalized(doc_a, doc_b));
}

TEST(SimilarityMeasures, Cosine_Similarity_Equals_Test) {

    std::vector<float> doc_a = { 1, 3, 5, 8, 100, 100 };
    std::vector<float> doc_b = { 2, 4, 5, 1, 2, 0 };
    EXPECT_FLOAT_EQ(0.246390939, Utility::SimilarityMeasures::calcCosSim(doc_a, doc_b));
}

TEST(SimilarityMeasures, Vector_Length_Test) {

    std::vector<float> doc = { 1, 3, 5, 8, 0, 4 };
    EXPECT_FLOAT_EQ(10.7238052948, Utility::SimilarityMeasures::vectorLength(doc));
}