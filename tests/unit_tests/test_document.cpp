#include "document.hh"
#include "document_manager.hh"
#include "index_manager.hh"
#include "types.hh"
#include "string_util.hh"
#include "ir_util.hh"
#include "gtest/gtest.h"

/**
 * @brief DocumentTest Mock class for testing functionalities of the Document Manager
 *
 */
class DocumentTest : public testing::Test {
  protected:
    virtual void SetUp() {
        const control_block_t lControlBlock = { false, false, false, "./tests/data/collection_test_mwe.docs", "./data/" , "./data/s-3.qrel", "./data/stopwords.large", "./data/w2v/glove.6B.300d.txt", "./", "./", 10, 10, 100 };
        Trace::getInstance().init(lControlBlock);
        RandomProjection::getInstance().reset(0);

        docMan = &(DocumentManager::getInstance());
        docMan->init(lControlBlock);
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
 * D-1 ~ today sun jon lemon food hi
 * D-2 ~ today we go ski outside food hi go
 * D-3 ~ today tomorrow tomorrow no go sun lemon lemon lemon
 */

TEST_F(DocumentTest, Tf_Equals_Test) {

    float tf_jon = docMan->getDocument(0).getTf("jon");
    float tf_sun = docMan->getDocument(2).getTf("sun");
    float tf_lemon = docMan->getDocument(2).getTf("lemon");
    float tf_tomorrow = docMan->getDocument(2).getTf("tomorrow");
    EXPECT_EQ(tf_jon, 1);
    EXPECT_EQ(tf_sun, Util::calcTf(1, Util::getMaxWordFrequency(docMan->getDocument(2).getContent())));
    EXPECT_EQ(tf_lemon, 1);
    EXPECT_EQ(tf_tomorrow, Util::calcTf(2, Util::getMaxWordFrequency(docMan->getDocument(2).getContent())));
}

TEST_F(DocumentTest, Idf_Equals_Test) {

    const float idf_today = indexManager->getIdf("today");
    //float idf_today = indexManager->getInvertedIndex().getPostingList("today").getIdf();
    float idf_food = indexManager->getInvertedIndex().getPostingList("food").getIdf();
    EXPECT_EQ(idf_today, 0);
    EXPECT_EQ(idf_food, Util::calcIdf(docMap->size(), 2));
}
