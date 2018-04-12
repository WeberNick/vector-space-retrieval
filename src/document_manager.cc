#include "document_manager.hh"

size_t DocumentManager::_countID = 0;

DocumentManager::DocumentManager() : 
    _collectionFile("./data/collection.docs"), //relative path from /path/to/repo/vector-space-retrieval
    _delimiter('~'),
    _docs()
{
    read(_collectionFile);
    // handle indexManager calls
}

DocumentManager::~DocumentManager() { }

DocumentManager& DocumentManager::getInstance() {
    static DocumentManager lInstance;
    return lInstance;
}

/**
 * @brief
 *
 * @param aFile
 */
void DocumentManager::read(const std::string& aFile) {
    std::ifstream file(aFile);
    std::string line;
    while (std::getline(file, line)) {
        string_vt parts;
        Utility::StringOp::splitString(line, _delimiter, parts);
        string_vt content;
        Utility::StringOp::splitString(parts[1], ' ', content);
        Document doc(parts[0], content);
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
