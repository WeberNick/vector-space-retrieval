#include "utility.hh"
#include "gtest/gtest.h"

TEST(IR, Threshold_Equals_Test) {
    
    const sizet_float_mt posting = { {1, 0.5}, {2, 0}, {3, 0.45}, {4, 0.6}, {5, 0.23}, {6, 0.34}, {7, 0.87},
                                     {8, 0.87}, {9, 0.91}, {10, 0.11}, {11, 0.22}, {12, 0.33}, {13, 0.44}, {14, 0.55} };
    EXPECT_EQ(true, Utility::IR::getThresholds("Thisends", "sends"));
}