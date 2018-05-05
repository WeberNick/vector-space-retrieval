#include "document_manager.hh"
#include "index_manager.hh"
#include "types.hh"
#include "utility.hh"

/**
 * @brief Construct a new Document Manager:: Document Manager object
 *
 */
DocumentManager::DocumentManager() :
    _cb(nullptr), _init(false), _collectionFile(), // relative path from /path/to/repo/vector-space-retrieval
    _delimiter('~'), _docs() {}

/**
 * @brief Destroy the Document Manager:: Document Manager object
 *
 */
DocumentManager::~DocumentManager() {}

void DocumentManager::init(const control_block_t& aControlBlock) {
    if (!_init) {
        _cb = &aControlBlock;
        _collectionFile = _cb->collectionPath();
        read(_collectionFile);
        _init = true;
    }
}

void DocumentManager::read(const std::string& aFile) {
    std::ifstream file(aFile);
    std::string line;
    while (std::getline(file, line)) {
        string_vt parts;
        Utility::StringOp::splitStringBoost(line, _delimiter, parts);
        string_vt content;
        Utility::StringOp::splitStringBoost(parts[1], ' ', content);
        std::string docID = parts[0];
        Document doc(docID, content);
        this->insert(std::make_pair(doc.getID(), doc));
        _ids.push_back(doc.getID());
    }
}

bool DocumentManager::insert(const doc_map_elem_t& aElement) { return _docs.insert(aElement).second; }

bool DocumentManager::insert(const size_t aKey, const Document& aDocument) { return _docs.insert(std::pair<size_t, Document>(aKey, aDocument)).second; }

doc_map_iter_t DocumentManager::find(const size_t aKey) { return _docs.find(aKey); }

void DocumentManager::erase(const size_t aKey) { _docs.erase(aKey); }

void DocumentManager::erase(const doc_map_iter_t aIterator) { _docs.erase(aIterator); }

Document& DocumentManager::getDocument(size_t aDocID) {
    if (_docs.find(aDocID) != _docs.end())
        return _docs.at(aDocID);
    else
        throw InvalidArgumentException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "This docID does not appear in the document collection.");
}

// TODO: Wenn wir das in nem loop, mit testsearch aufrufen bekomme ich ne bad malloc
Document DocumentManager::createQueryDoc(std::string& query) {
    std::cout << "before remove stopword" << std::endl;
    Utility::IR::removeStopword(query, QueryProcessingEngine::getInstance().getStopwordlist()); // Remove stopwords
    std::cout << "after remove stopword before trim" << std::endl;
    Utility::StringOp::trim(query); // Trim whitespaces
    std::cout << "after trying before split string" << std::endl;
    string_vt proc_query;
    Utility::StringOp::splitString(query, ' ', proc_query); // Split string by whitespaces
    std::cout << "after split string" << std::endl;
    Utility::StringOp::removeEmptyStringsFromVec(proc_query); // Remove eventually empty strings from the query term vector
    std::cout << "after remove empty string" << std::endl;
    std::vector<std::string> preprocessed_content;
    for (auto& elem : proc_query) { // Preprocess query
        std::string preprocess = Utility::IR::stemPorter(elem);
        std::cout << "Preprocessed: " << preprocess << std::endl;
        preprocessed_content.push_back(preprocess);
    }

    Document d("query-0", preprocessed_content);

    // build docTermTFMap
    const string_vt& con = d.getContent();
    str_int_mt tf_counts;
    str_float_mt tf_out;

    for (const std::string& term : con) {
        ++tf_counts[term];
    }

    int maxFreq = Utility::StringOp::getMaxWordFrequency(con);
    for (const auto& [term, count] : tf_counts) { // this loops through the distinct terms of this document
        tf_out[term] = Utility::IR::calcTf(count, maxFreq);
    }
    d.setTermTfMap(tf_out);
    // End build docTermTFMap

    IndexManager::getInstance().buildTfIdfVector(d);
    IndexManager::getInstance().buildRandProjVector(d);

    std::cout << "For query: " << query << std::endl;
    std::cout << "TFID:";

    for (size_t j = 0; j < d.getTfIdfVector().size(); ++j) {
        std::cout << d.getTfIdfVector()[j];
    }
    std::cout << std::endl;

    std::cout << "RANDVEC:";
    for (size_t j = 0; j < d.getRandProjVec().size(); ++j) {
        std::cout << d.getRandProjVec()[j];
    }
    std::cout << std::endl;

    return d;
}
