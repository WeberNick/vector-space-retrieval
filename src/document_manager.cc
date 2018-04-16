#include "document_manager.hh"
#include "utility.hh"

/**
 * @brief Construct a new Document Manager:: Document Manager object
 * 
 */
DocumentManager::DocumentManager() : 
    _cb(nullptr),
    _init(false),
    _collectionFile("./data/collection.docs"), // relative path from /path/to/repo/vector-space-retrieval
    _delimiter('~'),
    _docs()
{}

/**
 * @brief Destroy the Document Manager:: Document Manager object
 * 
 */
DocumentManager::~DocumentManager() { }

void DocumentManager::init(const control_block_t& aControlBlock) {
    _cb = &aControlBlock;
    if (!_init) {
        read(_collectionFile);
        _init = true;
    }
}

void DocumentManager::read(const std::string& aFile) {
    std::ifstream file(aFile);
    std::string line;
    while (std::getline(file, line)) {
        string_vt parts;
        Utility::StringOp::splitString(line, _delimiter, parts);
        string_vt content;
        Utility::StringOp::splitString(parts[1], ' ', content);
        std::string docID = parts[0];
        Document doc(docID, content);
        this->insert(std::make_pair(doc.getID(), doc));
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