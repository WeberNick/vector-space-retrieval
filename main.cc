#include "args.hh"
#include "document_manager.hh"
#include "index_manager.hh"
#include "inverted_index.hh"
#include "measure.hh"
#include "query_processing_engine.hh"
#include "random_projection.hh"
#include "utility.hh"

#include <experimental/filesystem>
#include <iostream>
#include <vector>
namespace fs = std::experimental::filesystem;

bool hash(std::vector<float>& origVec, std::vector<float>& randVec) {

    if (origVec.size() != randVec.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors are not the same size");

    double dot = Utility::scalar_product(origVec, randVec);
    if (dot >= 0) {
        return 1;
    } else {
        return 0;
    }
}

unsigned int hashExercise2Task2(std::vector<float>& origVec, std::vector<float>& randVec) {

    if (origVec.size() != randVec.size()) throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors are not the same size");

    double dot = Utility::scalar_product(origVec, randVec);
    if (dot > 0.75) {
        return 1;
    } else {
        return 0;
    }
}

// insert everything here what is not actually meant to be in main
void test(const control_block_t& aControlBlock) {
    /* Example how to use Measurement class (also described in measure.hh) */

    Measure lMeasure;
    if (aControlBlock.measure()) { lMeasure.start(); }
    // do processing
    lMeasure.stop();
    double lSeconds = lMeasure.mTotalTime();
    std::cout << "This print message is just used to prevent unused variable warnings. " << lSeconds << std::endl;

    DocumentManager& docManager = DocumentManager::getInstance();
    std::cout << "This print message is just used to prevent unused variable warnings. " << docManager.getNoDocuments() << std::endl;

    std::string text = "Let me split this into words";
    std::vector<std::string> results;

    /*Utility::StringOp::splitString(text, ' ', results);
    for (auto t : results) {
        std::cout << "Word: " << t << std::endl;
    }*/

    /*std::vector<double> doc_a = { 1, 3, 5, 8, 100, 100 };
    std::vector<double> doc_b = { 2, 4, 5, 1, 2, 0 };

    std::cout << similarity_measures::tf_idf(5.0, 10.0) << std::endl;

    double euclid_sim = similarity_measures::calcEuclideanDistanceNormalized(doc_a, doc_b);
    double cos_sim = similarity_measures::calcCosineSimilarity(doc_a, doc_b);

    random_projection::createRandomMatrix(0, 1, true, 0.1, "gaussian");

    std::cout << euclid_sim << std::endl;
    std::cout << cos_sim << std::endl;
    int dimension = 1;
    std::cout << random_projection::dimension(dimension, 0.1) << std::endl;
    std::cout << "Hello CMake" << std::endl;*/

    // random_projection::createRandomMatrix(100, 500, true, 0.1, "gaussian");

    /*std::vector<float> doc_a = Utility::generateRandomVector(10, 0, 1);
    std::vector<float> doc_b = Utility::generateRandomVector(10, 0, 1);
    std::vector<float> doc_c = Utility::generateRandomVector(10, 0, 1);

    RandomProjection::getInstance().setDimensions(5);
    RandomProjection::getInstance().setOrigVectorSize(doc_a.size());
    RandomProjection::getInstance().initRandomVectors();

    for (int j = 0; j < doc_a.size(); ++j) {
        std::cout << doc_a[j] << ",";
    }
    std::cout << std::endl;
    for (int j = 0; j < doc_b.size(); ++j) {
        std::cout << doc_b[j] << ",";
    }
    std::cout << std::endl;
    for (int j = 0; j < doc_c.size(); ++j) {
        std::cout << doc_c[j] << ",";
    }
    std::cout << std::endl;
    std::cout << "Dimension inside RandomProjection = " << RandomProjection::getInstance().getDimensions() << std::endl;

    for (auto& elem : RandomProjection::getInstance().getRandomVectors()) {
        for (auto& dimValue : elem) {
            std::cout << dimValue << ",";
        }
        std::cout << std::endl;
    }*/
}

void testNico(const control_block_t& aControlBlock) {
    Measure lMeasure;
    lMeasure.start();
    DocumentManager& docManager = DocumentManager::getInstance();
    // docManager.init(aControlBlock, "./data/collection_test_mwe.docs");
    docManager.init(aControlBlock, "./data/collection.docs");
    doc_mt& docMap = docManager.getDocumentMap();

    IndexManager& imInstance = IndexManager::getInstance();
    imInstance.init(aControlBlock, docMap);
    // std::string term = "sabdariffa";
    std::string term = "today";

    lMeasure.stop();
    double lSeconds = lMeasure.mTotalTime();
    std::cout << "Index creation took " << lSeconds << " sec." << std::endl;
    // int count = 0;
    std::cout << docManager.getDocument(2) << std::endl;
    std::cout << "\"" << term << "\"" << imInstance.getInvertedIndex().getPostingList(term) << std::endl;
    /*for (const auto& [term, idf] : imInstance.getIdfMap()) {
        ++count;
        if (count > 100) return;
        std::cout << term << ": ";
        std::cout << idf << std::endl;
    }*/
    // testSearch("why deep fried foods may cause cancer");
    // testSearch("do cholesterol statin drugs cause breast cancer ?");
}

void testSearch(std::string query) {
    QueryProcessingEngine& qpe = QueryProcessingEngine::getInstance();

    // Remove stopwords
    Utility::IR::removeStopword(query, qpe.getStopwordlist());

    // Trim whitespaces
    Utility::StringOp::trim(query);

    string_vt proc_query;

    // Split string by whitespaces
    Utility::StringOp::splitString(query, ' ', proc_query);

    // Remove eventually empty strings from the query term vector
    Utility::StringOp::removeEmptyStringsFromVec(proc_query);

    // Create doc from query vector
    Document doc("0", proc_query);

    Measure lMeasureQuery;
    lMeasureQuery.start();
    std::vector<size_t> result = qpe.search(&doc, 50);
    lMeasureQuery.stop();
    double lSecondsQuery = lMeasureQuery.mTotalTime();
    std::cout << "Search took " << lSecondsQuery << " sec." << std::endl;

    for (size_t j = 0; j < result.size(); ++j) {
        std::cout << "(" << j << ".) " << DocumentManager::getInstance().getDocument(result[j]).getDocID() << ": ";

        for (auto& elem : DocumentManager::getInstance().getDocument(result[j]).getContent()) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

void testAlex(const control_block_t& aControlBlock) {
    Measure lMeasureIndexing;
    lMeasureIndexing.start();
    DocumentManager& docManager = DocumentManager::getInstance();
    docManager.init(aControlBlock, "./data/collection.docs");
    doc_mt& docMap = docManager.getDocumentMap();

    IndexManager& imInstance = IndexManager::getInstance();
    imInstance.init(aControlBlock, docMap);

    lMeasureIndexing.stop();
    double lSeconds = lMeasureIndexing.mTotalTime();
    std::cout << "Index creation took " << lSeconds << " sec." << std::endl;

    QueryProcessingEngine::getInstance().init(aControlBlock);

    testSearch("why deep fried foods may cause cancer");
    testSearch("do cholesterol statin drugs cause breast cancer ?");
}

/**
 * @brief Starts the program
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(const int argc, const char* argv[]) {
    // this is just a test, needs a proper implementation later on
    if (!Utility::StringOp::endsWith(fs::current_path().string(), "vector-space-retrieval")) {
        // todo: change error message
        std::cerr << "Incorrect execution path! Please start the executable from the path ending with 'vector-space-retrieval'" << std::endl;
        std::cout << "Current Working Directory: " << fs::current_path() << std::endl;
        return -1;
    }

    /* How to use class Args is described in args.hh */
    Args lArgs;
    argdesc_vt lArgDesc;
    construct_arg_desc(lArgDesc);

    if (!parse_args<Args>(1, argc, argv, lArgDesc, lArgs)) {
        std::cerr << "error while parsing arguments." << std::endl;
        return -1;
    }

    if (lArgs.help()) {
        print_usage(std::cout, argv[0], lArgDesc);
        return 0;
    }

    const control_block_t lCB = { 
        lArgs.trace(), 
        lArgs.measure(), 
        lArgs.plot(), 
        lArgs.collectionPath(), 
        lArgs.tracePath(),
        lArgs.evalPath(),
        lArgs.results(), 
        lArgs.tiers(), 
        lArgs.dimensions() };

    // insert everything here what is not actually meant to be in main
    // test(lCB);
    testNico(lCB);
    // testAlex(lCB);
    return 0;
}
