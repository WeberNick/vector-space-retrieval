#include "document_manager.hh"
#include <fstream>
#include <sstream>

size_t DocumentManager::_countID = 0;
DocumentManager* DocumentManager::_instance = nullptr;

DocumentManager::DocumentManager(const std::string& aPath) {
  read(aPath);
}

DocumentManager::~DocumentManager()
{
    destroyInstance();
}

void DocumentManager::createInstance(const std::string& aPath){
    if (!_instance) //if _instance -> nullptr = false, else true -> !_instance is true only if not created
    {
        _instance = new DocumentManager(aPath);
    }
}

void DocumentManager::destroyInstance() {
        if (_instance) 
        { 
            delete _instance; 
            _instance = nullptr;
        }
    }

DocumentManager& DocumentManager::getInstance() {
    if (!_instance) {
        //either change back to a return of a pointer or find more graceful solution?
        throw SingletonException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    } 
    return *_instance;
}


void DocumentManager::read(const std::string& aFile) {
  std::ifstream lFile(aFile);
  std::string line;
  while (std::getline(lFile, line)) {
    std::istringstream isstream(line);
  }
}
