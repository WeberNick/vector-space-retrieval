//
// Created by Alexander Weiß on 30.03.18.
//

#include "gtest/gtest.h"
#include <similarity_measures.hh>
#include <util/utils.hh>

TEST(Euclidean_Distance_Equals_Test, Equals) {

  std::vector<double> doc_a = { 1, 3, 5, 8, 100, 100 };
  std::vector<double> doc_b = { 2, 4, 5, 1, 2, 0 };

  EXPECT_EQ(140.19629096377693, similarity_measures::euclidean_distance(doc_a, doc_b));
}

TEST(Euclidean_Distance_Normalized_Equals_Test, Equals) {

  std::vector<double> doc_a = { 1, 3, 5, 8, 100, 100 };
  std::vector<double> doc_b = { 2, 4, 5, 1, 2, 0 };

  EXPECT_EQ(1.2276881238958353, similarity_measures::euclidean_distance_normalized(doc_a, doc_b));
}

TEST(Cosine_Similarity_Equals_Test, Equals) {

  std::vector<double> doc_a = { 1, 3, 5, 8, 100, 100 };
  std::vector<double> doc_b = { 2, 4, 5, 1, 2, 0 };

  EXPECT_EQ(0.246390935222562, similarity_measures::cosine_similarity(doc_a, doc_b));
}
