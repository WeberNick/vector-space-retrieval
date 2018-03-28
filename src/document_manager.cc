#include "document_manager.hh"
//#include <fstream>

size_t DocumentManager::_countID = 0;
std::string DocumentManager::_dataDir = "./data";

/*DocumentManager::DocumentManager(const std::string& aFile) {
  read(aFile);
}

void DocumentManager::read(const std::string& aFile) {
  std::ifstream lFile(aFile);
  std::string line;
  while (std::getline(lFile, line)) {
    std::istringstream isstream(line);
  }
}*/
