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
    if (origVec.size() != randVec.size())
        throw VectorException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Vectors are not the same size");

    double dot = Utility::scalar_product(origVec, randVec);
    if (dot >= 0) {
        return true;
    } else {
        return false;
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

    Utility::StringOp::splitString(text, ' ', results);
    for (auto t : results) {
        std::cout << "Word: " << t << std::endl;
    }

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

    std::vector<float> doc_a = Utility::generateRandomVector(500, 0, 10000);
    std::vector<float> doc_b = Utility::generateRandomVector(500, 0, 10000);

    RandomProjection::getInstance().setDimensions(10);
    RandomProjection::getInstance().setOrigVectorSize(doc_a.size());
    RandomProjection::getInstance().initRandomVectors();

    std::cout << "Dimension inside RandomProjection = " << RandomProjection::getInstance().getDimensions() << std::endl;

    for (auto& elem : RandomProjection::getInstance().getRandomVectors()) {
        for (auto& dimValue : elem) {
            std::cout << dimValue << ",";
        }
        std::cout << std::endl;
    }

    std::cout << "Angular sim before locality hashing: " << Utility::SimilarityMeasures::calcAngularSimilarity(doc_a, doc_b) << std::endl;

    // std::vector<unsigned int> doc_a_proj = RandomProjection::getInstance().localiltySensitveHashProjection(doc_a, hash);
    // std::vector<unsigned int> doc_b_proj = RandomProjection::getInstance().localiltySensitveHashProjection(doc_b, hash);

    // size_t doc_a_proj = RandomProjection::getInstance().localiltySensitveHashProjection2(doc_a);
    // size_t doc_b_proj = RandomProjection::getInstance().localiltySensitveHashProjection2(doc_b);

    std::vector<bool> doc_a_proj = RandomProjection::getInstance().localitySensitiveHashProjection(doc_a, hash);
    std::vector<bool> doc_b_proj = RandomProjection::getInstance().localitySensitiveHashProjection(doc_b, hash);

    std::cout << "doc_a after hashing" << std::endl;

    for (size_t i = 0; i < doc_a_proj.size(); ++i) {
        std::cout << doc_a_proj[i] << ",";
    }

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "doc_b after hashing" << std::endl;
    for (size_t i = 0; i < doc_b_proj.size(); ++i) {
        std::cout << doc_b_proj[i] << ",";
    }
    std::cout << std::endl;

    std::cout << "Hamming distance doc_a_proj and doc_b_proj" << Utility::SimilarityMeasures::calcHammingDist(doc_a_proj, doc_b_proj) << std::endl;
    std::cout << "Angular sim after locality hashing: " << Utility::SimilarityMeasures::calcAngSimHamming(doc_a_proj, doc_b_proj) << std::endl;

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

    const control_block_t lCB = { 
        lArgs.trace(),
        lArgs.measure(),
        lArgs.print(),
        lArgs.path(),
        lArgs.results(),
        lArgs.tiers(),
        lArgs.dimensions()
    };

    // insert everything here what is not actually meant to be in main
    // test(lCB);
    testNico(lCB);

    return 0;
}