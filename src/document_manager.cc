#include "document_manager.hh"
#include "index_manager.hh"
#include "types.hh"
#include "utility.hh"

/**
 * @brief Construct a new Document Manager:: Document Manager object
 *
 */
DocumentManager::DocumentManager() :
    _cb(nullptr), _init(false), _delimiter('~'), _collectionFile(), _queryTypes({"nontopictitles", "titles", "viddesc", "vidtitles"}), _docids(), _docs(),
    _str_docid(), _queryids(), _queries() {}

/**
 * @brief Destroy the Document Manager:: Document Manager object
 *
 */
DocumentManager::~DocumentManager() {}

void DocumentManager::init(const control_block_t& aControlBlock) {
    if (!_init) {
        _cb = &aControlBlock;
        _collectionFile = _cb->collectionPath();
        readDocs(_collectionFile);
        readQueries(_queryTypes);
        _init = true;
        TRACE("Document manager initialized");
    }
}

void DocumentManager::readDocs(const std::string& aFile) {
    std::ifstream file(aFile);
    std::string line;
    while (std::getline(file, line)) {
        string_vt parts;
        Utility::StringOp::splitStringBoost(line, _delimiter, parts);
        string_vt content;
        Utility::StringOp::splitStringBoost(parts[1], ' ', content);
        std::string docID = parts[0];
        Document doc(docID, content);
        _docs.insert(std::make_pair(doc.getID(), doc));
        _docids.push_back(doc.getID());
        _str_docid[doc.getDocID()] = doc.getID();
        std::cout << _docs.size() << std::endl;
    }
}

void DocumentManager::readQueries(const string_vt& aQueryTypes) {
    for (const auto& aType : aQueryTypes) {
        const std::string& aFile = _cb->queryPath() + "q-" + aType + ".queries";
        std::ifstream file(aFile);
        std::string line;
        doc_mt queries;
        sizet_vt queryids;
        while (std::getline(file, line)) {
            string_vt parts;
            Utility::StringOp::splitStringBoost(line, _delimiter, parts);
            std::string& queryID = parts[0];
            Document query = createQuery(parts[1], queryID);
            queries.insert(std::make_pair(query.getID(), query));
            queryids.push_back(query.getID());
            // std::cout << "Query " << queryID << ": " << parts[1];
        }
        _queries[aType] = queries;
        _queryids[aType] = queryids;
    }
}

Document DocumentManager::createQuery(std::string& content, const std::string& queryID) {
    std::string temp;
    content = Utility::StringOp::toLower(content);
    Utility::IR::removeStopword(content, QueryProcessingEngine::getInstance().getStopwordlist()); // Remove stopwords
    std::remove_copy_if(content.begin(), content.end(),
                        std::back_inserter(temp), // Store output
                        std::ptr_fun<int, int>(&std::ispunct));
    content = temp;
    Utility::StringOp::trim(content); // Trim whitespaces at front and end
    string_vt proc_query;
    Utility::StringOp::splitStringBoost(content, ' ', proc_query); // Split string by whitespaces
    Utility::StringOp::removeEmptyStringsFromVec(proc_query);      // Remove eventually empty strings from the query term vector

    std::vector<std::string> preprocessed_content;
    for (auto& elem : proc_query) { // Preprocess query
        std::string preprocess = Utility::IR::stemPorter(elem);
        preprocessed_content.push_back(preprocess);
    }
    Document quer(queryID, preprocessed_content);

    const string_vt& con = quer.getContent(); // start build docTermTFMap
    str_int_mt tf_counts;
    str_float_mt tf_out;

    for (const std::string& term : con) {
        ++tf_counts[term];
    }

    int maxFreq = Utility::StringOp::getMaxWordFrequency(con);
    for (const auto& [term, count] : tf_counts) { // this loops through the distinct terms of this document
        tf_out[term] = Utility::IR::calcTf(count, maxFreq);
    }
    quer.setTermTfMap(tf_out); // end build docTermTFMap

    IndexManager::getInstance().buildTfIdfVector(quer);
    IndexManager::getInstance().buildWordEmbeddingsVector(quer);
    IndexManager::getInstance().buildRandProjVector(quer);

    return quer;
}
