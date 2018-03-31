//
// Created by Alexander Weiß on 31.03.18.
//

#include "gtest/gtest.h"
#include <util/utils.hh>

TEST(To_Lower_Equals_Test, Equals) {

  std::string loweredWord = utils::to_lower("You");

  EXPECT_EQ("you", loweredWord);
}

TEST(Count_Term_Equals_Test, Equals) {

  std::string sentence = "Hi how are you today, you look gorgeous";
  EXPECT_EQ(2, utils::count_word_in_string(sentence, "you", true));
}

TEST(Random_Vector_Size_Equals_Test, Equals) {

  std::vector<double> vec = utils::generate_random_vector(10, 0, 1000);

  EXPECT_EQ(10, vec.size());
}