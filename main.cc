#include "src/args.hh"
#include "src/document_manager.hh"
#include "src/index_manager.hh"
#include "src/query_manager.hh"
#include "src/inverted_index.hh"
#include "src/measure.hh"
#include "src/query_execution_engine.hh"
#include "src/random_projection.hh"
#include "src/evaluation.hh"
#include "src/types.hh"
#include "src/word_embeddings.hh"
#include "src/string_util.hh"

#include <experimental/filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>
namespace fs = std::experimental::filesystem;

// insert everything here what is not actually meant to be in main
void test(const control_block_t& aControlBlock) {
    /* Example how to use Measurement class (also described in measure.hh) */

    Evaluation& e = Evaluation::getInstance();

    e.start(kVANILLA, "Med1");

    std::this_thread::sleep_for(std::chrono::seconds(3));

    e.stop();

    e.start(kTIERED, "Med2");

    std::this_thread::sleep_for(std::chrono::seconds(4));

    e.stop();

    e.start(kCLUSTER, "Med3");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    e.stop();

    str_set set = {"Med-1", "Med-2", "Med-3"};
    e.constructJSON(set);
}

void search(std::string query, size_t topK, IR_MODE mode) {
    QueryExecutionEngine& qpe = QueryExecutionEngine::getInstance();

    Measure lMeasureQuery;
    lMeasureQuery.start();
    std::vector<std::pair<size_t, float>> result = qpe.search(query, topK, mode);
    lMeasureQuery.stop();

    double lSecondsQuery = lMeasureQuery.mTotalTime();
    // std::cout << "Search took " << lSecondsQuery << " sec." << std::endl;

    using json = nlohmann::json;
    json json_result = json::array();

    for (size_t j = 0; j < result.size(); ++j) {

        const Document& d = DocumentManager::getInstance().getDocument(result[j].first);
        json json_doc = json::object();
        json_doc["id"] = d.getDocID();
        json_doc["similarity"] = result[j].second;
        json_doc["content"] = Util::string_vt_2_str(d.getContent());
        json_result.push_back(json_doc);
    }

    std::cout << "[Your result]:" << json_result << std::endl;
}

void testNico() {

    const control_block_t& aControlBlock = {false, false, "./data/collection_test.docs", "", "", "./data/stopwords.large", "", "./tests/_trace_test/", "", 0,
                                            3,     1000};
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
    const Document& d = docManager.getDocument(0);
    const Document& d2 = docManager.getDocument(1);
    const Document& d3 = docManager.getDocument(2);

    QueryExecutionEngine::getInstance().init(aControlBlock);

    std::string qs = "Util";
    search(qs, 10, IR_MODE::kTIERED);

}

void testAlex(const control_block_t& aControlBlock) {

    /*Measure lMeasureIndexing;
    lMeasureIndexing.start();

    DocumentManager& docManager = DocumentManager::getInstance();
    docManager.init(aControlBlock);

    doc_mt& docMap = docManager.getDocumentMap();

    IndexManager& imInstance = IndexManager::getInstance();
    imInstance.init(aControlBlock, docMap);

    std::cout << "after index manager init" << std::endl;

    lMeasureIndexing.stop();
    double lSeconds = lMeasureIndexing.mTotalTime();
    std::cout << "Index creation took " << lSeconds << " sec." << std::endl;

    std::cout << "number of embeddings: " << imInstance.getWordEmbeddingsIndex().getNoWordEmbeddings() << std::endl;

    float_vt result;
    result.resize(300);
    string_vt content = {"the", "to"};

    imInstance.getWordEmbeddingsIndex().calcWordEmbeddingsVector(content, result);


    std::cout << result.size() << std::endl;

    for (auto& elem : result) {
        std::cout << elem << ",";
    for(auto& elem: docManager.getDocument("MED-241").getWordEmbeddingsVector()) {
        std::cout << elem  << ",";
    }
    std::cout << std::endl;

    /*std::string word = "the";

    for(auto& elem: imInstance.getWordEmbeddingsIndex().getWordEmbeddings(word)) {
        std::cout << elem  << ",";
    }
    std::cout << std::endl;

    std::cout << imInstance.getWordEmbeddingsIndex().getWordEmbeddings(word).size() << std::endl;*/

    //

    /*QueryExecutionEngine::getInstance().init(aControlBlock);

    std::cout << "[Ready]" << std::endl;

    // search("Why breast cancer", 10, IR_MODE::kCLUSTER);

    while (true) {
        json j;
        std::cin >> j;
        search(j["query"].get<std::string>(), j["topK"].get<size_t>(), stringToMode(j["mode"].get<std::string>()), j["lsh"].get<bool>());
    }*/
}

