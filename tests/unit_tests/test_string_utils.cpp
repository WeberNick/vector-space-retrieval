#include "utility.hh"
#include "gtest/gtest.h"
#include <vector>

TEST(StringOp, To_Lower_Equals_Test) {

    std::string loweredWord = Utility::StringOp::toLower("You");
    std::string anotherWord = Utility::StringOp::toLower("youagain");
    EXPECT_EQ("you", loweredWord);
    EXPECT_EQ("youagain", anotherWord);
}

TEST(StringOp, Ends_With_Equals_Test) {
    
    std::string word = "Thisends";
    EXPECT_EQ(true, Utility::StringOp::endsWith("Thisends", "sends"));
}

/*TEST(StringOp, Count_Word_Equals_Test) {

    std::string sentence = "Hi how are you today, you look gorgeous";
    EXPECT_EQ(2, Utility::StringOp::countWordInString(sentence, "you", true));
}*/

TEST(StringOp, Count_Word_Equals_Test_VT) {

    std::vector<std::string> sentence = { "Hi", "how", "are", "you", "today,", "you", "look", "gorgeous" };
    EXPECT_EQ(2, Utility::StringOp::countWordInString(sentence, "you", true));
}

TEST(StringOp, Word_Trim_Equals_Test) {

    std::string sentence = " Whitespaces are lost ";
    Utility::StringOp::trim(sentence);
    EXPECT_EQ("Whitespaces are lost", sentence);
}

TEST(StringOp, Split_String_By_Test) {

    std::string sentence = "This is split by Space.";
    string_vt vec;
    string_vt split_res = {"This", "is", "split", "by", "Space." };
    Utility::StringOp::splitString(sentence, ' ', vec);
    EXPECT_EQ(split_res, vec);
}

TEST(StringOp, Split_String_Test) {

    std::string sentence = "This is split by Space.";
    string_vt vec;
    string_vt vec_comp ={"This", "is", "split", "by", "Space." };
    Utility::StringOp::splitStringBoost(sentence, ' ', vec);
    EXPECT_EQ(vec_comp, vec);
}

TEST(StringOp, Max_Word_String_Equals_Test) {

    EXPECT_EQ(1, Utility::StringOp::getMaxWordFrequency("This has max=1 terms this lemon."));
    EXPECT_EQ(3, Utility::StringOp::getMaxWordFrequency("Ice Ice Baby, Vanilla Ice"));
    EXPECT_EQ(2, Utility::StringOp::getMaxWordFrequency("Ice Ice Baby, Vanilla Ice."));
    EXPECT_EQ(1, Utility::StringOp::getMaxWordFrequency("Hi this is me"));
    EXPECT_EQ(0, Utility::StringOp::getMaxWordFrequency(""));

}

TEST(StringOp, Max_Word_String_Equals_Test_VT) {

    string_vt sentence = {"This", "has", "max=2", "terms", "this", "lemon."};
    EXPECT_EQ(1, Utility::StringOp::getMaxWordFrequency(sentence));

    sentence = {"Ice", "Ice", "Baby,", "Vanilla", "Ice"};
    EXPECT_EQ(3, Utility::StringOp::getMaxWordFrequency(sentence));

    sentence = {"Hi", "this", "is", "me"};
    EXPECT_EQ(1, Utility::StringOp::getMaxWordFrequency(sentence));

    sentence = {};
    EXPECT_EQ(0, Utility::StringOp::getMaxWordFrequency(sentence));

    sentence = {""};
    EXPECT_EQ(0, Utility::StringOp::getMaxWordFrequency(sentence));
}