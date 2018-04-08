//
// Created by Alexander Weiß on 30.03.18.
//

#include "utility.hh"
#include "gtest/gtest.h"

TEST(Euclidean_Distance_Equals_Test, Equals) {

    std::vector<float> doc_a = { 1, 3, 5, 8, 100, 100 };
    std::vector<float> doc_b = { 2, 4, 5, 1, 2, 0 };

    EXPECT_FLOAT_EQ(140.196289, Utility::SimilarityMeasures::calcEuclDist(doc_a, doc_b));
}

TEST(Euclidean_Distance_Normalized_Equals_Test, Equals) {

    std::vector<float> doc_a = { 1, 3, 5, 8, 100, 100 };
    std::vector<float> doc_b = { 2, 4, 5, 1, 2, 0 };

    EXPECT_FLOAT_EQ(1.22768807, Utility::SimilarityMeasures::calcEuclDistNormalized(doc_a, doc_b));
}

TEST(Cosine_Similarity_Equals_Test, Equals) {

    std::vector<float> doc_a = { 1, 3, 5, 8, 100, 100 };
    std::vector<float> doc_b = { 2, 4, 5, 1, 2, 0 };

    EXPECT_FLOAT_EQ(0.246390939, Utility::SimilarityMeasures::calcCosSim(doc_a, doc_b));
}
