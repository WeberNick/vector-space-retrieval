#include "document_manager.hh"
#include "utility.hh"
#include "index_manager.hh"

/**
 * @brief Construct a new Document Manager:: Document Manager object
 * 
 */
DocumentManager::DocumentManager() : 
    _cb(nullptr),
    _init(false),
    _collectionFile(), // relative path from /path/to/repo/vector-space-retrieval
    _delimiter('~'),
    _docs()
{}

/**
 * @brief Destroy the Document Manager:: Document Manager object
 * 
 */
DocumentManager::~DocumentManager() { }

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

bool DocumentManager::insert(const doc_map_elem_t& aElement) {
    return _docs.insert(aElement).second;
}

bool DocumentManager::insert(const size_t aKey, const Document& aDocument) {
    return _docs.insert(std::pair<size_t, Document>(aKey, aDocument)).second;
}

doc_map_iter_t DocumentManager::find(const size_t aKey) {
    return _docs.find(aKey);
}

void DocumentManager::erase(const size_t aKey) {
    _docs.erase(aKey);
}

void DocumentManager::erase(const doc_map_iter_t aIterator) {
    _docs.erase(aIterator);
}

Document& DocumentManager::getDocument(size_t aDocID) {
    if (_docs.find(aDocID) != _docs.end())
        return _docs.at(aDocID);
    else
        throw InvalidArgumentException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "This docID does not appear in the document collection.");
}
Document DocumentManager::createQueryDoc(std::string &query) {
    Utility::IR::removeStopword(query, QueryProcessingEngine::getInstance().getStopwordlist()); // Remove stopwords
    Utility::StringOp::trim(query);                        // Trim whitespaces

    string_vt proc_query;
    Utility::StringOp::splitString(query, ' ', proc_query);   // Split string by whitespaces
    Utility::StringOp::removeEmptyStringsFromVec(proc_query); // Remove eventually empty strings from the query term vector

    std::vector<std::string> preprocessed_content;
    for (auto& elem : proc_query) { // Preprocess query
        std::string preprocess = Utility::IR::stemPorter(elem);
        std::cout << "Preprocessed: " << preprocess << std::endl;
        preprocessed_content.push_back(preprocess);
    }

    Document d("query-0", preprocessed_content);
    IndexManager::getInstance().buildTfIdfVector(d);
    IndexManager::getInstance().buildRandProjVector(d);
    return d;
}
