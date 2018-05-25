#include "string_util.hh"
#include "gtest/gtest.h"
#include <vector>

TEST(StringOp, To_Lower_Equals_Test) {

    std::string loweredWord = Util::toLower("You");
    std::string anotherWord = Util::toLower("youagain");
    EXPECT_EQ("you", loweredWord);
    EXPECT_EQ("youagain", anotherWord);
}

TEST(StringOp, Ends_With_Equals_Test) {
    
    std::string word = "Thisends";
    EXPECT_EQ(true, Util::endsWith("Thisends", "sends"));
}

/*TEST(StringOp, Count_Word_Equals_Test) {

    std::string sentence = "Hi how are you today, you look gorgeous";
    EXPECT_EQ(2, Util::countWordInString(sentence, "you", true));
}*/

TEST(StringOp, Count_Word_Equals_Test_VT) {

    std::vector<std::string> sentence = { "Hi", "how", "are", "you", "today,", "you", "look", "gorgeous" };
    EXPECT_EQ(2, Util::countWordInString(sentence, "you", true));
}

TEST(StringOp, Word_Trim_Equals_Test) {

    std::string sentence = " Whitespaces are lost ";
    Util::trim(sentence);
    EXPECT_EQ("Whitespaces are lost", sentence);
}

TEST(StringOp, Split_String_Boost_Test) {

    std::string sentence = "This is split by Space.";
    string_vt vec;
    string_vt vec_comp = { "This", "is", "split", "by", "Space." };
    Util::splitStringBoost(sentence, ' ', vec);
    EXPECT_EQ(vec_comp, vec);

    std::string sentence_two = "today sun jon lemon food hi";
    string_vt vec_two;
    string_vt vec_comp_two = { "today", "sun", "jon", "lemon", "food", "hi" };
    Util::splitStringBoost(sentence_two, ' ', vec_two);
    EXPECT_EQ(vec_comp_two, vec_two);
}

TEST(StringOp, Max_Word_String_Equals_Test) {

    EXPECT_EQ(1, Util::getMaxWordFrequency("This has max=1 terms this lemon."));
    EXPECT_EQ(3, Util::getMaxWordFrequency("Ice Ice Baby, Vanilla Ice"));
    EXPECT_EQ(2, Util::getMaxWordFrequency("Ice Ice Baby, Vanilla Ice."));
    EXPECT_EQ(1, Util::getMaxWordFrequency("Hi this is me"));
    EXPECT_EQ(0, Util::getMaxWordFrequency(""));
}

TEST(StringOp, Max_Word_String_Equals_Test_VT) {

    string_vt sentence = {"This", "has", "max=2", "terms", "this", "lemon."};
    EXPECT_EQ(1, Util::getMaxWordFrequency(sentence));

    sentence = {"Ice", "Ice", "Baby,", "Vanilla", "Ice"};
    EXPECT_EQ(3, Util::getMaxWordFrequency(sentence));

    sentence = {"Hi", "this", "is", "me"};
    EXPECT_EQ(1, Util::getMaxWordFrequency(sentence));

    sentence = {};
    EXPECT_EQ(0, Util::getMaxWordFrequency(sentence));

    sentence = {""};
    EXPECT_EQ(0, Util::getMaxWordFrequency(sentence));
}