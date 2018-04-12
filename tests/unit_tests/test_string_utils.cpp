//
// Created by Alexander Weiß on 07.04.18.
//

//
// Created by Alexander Weiß on 31.03.18.
//

#include "utility.hh"
#include "gtest/gtest.h"
#include <vector>

TEST(To_Lower_Equals_Test, Equals) {

    std::string loweredWord = Utility::StringOp::toLower("You");

    EXPECT_EQ("you", loweredWord);
}

//TODO: Fails
TEST(Count_Word_Equals_Test, Equals) {

    std::string sentence = "Hi how are you today, you look gorgeous";
    EXPECT_EQ(2, Utility::StringOp::countWordInString(sentence, "you", true));
}


TEST(Count_Word_Equals_Test_VT, Equals) {

    std::vector<std::string> sentence = { "Hi", "how", "are", "you", "today,", "you", "look", "gorgeous" };
    EXPECT_EQ(2, Utility::StringOp::countWordInString(sentence, "you", true));
}

TEST(Word_Trim_Equals_Test, Equals) {

    std::string sentence = " Whitespaces are lost ";
    Utility::StringOp::trim(sentence);
    EXPECT_EQ("Whitespaces are lost", sentence);
}


TEST(Split_String_By, Equals) {

    std::string sentence = "This is split by Space.";
    string_vt vec ={"This", "is", "split", "by", "Space." };
    string_vt vec_comp ={"This", "is", "split", "by", "Space." };
    Utility::StringOp::splitStringBy(sentence, ' ', vec);
    EXPECT_EQ(vec_comp, vec);
}

TEST(Split_String, Equals) {

    std::string sentence = "This is split by Space.";
    string_vt vec;
    string_vt vec_comp ={"This", "is", "split", "by", "Space." };
    Utility::StringOp::splitString(sentence, ' ', vec);
    EXPECT_EQ(vec_comp, vec);
}