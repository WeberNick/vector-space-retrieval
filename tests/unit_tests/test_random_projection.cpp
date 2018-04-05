//
// Created by Alexander Weiß on 31.03.18.
//

#include "random_projection.hh"
#include "gtest/gtest.h"

TEST(Dimension_Estimation_Equals_Test, Equals) {

    int dimension = 1000;

    EXPECT_EQ(5920, random_projection::dimension(dimension));
}
