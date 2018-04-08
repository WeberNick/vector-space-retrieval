#include "document_manager.hh"
#include <fstream>
#include <sstream>

size_t DocumentManager::_countID = 0;
bool DocumentManager::_isCreated = false;
DocumentManager* DocumentManager::_instance = nullptr;

DocumentManager::DocumentManager(const std::string& aPath) {
  read(aPath);
}

DocumentManager::~DocumentManager()
{
    destroyInstance();
}

void DocumentManager::createInstance(const std::string& aPath){
    if (!DocumentManager::_isCreated) { //change to nullptr check and remove bool flag therefore?
        DocumentManager::_isCreated = true;
        DocumentManager::_instance = new DocumentManager(aPath);
    }
}

void DocumentManager::destroyInstance() {
    //change to nullptr check and remove bool flag therefore?
        if (DocumentManager::_isCreated) 
        { 
            _isCreated = false;     
            delete DocumentManager::_instance; 
        }
    }

DocumentManager& DocumentManager::getInstance() {
    if (!DocumentManager::_isCreated) {//change to nullptr check and remove bool flag therefore?
        //either change back to a return of a pointer or find more graceful solution?
        throw SingletonException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    } 
    return *DocumentManager::_instance;
}


void DocumentManager::read(const std::string& aFile) {
  std::ifstream lFile(aFile);
  std::string line;
  while (std::getline(lFile, line)) {
    std::istringstream isstream(line);
  }
}
