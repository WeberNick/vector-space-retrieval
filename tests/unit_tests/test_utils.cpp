//
// Created by Alexander Weiß on 31.03.18.
//

#include "utility.hh"
#include "gtest/gtest.h"

TEST(To_Lower_Equals_Test, Equals) {

    std::string loweredWord = toLower("You");

    EXPECT_EQ("you", loweredWord);
}

TEST(Count_Term_Equals_Test, Equals) {

    std::string sentence = "Hi how are you today, you look gorgeous";
    EXPECT_EQ(2, countWordInString(sentence, "you", true));
}

TEST(Random_Vector_Size_Equals_Test, Equals) {

    std::vector<double> vec = generateRandomVector(10, 0, 1000);

    EXPECT_EQ(10, vec.size());
}
