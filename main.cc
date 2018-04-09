//
// Created by Alexander Weiß on 30.03.18.
//

#include "args.hh"
#include "measure.hh"
#include "random_projection.hh"
#include "utility.hh"

#include <iostream>
#include <query_processing_engine.hh>
#include <vector>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

/**
 * @brief Starts everything
 *
 * @param argc
 * @param argv
 * @return
 */
int main(const int argc, const char* argv[]) {
    //this is just a test, needs a proper implementation later on
    if(!Utility::StringOp::endsWith(fs::current_path().string(), "vector-space-retrieval"))
    {
        //todo: change error message
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

    /* Example how to use Measurement class (also described in measure.hh) */

    Measure lMeasure;
    if (lArgs.measure()) { lMeasure.start(); }
    // do processing
    lMeasure.stop();
    double lSeconds = lMeasure.mTotalTime();
    std::cout << "This print message is just used to prevent unused variable warnings. " << lSeconds << std::endl;

    DocumentManager& docManager = DocumentManager::getInstance(); 
    std::cout << "This print message is just used to prevent unused variable warnings. " << docManager.getCurrID() << std::endl;


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

    random_projection::projectMatrix();

    //QueryProcessingEngine::getInstance().cosineScore("Documenting transportation is such a great fundamental human being being being being", 10);
    //

    return 0;
}
