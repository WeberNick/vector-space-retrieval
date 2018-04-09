#include "document_manager.hh"

#include <fstream>
#include <sstream>
#include <vector>

//DocumentManager* DocumentManager::_instance = nullptr;
size_t DocumentManager::_countID = 0;

DocumentManager::DocumentManager() : 
    _collectionFile("./data/collection.docs"),
    _delimiter('~'),
    _indexMgr(IndexManager::getInstance()),
    _docs()
{

    //read(_collectionFile);
    // handle indexManager calls
}

DocumentManager::~DocumentManager() { /*destroyInstance();*/ }

/*void DocumentManager::createInstance(const std::string& aPath) {*/
    //if (!_instance) // if _instance -> nullptr = false, else true -> !_instance is true only if not created
    //{
        //_instance = new DocumentManager(aPath);
    //}
/*}*/

/*void DocumentManager::destroyInstance() {*/
    //if (_instance) {
        //delete _instance;
        //_instance = nullptr;
    //}
/*}*/

DocumentManager& DocumentManager::getInstance() {
   /* if (!_instance) {*/
        //// either change back to a return of a pointer or find more graceful solution?
        //throw SingletonException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    //}
    /*return *_instance;*/
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
