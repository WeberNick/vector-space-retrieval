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

/**
 * @brief Starts everything
 *
 * @param argc
 * @param argv
 * @return
 */
int main(const int argc, const char* argv[]) {
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

    DocumentManager::createInstance(lArgs.path());
    //Nick: For ease of use, I changed the return of getInstance to a reference (see document_manager.hh)
    DocumentManager& docManager = DocumentManager::getInstance(); 

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

    std::cout << "hi" << std::endl;
    random_projection::projectMatrix();

    // std::cout << QueryProcessingEngine::Instance().search("Documenting transportation is such a great fundamental human being") << std::endl;

    return 0;
}
