//
// Created by Alexander Weiß on 31.03.18.
//

#include "utility.hh"
#include "gtest/gtest.h"

TEST(Random_Vector_Size_Equals_Test, Equals) {

    std::vector<float> vec = Utility::generateRandomVector(10, 0, 1000);

    EXPECT_EQ(10, vec.size());
}

TEST(Dot_Product_Equals_Test, Equals) {

    std::vector<float> vec_a = { 90, 36, 28, 94, 9, 51, 47, 50, 12, 9 };
    std::vector<double> vec_b = { 1.60159, -0.25909, 0.174768, -1.49896, -0.302023, 0.119264, 0.188985, 0.458181, -2.36984, 0.394974 };
    std::vector<float> vec_c(vec_b.begin(), vec_b.end());

    EXPECT_EQ(9.0794126987457275391, Utility::scalar_product(vec_a, vec_c));
}
