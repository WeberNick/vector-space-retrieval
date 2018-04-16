#include "document.hh"
#include "document_manager.hh"
#include "index_manager.hh"
#include "types.hh"
#include "utility.hh"
#include "gtest/gtest.h"


/**
 * @brief
 *
 */
class DocumentTest : public testing::Test {
  protected:
    virtual void SetUp() {
        docMan = &(DocumentManager::getInstance());
        const control_block_t lControlBlock = { false, false, false, "", 0, 0, 0 };
        docMan->init(lControlBlock, "./data/collection_test_mwe.docs");
        docMap = &(docMan->getDocumentMap());

        indexManager = &(IndexManager::getInstance());
        indexManager->init(lControlBlock, *docMap);
    }

    DocumentManager* docMan;
    doc_mt* docMap;
    IndexManager* indexManager;
};

/**
 * @brief Content of collection_test_mwe.docs:
 *
 * D-1 ~ today sun jon lemon food
 * D-2 ~ today we go ski outside food
 * D-3 ~ today tomorrow no go sun lemon lemon
 */

TEST_F(DocumentTest, Tf_Equals_Test) {
    float tf_jon = docMan->getDocument(1).getTf("jon");
    float tf_sun = docMan->getDocument(3).getTf("sun");
    float tf_lemon = docMan->getDocument(3).getTf("lemon");
    float tf_tomorrow = docMan->getDocument(3).getTf("tomorrow");
    EXPECT_EQ(tf_jon, 1);
    EXPECT_EQ(tf_sun, Utility::IR::calcTf(1, Utility::StringOp::getMaxWordFrequency(docMan->getDocument(3).getContent())));
    EXPECT_EQ(tf_lemon, 1);
    EXPECT_EQ(tf_tomorrow, Utility::IR::calcTf(2, Utility::StringOp::getMaxWordFrequency(docMan->getDocument(3).getContent())));
}

TEST_F(DocumentTest, Idf_Equals_Test) {

    // does not work. Why?
    const float idf_today2 = indexManager->getIdf("today");
    float idf_today = indexManager->getInvertedIndex().getPostingList("today").getIdf();
    float idf_food = indexManager->getInvertedIndex().getPostingList("food").getIdf();
    EXPECT_EQ(idf_today, 0);
    EXPECT_EQ(idf_food, Utility::IR::calcIdf(docMap->size(), 2));
}