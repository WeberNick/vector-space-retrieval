#include "ir_util.hh"
#include "vec_util.hh"
#include "gtest/gtest.h"

TEST(Utils, Random_Vector_Size_Equals_Test) {
    uint seed = 123;
    std::vector<float> vec = Util::generateRandomVector(10, 0, 1000, seed);
    EXPECT_EQ(10, vec.size());
}

TEST(Utils, Dot_Product_Equals_Test) {

    std::vector<float> vec_a = { 90, 36, 28, 94, 9, 51, 47, 50, 12, 9 };
    std::vector<double> vec_b = { 1.60159, -0.25909, 0.174768, -1.49896, -0.302023, 0.119264, 0.188985, 0.458181, -2.36984, 0.394974 };
    std::vector<float> vec_c(vec_b.begin(), vec_b.end());
    EXPECT_EQ(9.0794126987457275391, Util::scalar_product(vec_a, vec_c));
}

TEST(Utils, Pop_Front_Equals_Test) {

    std::vector<float> vec_a = { 5, 4, 2, 1 };
    std::vector<float> vec_a_first_popped = { 4, 2, 1 };
    float elem = Util::pop_front(vec_a);
    EXPECT_EQ(vec_a_first_popped, vec_a);
    EXPECT_EQ(elem, 5);

    std::vector<std::string> vec_b = { "Hi", "This", "Is", "John" };
    std::vector<std::string> vec_b_first_popped = { "This", "Is", "John" };
    const std::string& str_elem = Util::pop_front(vec_b);
    EXPECT_EQ(vec_b_first_popped, vec_b);
    EXPECT_EQ(str_elem, "Hi");
}