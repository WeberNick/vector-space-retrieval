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

    std::vector<float> d1 = { 0.17, 0.21, 0.35, 0.44, 0.49, 0.39, 0.09, 0.07, 0.37, 0.24 };
    std::vector<float> d2 = { 0.49, 0.48, 0.44, 0.09, 0.24, 0.2, 0.41, 0.16, 0.1, 0.15 };
    std::vector<float> d3 = { 0.41, 0.36, 0.27, 0.19, 0.15, 0.42, 0.23, 0.42, 0.02, 0.42 };
    std::vector<float> d4 = { 0.31, 0.41, 0.21, 0.19, 0.47, 0.28, 0.21, 0.39, 0.16, 0.38 };
    std::vector<float> d5 = { 0.46, 0.12, 0.21, 0.25, 0.38, 0.38, 0.46, 0.23, 0.31, 0.14 };
    std::vector<float> d6 = { 0.13, 0.33, 0.28, 0.42, 0.07, 0.13, 0.58, 0.15, 0.0, 0.49 };
    std::vector<float> d7 = { 0.21, 0.09, 0.07, 0.09, 0.3, 0.54, 0.24, 0.43, 0.51, 0.21 };
    std::vector<float> d8 = { 0.18, 0.39, 0.42, 0.05, 0.41, 0.1, 0.52, 0.12, 0.14, 0.38 };
    std::vector<float> d9 = { 0.4, 0.51, 0.01, 0.1, 0.12, 0.22, 0.26, 0.34, 0.42, 0.38 };

    std::vector<std::vector<float>> randomVectors = { { 0.33, 0.33, 0.42, 0.12, 0.2, 0.34, 0.58, 0.19, 0.07, 0.24 },
                                                      { 0.29, 0.16, 0.38, 0.48, 0.43, 0.11, 0.12, 0.33, 0.03, 0.44 },
                                                      { 0.01, 0.17, 0.11, 0.27, 0.23, 0.37, 0.35, 0.48, 0.54, 0.24 },
                                                      { 0.09, 0.05, 0.39, 0.25, 0.45, 0.48, 0.04, 0.45, 0.35, 0.12 },
                                                      { 0.13, 0.17, 0.4, 0.4, 0.07, 0.4, 0.35, 0.39, 0.44, 0.06 } };

    RandomProjection::getInstance().init(aControlBlock, 10);
    /*RandomProjection::getInstance().setRandomVectors(randomVectors);

    boost::dynamic_bitset<> doc_1_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d1, hashExercise2Task2);
    std::cout << "doc_1 after hashing: " << doc_1_proj << std::endl;

    boost::dynamic_bitset<> doc_2_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d2, hashExercise2Task2);
    std::cout << "doc_2 after hashing: " << doc_2_proj << std::endl;

    boost::dynamic_bitset<> doc_3_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d3, hashExercise2Task2);
    std::cout << "doc_3 after hashing: " << doc_3_proj << std::endl;

    boost::dynamic_bitset<> doc_4_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d4, hashExercise2Task2);
    std::cout << "doc_4 after hashing: " << doc_4_proj << std::endl;

    boost::dynamic_bitset<> doc_5_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d5, hashExercise2Task2);
    std::cout << "doc_5 after hashing: " << doc_5_proj << std::endl;

    boost::dynamic_bitset<> doc_6_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d6, hashExercise2Task2);
    std::cout << "doc_6 after hashing: " << doc_6_proj << std::endl;

    boost::dynamic_bitset<> doc_7_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d7, hashExercise2Task2);
    std::cout << "doc_7 after hashing: " << doc_7_proj << std::endl;

    boost::dynamic_bitset<> doc_8_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d8, hashExercise2Task2);
    std::cout << "doc_8 after hashing: " << doc_8_proj << std::endl;

    boost::dynamic_bitset<> doc_9_proj = RandomProjection::getInstance().localitySensitiveHashProjection(d9, hashExercise2Task2);
    std::cout << "doc_9 after hashing: " << doc_9_proj << std::endl;

    boost::dynamic_bitset<> l1(std::string("01100"));
    boost::dynamic_bitset<> l2(std::string("01110"));
    boost::dynamic_bitset<> l3(std::string("00000"));

    std::vector<boost::dynamic_bitset<>> projections;
    projections.push_back(doc_1_proj);
    projections.push_back(doc_2_proj);
    projections.push_back(doc_3_proj);
    projections.push_back(doc_4_proj);
    projections.push_back(doc_5_proj);
    projections.push_back(doc_6_proj);
    projections.push_back(doc_7_proj);
    projections.push_back(doc_8_proj);
    projections.push_back(doc_9_proj);

    for (size_t j = 0; j < projections.size(); ++j) {
        std::cout << "doc_" << j + 1 << std::endl;
        std::cout << "Similarity with l1: " << Utility::SimilarityMeasures::calcHammingSim(projections[j], l1) << std::endl;
        std::cout << "Similarity with l2: " << Utility::SimilarityMeasures::calcHammingSim(projections[j], l2) << std::endl;
        std::cout << "Similarity with l3: " << Utility::SimilarityMeasures::calcHammingSim(projections[j], l3) << std::endl;
    }

    std::vector<float> q = { 0.15, 0.39, 0.36, 0.25, 0.36, 0.15, 0.52, 0.37, 0.08, 0.27 };
    boost::dynamic_bitset<> q_proj = RandomProjection::getInstance().localitySensitiveHashProjection(q, hashExercise2Task2);
    std::cout << "q after hashing: " << q_proj << std::endl;

    std::cout << "Similarity of q with l1: " << Utility::SimilarityMeasures::calcHammingSim(q_proj, l1) << std::endl;
    std::cout << "Similarity of q with l2: " << Utility::SimilarityMeasures::calcHammingSim(q_proj, l2) << std::endl;
    std::cout << "Similarity of q with l3: " << Utility::SimilarityMeasures::calcHammingSim(q_proj, l3) << std::endl;

    std::string test = "This is a test test Test";

    std::cout << Utility::StringOp::countWordInString(test, "test", false) << std::endl;

    // std::cout << Utility::SimilarityMeasures::calcCosSim(q, doc) << std::endl;
    // std::cout << (1 - Utility::SimilarityMeasures::calcCosSim(q, doc)) << std::endl;

    /*std::cout << "Angular sim (a-b) before locality hashing: " << Utility::SimilarityMeasures::calcAngularSimilarity(doc_a, doc_b) << std::endl;

    std::cout << "Angular sim (a-c) before locality hashing: " << Utility::SimilarityMeasures::calcAngularSimilarity(doc_a, doc_c) << std::endl;

    boost::dynamic_bitset<> doc_a_proj = RandomProjection::getInstance().localitySensitiveHashProjection(doc_a, hash);
    boost::dynamic_bitset<> doc_b_proj = RandomProjection::getInstance().localitySensitiveHashProjection(doc_b, hash);
    boost::dynamic_bitset<> doc_c_proj = RandomProjection::getInstance().localitySensitiveHashProjection(doc_c, hash);

    std::cout << "doc_a after hashing: " << doc_a_proj << std::endl;
    std::cout << "doc_b after hashing: " << doc_b_proj << std::endl;
    std::cout << "doc_c after hashing: " << doc_c_proj << std::endl;

    std::cout << std::endl;

    boost::dynamic_bitset<> XOR = doc_a_proj ^ doc_b_proj;
    std::cout << XOR << std::endl;
    std::cout << XOR.count() << std::endl;

    boost::dynamic_bitset<> XOR2 = doc_a_proj ^ doc_c_proj;
    std::cout << XOR2 << std::endl;
    std::cout << XOR2.count() << std::endl;

    double sim = (static_cast<double>(XOR.size()) - static_cast<double>(XOR.count())) / XOR.size();
    std::cout << sim << std::endl;

    double sim2 = (static_cast<double>(XOR2.size()) - static_cast<double>(XOR2.count())) / XOR2.size();
    std::cout << sim2 << std::endl;

    // std::cout << "Hamming distance doc_a_proj and doc_b_proj" << Utility::SimilarityMeasures::calcHammingDist(doc_a_proj, doc_b_proj) << std::endl;
    // std::cout << "Angular sim after locality hashing: " << Utility::SimilarityMeasures::calcAngSimHamming(doc_a_proj, doc_b_proj) << std::endl;

    // size_t XOR = doc_a_proj ^ doc_b_proj;

    // double simAfterProj = (doc_a.size() - nnz(XOR) / doc_a.size());

    // std::cout << "Angular sim after locality hashing: " << simAfterProj << std::endl;
    // std::cout << "Angular sim after locality hashing: " << Utility::SimilarityMeasures::calcCosSimHamming(doc_a_proj, doc_b_proj) << std::endl;

    // std::vector<std::string> sentence = { "Hi", "how", "are", "you", "today,", "you", "you", "you", "look", "look", "look", "gorgeous" };
    // std::cout << Utility::StringOp::getMaxWordFrequency(sentence) << std::endl;

    // QueryProcessingEngine::getInstance().cosineScore("Documenting transportation is such a great fundamental human being being being being", 10);
    //*/
}

void testNico(const control_block_t& aControlBlock) {
    Measure lMeasure;
    lMeasure.start();
    DocumentManager& docManager = DocumentManager::getInstance();
    docManager.init(aControlBlock);
    std::cout << "This print message is just used to prevent unused variable warnings. " << docManager.getNoDocuments() << std::endl;

    IndexManager& imInstance = IndexManager::getInstance();
    lMeasure.stop();
    double lSeconds = lMeasure.mTotalTime();
    std::cout << "Index creation took " << lSeconds << " sec." << std::endl;
    imInstance.init(aControlBlock, docManager.getDocumentMap());
    std::string term = "sabdariffa";
    std::cout << "\"" << term << "\"" << imInstance.getInvertedIndex().getPostingList(term);
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

    const control_block_t lCB = { lArgs.trace(), lArgs.measure(), lArgs.print(), lArgs.path(), lArgs.results(), lArgs.tiers(), lArgs.dimensions() };

    // insert everything here what is not actually meant to be in main
    // test(lCB);
    testNico(lCB);

    return 0;
}