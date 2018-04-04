#include "document_manager.hh"
#include <fstream>
#include <sstream>

size_t DocumentManager::_countID = 0;
bool DocumentManager::_isCreated = false;
DocumentManager* DocumentManager::_instance = NULL;

DocumentManager::DocumentManager(const std::string& aPath) {
  read(aPath);
}

void DocumentManager::read(const std::string& aFile) {
  std::ifstream lFile(aFile);
  std::string line;
  while (std::getline(lFile, line)) {
    std::istringstream isstream(line);
  }
}