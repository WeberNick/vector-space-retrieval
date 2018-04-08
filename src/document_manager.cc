#include "document_manager.hh"

#include <fstream>
#include <sstream>
#include <vector>

const char DocumentManager::_delimiter = '~';
size_t DocumentManager::_countID = 0;
bool DocumentManager::_isCreated = false;
DocumentManager* DocumentManager::_instance = NULL;

DocumentManager::DocumentManager(const std::string& aPath) {
    read(aPath);
    // _indexMgr = IndexManager::getInstance();
    // handle indexManager calls
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

bool DocumentManager::insert(const DocumentMapElem& aElement) {}

bool DocumentManager::insert(const size_t aKey, const Document& aDocument) {}

DocumentManager::DocMapIterator DocumentManager::find(const size_t aKey) {}

bool DocumentManager::erase(const size_t aKey) {}

bool DocumentManager::erase(const DocMapIterator aIterator) {}