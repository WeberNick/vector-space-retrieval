//
// Created by Alexander Weiß on 31.03.18.
//

#include "utility.hh"
#include "gtest/gtest.h"

TEST(Random_Vector_Size_Equals_Test, Equals) {

    std::vector<float> vec = Utility::generateRandomVector(10, 0, 1000);

    EXPECT_EQ(10, vec.size());
}
