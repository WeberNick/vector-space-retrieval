#include "ir_util.hh"
#include "gtest/gtest.h"

TEST(IR, OrPostingLists_Equals_Test) {

    const sizet_vt& vec_a = {1, 2, 4};
    const sizet_vt& vec_b = {5, 4};
    std::vector<sizet_vt> vecs = {vec_a, vec_b};
    const sizet_vt& result = {1, 2, 4, 5};
    sizet_vt out;
    Util::orPostingLists(vecs, out);
    EXPECT_EQ(result, out);
}