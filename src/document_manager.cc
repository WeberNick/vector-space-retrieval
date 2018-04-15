#include "document_manager.hh"
#include "utility.hh"

size_t DocumentManager::_countID = 0;

/**
 * @brief Construct a new Document Manager:: Document Manager object
 * 
 */
DocumentManager::DocumentManager() : 
    _cb(nullptr),
    _collectionFile("./data/collection.docs"), // relative path from /path/to/repo/vector-space-retrieval
    _delimiter('~'),
    _docs()
{
    this->init();
}

DocumentManager::~DocumentManager() { }

void DocumentManager::init() {
    this->read(_collectionFile);
}

void DocumentManager::init(const control_block_t& aCB) {
    _cb = &aCB;
}

/**
 * @brief reads in the document collection, creates Document objects and fills the _docs map
 *
 * @param aFile the document collection
 */
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

/**
 * @brief 
 * 
 * @param aElement 
 * @return true 
 * @return false 
 */
bool DocumentManager::insert(const doc_map_elem_t& aElement) {
    return _docs.insert(aElement).second;
}

/**
 * @brief 
 * 
 * @param aKey 
 * @param aDocument 
 * @return true 
 * @return false 
 */
bool DocumentManager::insert(const size_t aKey, const Document& aDocument) {
    return _docs.insert(std::pair<size_t, Document>(aKey, aDocument)).second;
}

/**
 * @brief 
 * 
 * @param aKey 
 * @return doc_map_iter_t 
 */
doc_map_iter_t DocumentManager::find(const size_t aKey) {
    return _docs.find(aKey);
}

/**
 * @brief 
 * 
 * @param aKey 
 */
void DocumentManager::erase(const size_t aKey) {
    _docs.erase(aKey);
}

/**
 * @brief 
 * 
 * @param aIterator 
 */
void DocumentManager::erase(const doc_map_iter_t aIterator) {
    _docs.erase(aIterator);
}
