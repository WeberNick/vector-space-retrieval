#include "document_manager.hh"
#include <fstream>
#include <sstream>

size_t DocumentManager::_countID = 0;
bool DocumentManager::_isCreated = false;
DocumentManager* DocumentManager::_instance = nullptr;

DocumentManager::DocumentManager(const std::string& aPath) {
  read(aPath);
}

void DocumentManager::createInstance(const std::string& aPath){
    if (!DocumentManager::_isCreated) {
        DocumentManager::_isCreated = true;
        DocumentManager::_instance = new DocumentManager(aPath);
    }
}

void DocumentManager::destroyInstance() {
        if (DocumentManager::_isCreated) { delete DocumentManager::_instance; }
    }

DocumentManager* DocumentManager::getInstance() {
    if (DocumentManager::_isCreated) {
        return DocumentManager::_instance;
    } else {
        return nullptr;
    }
}


void DocumentManager::read(const std::string& aFile) {
  std::ifstream lFile(aFile);
  std::string line;
  while (std::getline(lFile, line)) {
    std::istringstream isstream(line);
  }
}
