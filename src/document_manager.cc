#include "document_manager.hh"

#include <fstream>
#include <sstream>
#include <vector>

size_t DocumentManager::_countID = 0;

DocumentManager::DocumentManager() : 
    _collectionFile("./data/collection.docs"), //relative path from /path/to/repo/evsr
    _delimiter('~'),
    _indexMgr(IndexManager::getInstance()),
    _docs()
{

    //read(_collectionFile);
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
        std::istringstream isstream(line);
        std::string part;
        std::vector<std::string>* lineParts;
        lineParts->reserve(2);
        while (std::getline(isstream, part, '~')) {
            lineParts->push_back(part);
        }
        Document doc(lineParts->at(0), lineParts->at(1));
        // std::pair<size_t, Document> docMapElem(doc.getID(), doc);
        // this->insert(docMapElem);
    }
}

bool DocumentManager::insert(const doc_map_elem_t& aElement) {}

bool DocumentManager::insert(const size_t aKey, const Document& aDocument) {}

doc_map_iter_t DocumentManager::find(const size_t aKey) {}

bool DocumentManager::erase(const size_t aKey) {}

bool DocumentManager::erase(const doc_map_iter_t aIterator) {}
