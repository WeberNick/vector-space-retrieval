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
#include "src/vec_util.hh"
#include "src/measure.hh"

#include <experimental/filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>
namespace fs = std::experimental::filesystem;

void search(std::string query, size_t topK, IR_MODE mode) {
    QueryExecutionEngine& qpe = QueryExecutionEngine::getInstance();

    std::vector<std::pair<size_t, float>> result = qpe.search(query, topK, mode);
   
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

void serverMode(const control_block_t& aControlBlock) {
    
    DocumentManager& docManager = DocumentManager::getInstance();
    docManager.init(aControlBlock);

    IndexManager& imInstance = IndexManager::getInstance();
    imInstance.init(aControlBlock, docManager.getDocumentMap());

    //QueryManager& queryManager = QueryManager::getInstance();
    //queryManager.init(aControlBlock);

    QueryExecutionEngine& qpe = QueryExecutionEngine::getInstance();
    qpe.init(aControlBlock);

    std::cout << "[Ready]" << std::endl;

    while (true) {
        json j;
        try {
            std::cin >> j;
            search(j["query"].get<std::string>(), j["topK"].get<size_t>(), stringToMode(j["mode"].get<std::string>()));
        }catch (std::exception& e) {
            std::cout << "Malformated JSON" << std::endl;
        }
        
    }
}

void evalMode(const control_block_t& aControlBlock) {

    std::cout << "[Evaluation mode]" << std::endl;

    DocumentManager& docManager = DocumentManager::getInstance();
    docManager.init(aControlBlock);
    std::cout << "Document Manager initialized" << std::endl;

    Measure m;
    m.start();
    IndexManager& imInstance = IndexManager::getInstance();
    imInstance.init(aControlBlock, docManager.getDocumentMap());
    m.stop();
    std::cout << "Took " << m.mTotalTime() << std::endl;
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

    std::cout << "[Ready]" << std::endl;
    std::cout << "[Start Evaluating]" << std::endl;

     str_set queryNamesSet;

    /*
    for (int i = 0; i < kNumberOfTypes; ++i ) {

        QUERY_TYPE type = static_cast<QUERY_TYPE>(i);
        
        for (int j = 0; j < kNumberOfModes; ++j) {
            
             REMOVE
            if(aControlBlock.rand()){
                if(!(j==1||j==4||j==7)){
                    continue;
                }
            } else if(aControlBlock.tiered()){
                if(!(j==3||j==4||j==5)){
                    continue;
                }
            }

            IR_MODE mode = static_cast<IR_MODE>(j);

            std::cout <<  typeToString(type) << " for mode " << modeToString(mode) << std::endl; 
            
            auto& queryForType = QueryManager::getInstance().getQueryMap(type);
            for (auto& [query_id, query] : queryForType) {
                Document queryDoc = queryManager.createQueryDoc(query, query_id, true);

                queryNamesSet.insert(queryDoc.getDocID());
                e.start(type, mode, queryDoc.getDocID());
                std::vector<std::pair<size_t, float>> result = qpe.search(queryDoc, aControlBlock.results(), mode);
                e.stop();
                e.evalIR(type, mode, queryDoc.getDocID(), result);
            }
        }
    }*/

    const std::vector<IR_MODE> modes{kVANILLA, kVANILLA_RAND};
    const std::vector<QUERY_TYPE> types{kALL, kTITLES};

    for(auto type : types){
        for(auto mode: modes) {
            std::cout <<  typeToString(type) << " for mode " << modeToString(mode) << std::endl; 
            
            auto& queryForType = QueryManager::getInstance().getQueryMap(type);
            for (auto& [query_id, query] : queryForType) {
                Document queryDoc = queryManager.createQueryDoc(query, query_id, true);

                queryNamesSet.insert(queryDoc.getDocID());
                e.start(type, mode, queryDoc.getDocID());
                std::vector<std::pair<size_t, float>> result = qpe.search(queryDoc, aControlBlock.results(), mode);
                e.stop();
                e.evalIR(type, mode, queryDoc.getDocID(), result);
            }
        }
    }
    



    e.constructJSON(queryNamesSet);
    std::cout << "[Finish Evaluating]" << std::endl;
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
        std::cout << " ## Warning : Programm was not executed from the recommended directory " 
            << "'vector-space-retrieval'. Make sure to provide all necessary file "
            << "paths as command line argument. For further information, start the executable with '--help'."
            << "\n            ## Current Working Directory: " << fs::current_path() << std::endl;
    }

    if(!(fs::exists(lArgs.collectionPath())))
    {
        std::cerr << "Given path to the data collection is invalid." << std::endl;
        return -1;
    }

    if(!lArgs.server() && !fs::exists(lArgs.queryPath()))
    {
        std::cerr << "Given path to the query collection is invalid." << std::endl;
        return -1;
    }
 
    if(!lArgs.server() && !fs::exists(lArgs.relevanceScoresPath()))
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
        lArgs.server(),              // Starting the binary in server mode (free text search) ?
        lArgs.collectionPath(),      // path to doc collection file
        lArgs.queryPath(),           // path to directory with query files
        lArgs.relevanceScoresPath(), // path to relevance score path
        lArgs.stopwordPath(),        // path to stopword file
        lArgs.wordEmbeddingsPath(),  // path to word embeddings file
        lArgs.tracePath(),           // path to trace log file
        lArgs.evalPath(),            // path to evaluation results (JSON object is stored here)
        lArgs.results(),             // topK argument
        lArgs.tiers(),               // number of tiers
        lArgs.dimensions(),          // number of dimensions
        lArgs.seed()                 // seed for random projections and cluster leader election
    };

    std::cout << lCB;

    // Init tracing
    Trace::getInstance().init(lCB);
    
    if (lCB.server()){
        serverMode(lCB);
    } else {
        evalMode(lCB);
    }
    
    return 0;
}