void testEval(const control_block_t& aControlBlock) {
    DocumentManager& docManager = DocumentManager::getInstance();
    docManager.init(aControlBlock);
    std::cout << "Document Manager initialized" << std::endl;

    IndexManager& imInstance = IndexManager::getInstance();
    imInstance.init(aControlBlock, docManager.getDocumentMap());
    std::cout << "Index Manager initialized" << std::endl;

    QueryManager& queryManager = QueryManager::getInstance();
    queryManager.init(aControlBlock);
    std::cout << "Query Manager initialized" << std::endl;

    QueryExecutionEngine& qpe = QueryExecutionEngine::getInstance();
    qpe.init(aControlBlock);
    std::cout << "Query Execution Engine initialized" << std::endl;

    Evaluation& e = Evaluation::getInstance();
    e.init(aControlBlock);
    std::cout << "Evaluation initialized" << std::endl;

    str_set queryNamesSet;

    std::cout << "Start eval " << std::endl;
    // hier kommt die for schleife über die enums type

    QUERY_TYPE type = QUERY_TYPE::kTITLES;

    const Document& query = queryManager.getQuery(type, "PLAIN-3448");
    
    /**
     * Type: 2Mode: Cluster_W2VQueryId: PLAIN-3448
        Searching in mode: Cluster_W2V
        Searching for: pesticid rins
        Returning results
        id: 0 sim: nan
        id: 1 sim: nan
        id: 2 sim: nan
        id: 3 sim: nan
        id: 4 sim: nan
        id: 5 sim: nan
        id: 6 sim: nan
        id: 7 sim: nan
        id: 8 sim: nan
        id: 9 sim: nan
     * 
     */

    std::vector<std::pair<size_t, float>> result = qpe.search(const_cast<Document&>(query), 30, IR_MODE::kCLUSTER_W2V);

    for (int j = 0; j < kNumberOfModes; ++j) {
        IR_MODE mode = static_cast<IR_MODE>(j);

        std::cout << "Mode " << modeToString(mode) << ":" << j << std::endl;

        auto& queryForType = QueryManager::getInstance().getQueryMap(type);//DocumentManager::getInstance().getQueriesForType(type);

        std::cout << "queries for types geholt: " << queryForType.size() << std::endl;

        for (auto& [query_id, query] : queryForType) {
            std::cout << "Type: " << type << "Mode: " << modeToString(mode) << "QueryId: " << query.getDocID() << std::endl;
            queryNamesSet.insert(query.getDocID());
            e.start(mode, query.getDocID());
            std::vector<std::pair<size_t, float>> result = qpe.search(query, 30, mode);
            std::cout << "after result" << std::endl;
            e.stop();
            std::cout << "after stop" << std::endl;
            e.evalIR(mode, query.getDocID(), result);
            std::cout << "after eval ir" << std::endl;
        }
    }
    e.constructJSON(queryNamesSet);
}

/**
 * @brief Starts the program
 *
 * @param argc
 * @param argv
 * @return int
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


    if(!Util::endsWith(fs::current_path().string(), "vector-space-retrieval")) 
    {
        std::cerr << " ## Warning : Programm was not executed from the recommended directory " 
            << "'vector-space-retrieval'. Make sure to provide all necessary file "
            << "paths as command line argument. For further information, start the executable with '--help'."
            << "\n            ## Current Working Directory: " << fs::current_path() << std::endl;
    }

   if(!(fs::exists(lArgs.collectionPath())))
   {
       std::cerr << "Given path to the data collection is invalid." << std::endl;
       return -1;
   }

   if(!fs::exists(lArgs.queryPath()))
   {
       std::cerr << "Given path to the query collection is invalid." << std::endl;
       return -1;
   }

   if(!fs::exists(lArgs.relevanceScoresPath()))
   {
       std::cerr << "Given path to the relevance scores is invalid." << std::endl;
       return -1;
   }

   if(!fs::exists(lArgs.wordEmbeddingsPath()))
   {
       std::cerr << "Given path to the word embeddings is invalid." << std::endl;
       return -1;
   }

   if(!fs::exists(lArgs.stopwordPath()))
   {
       std::cerr << "Given path to the stopwords file is invalid." << std::endl;
       return -1;
   }

   if(!fs::exists(lArgs.evalPath()))
   {
       std::cerr << "The path where to store the evaluation is invalid." << std::endl;
       return -1;
   }

   if(lArgs.trace() && !fs::exists(lArgs.tracePath()))
   {
       std::cerr << "The path where to store the trace file is invalid." << std::endl;
       return -1;
   }

   if(lArgs.tiers() < 2)
   {
       std::cerr << "The number of tiers must be larger than two." << std::endl;
       return -1;
   }

    const control_block_t lCB = {
        lArgs.trace(),               // trace activated?
        lArgs.measure(),             // measure runtime/IR performance?
        lArgs.collectionPath(),      // path to doc collection file
        lArgs.queryPath(),           // path to directory with query files
        lArgs.relevanceScoresPath(), // path to relevance score path
        lArgs.stopwordPath(),        // path to stopword file
        lArgs.wordEmbeddingsPath(),  // path to word embeddings file
        lArgs.tracePath(),           // path to trace log file
        lArgs.evalPath(),            // path to evaluation results (JSON object is stored here)
        lArgs.results(),             // topK argument
        lArgs.tiers(),               // number of tiers
        lArgs.dimensions()           // number of dimensions
    };

    Trace::getInstance().init(lCB);
    Evaluation::getInstance().init(lCB);

    // insert everything here what is not actually meant to be in main
    // test(lCB);
    // testNico();
    // testAlex(lCB);
    testEval(lCB);

    return 0;
}
