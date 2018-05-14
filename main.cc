#include "args.hh"
#include "document_manager.hh"
#include "index_manager.hh"
#include "inverted_index.hh"
#include "measure.hh"
#include "query_processing_engine.hh"
#include "random_projection.hh"
#include "src/evaluation.hh"
#include "src/types.hh"
#include "utility.hh"

#include <evaluation.hh>
#include <experimental/filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>
#include <word_embeddings.hh>
namespace fs = std::experimental::filesystem;

// insert everything here what is not actually meant to be in main
void test(const control_block_t& aControlBlock) {
    /* Example how to use Measurement class (also described in measure.hh) */

    // Measure lMeasure;
    // if (aControlBlock.measure()) { lMeasure.start(); }
    //// do processing
    // lMeasure.stop();
    // double lSeconds = lMeasure.mTotalTime();
    // std::cout << "This print message is just used to prevent unused variable warnings. " << lSeconds << std::endl;

    // DocumentManager& docManager = DocumentManager::getInstance();
    // std::cout << "This print message is just used to prevent unused variable warnings. " << docManager.getNoDocuments() << std::endl;

    // std::string text = "Let me split this into words";
    // std::vector<std::string> results;

    Evaluation::getInstance().start(kVANILLA, "Med1");

    std::this_thread::sleep_for(std::chrono::seconds(3));

    Evaluation::getInstance().stop();

    Evaluation::getInstance().start(kTIERED, "Med2");

    std::this_thread::sleep_for(std::chrono::seconds(4));

    Evaluation::getInstance().stop();

    Evaluation::getInstance().start(kCLUSTER, "Med3");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    Evaluation::getInstance().stop();

    Evaluation::getInstance().constructJSON();

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

void search(std::string query, size_t topK, IR_MODE mode, bool use_lsh) {
    QueryProcessingEngine& qpe = QueryProcessingEngine::getInstance();

    Measure lMeasureQuery;
    lMeasureQuery.start();
    std::vector<std::pair<size_t, float>> result = qpe.search(query, topK, mode, use_lsh);
    lMeasureQuery.stop();

    double lSecondsQuery = lMeasureQuery.mTotalTime();
    // std::cout << "Search took " << lSecondsQuery << " sec." << std::endl;

    using json = nlohmann::json;
    json json_result = json::array();

    for (size_t j = 0; j < result.size(); ++j) {

        Document& d = DocumentManager::getInstance().getDocument(result[j].first);
        json json_doc = json::object();
        json_doc["id"] = d.getDocID();
        json_doc["similarity"] = result[j].second;
        json_doc["content"] = Utility::StringOp::string_vt_2_str(d.getContent());
        json_result.push_back(json_doc);
    }

    std::cout << "[Your result]:" << json_result << std::endl;
}

void testNico() {
    const control_block_t& aControlBlock = {false, false, false, "./data/collection_test.docs", "./tests/_trace_test/", "", "./data/stopwords.large",
                                            0,     3,     1000};
    // assert(aNumTiers > 1);
    Measure lMeasure;
    lMeasure.start();
    DocumentManager& docManager = DocumentManager::getInstance();
    // docManager.init(aControlBlock);
    docManager.init(aControlBlock);

    doc_mt& docMap = docManager.getDocumentMap();

    IndexManager& imInstance = IndexManager::getInstance();
    imInstance.init(aControlBlock, docMap);
    lMeasure.stop();
    double lSeconds = lMeasure.mTotalTime();
    std::cout << "Index creation took " << lSeconds << " sec." << std::endl;
    const InvertedIndex& ii = imInstance.getInvertedIndex();
    const TieredIndex& ti = imInstance.getTieredIndex();
    std::cout << ii << std::endl;
    std::cout << ti << std::endl;

    std::cout << std::endl;
    Document& d = docManager.getDocument(0);
    Document& d2 = docManager.getDocument(1);
    Document& d3 = docManager.getDocument(2);

    /*std::cout << Utility::SimilarityMeasures::calcCosDist(d, d2) << std::endl;
    std::cout << Utility::SimilarityMeasures::calcCosDist(d, d3) << std::endl;

    std::cout << Utility::SimilarityMeasures::calcHammingDist(d.getRandProjVec(), d2.getRandProjVec()) << std::endl;
    std::cout << Utility::SimilarityMeasures::calcHammingDist(d.getRandProjVec(), d3.getRandProjVec()) << std::endl;*/

    QueryProcessingEngine::getInstance().init(aControlBlock);

    std::string qs = "Util";
    search(qs, 10, IR_MODE::kTIERED, false);

    // int count = 0;
    // std::cout << docManager.getDocument(2) << std::endl;
    // std::cout << "\"" << term << "\"" << imInstance.getInvertedIndex().getPostingList(term) << std::endl;
    /*for (const auto& [term, idf] : imInstance.getIdfMap()) {
        ++count;
        if (count > 100) return;
        std::cout << term << ": ";
        std::cout << idf << std::endl;
    }*/
    // search("why deep fried foods may cause cancer");
    // search("do cholesterol statin drugs cause breast cancer ?");
}

void testAlex(Args& lArgs) {

    /*Measure lMeasureIndexing;
    lMeasureIndexing.start();
    DocumentManager& docManager = DocumentManager::getInstance();
    docManager.init(aControlBlock);
    doc_mt& docMap = docManager.getDocumentMap();

    IndexManager& imInstance = IndexManager::getInstance();
    imInstance.init(aControlBlock, docMap);

    lMeasureIndexing.stop();
    double lSeconds = lMeasureIndexing.mTotalTime();
    std::cout << "Index creation took " << lSeconds << " sec." << std::endl;

    WordEmbeddings& wb = WordEmbeddings::getInstance();
    wb.init(aControlBlock, "./data/w2v/cb_hs_500_10.w2v");

    // create doc2vec model
    w2v::d2vModel_t d2vModel(wb.getw2v()->vectorSize());

    std::cout << docManager.getDocumentMap().size() << std::endl;

    {
        w2v::doc2vec_t doc2vec(wb.getw2v(), Utility::StringOp::string_vt_2_str(docManager.getDocument(1).getContent()));
        // add vector with ID = 1 to the model
        d2vModel.set(0, doc2vec);
    }

    {
        w2v::doc2vec_t doc2vec(wb.getw2v(), Utility::StringOp::string_vt_2_str(docManager.getDocument(2).getContent()));
        // add vector with ID = 1 to the model
        d2vModel.set(1, doc2vec);
    }

    {
        w2v::doc2vec_t doc2vec(wb.getw2v(), Utility::StringOp::string_vt_2_str(docManager.getDocument(3).getContent()));
        // add vector with ID = 1 to the model
        d2vModel.set(2, doc2vec);
    }

    {
        w2v::doc2vec_t doc2vec(wb.getw2v(), Utility::StringOp::string_vt_2_str(docManager.getDocument(4).getContent()));
        // add vector with ID = 1 to the model
        d2vModel.set(3, doc2vec);
    }

    w2v::doc2vec_t doc2vec(wb.getw2v(), "do cholesterol statin drugs cause breast cancer ?");

    // get nearest article IDs from the model
    std::vector<std::pair<std::size_t, float>> nearest;
    d2vModel.nearest(doc2vec, nearest, d2vModel.modelSize());

    // output result set
    for (auto const& i : nearest) {
        std::cout << i.first << ": " << i.second << std::endl;
    }*/

    //const control_block_t& aControlBlock = {false, false, false, "./data/collection_test.docs", "./tests/_trace_test/", "", "./data/stopwords.large",
    //                                        0,     10,    100};
     const control_block_t& aControlBlock = {false, false, false, "./data/collection_test.docs", "./tests/_trace_test/", "", lArgs.stopwordPath(), 0, lArgs.tiers(),
     lArgs.dimensions()};

    Measure lMeasureIndexing;
    lMeasureIndexing.start();

    DocumentManager& docManager = DocumentManager::getInstance();
    docManager.init(aControlBlock);

    doc_mt& docMap = docManager.getDocumentMap();

    IndexManager& imInstance = IndexManager::getInstance();
    imInstance.init(aControlBlock, docMap);

    const TieredIndex& ti = imInstance.getTieredIndex();
    std::cout << ti << std::endl;

    lMeasureIndexing.stop();
    double lSeconds = lMeasureIndexing.mTotalTime();
    // std::cout << "Index creation took " << lSeconds << " sec." << std::endl;

    QueryProcessingEngine::getInstance().init(aControlBlock);

    std::cout << "[Ready]" << std::endl;

    // search("Why breast cancer", 10, IR_MODE::kCLUSTER);

    while (true) {
        json j;
        std::cin >> j;
        search(j["query"].get<std::string>(), j["topK"].get<size_t>(), stringToMode(j["mode"].get<std::string>()), j["lsh"].get<bool>());
    }
}

/**
 * @brief Starts the program
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(const int argc, const char* argv[]) {
    // std::cout << "Test";
    // this is just a test, needs a proper implementation later on
    /*if (!Utility::StringOp::endsWith(fs::current_path().string(), "vector-space-retrieval")) {
        // todo: change error message
        std::cerr << "Incorrect execution path! Please start the executable from the path ending with 'vector-space-retrieval'" << std::endl;
        std::cout << "Current Working Directory: " << fs::current_path() << std::endl;
        return -1;
    }*/

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

    // THROW EXCEPTION if numtiers < 2
    const control_block_t lCB = {lArgs.trace(),    lArgs.measure(),      lArgs.plot(),    lArgs.collectionPath(), lArgs.tracePath(),
                                 lArgs.evalPath(), lArgs.stopwordPath(), lArgs.results(), lArgs.tiers(),          lArgs.dimensions()};

    Trace::getInstance().init(lCB);
    Evaluation::getInstance().init(lCB);
    // insert everything here what is not actually meant to be in main
    // test(lCB);
    // testNico();
    testAlex(lArgs);

    /*std::vector<sizet_vt> vecs;
    sizet_vt out;

    sizet_vt stvt1 = {9};
    sizet_vt stvt2 = {9};
  sizet_vt stvt3 = {9};

    vecs.push_back(stvt1);
    vecs.push_back(stvt2);
    vecs.push_back(stvt3);

    Utility::IR::mergePostingLists(vecs, out);

    std::cout << "result" << std::endl;
    for(auto& elem: out) {
      std::cout << elem << std::endl;
    }*/

    return 0;
}
