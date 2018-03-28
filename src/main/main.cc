#include "../document.hh"
#include "../document_collection.hh"
#include "../infra/types.hh"
#include "../inverted_index.hh"
#include "../posting.hh"

int main(const int argc, const char* argv[]) {
  /**
   * Basic Idea to create inverted index (pseudo code):
   *
   * DocumentCollection docCol("path/to/root/directory")
   * docCol.collect();
   * InvertedIndex invInd();
   * vector<string> terms;
   * for each Document d in docCol{
   *      d.preprocess();
   *      for each term t in d{
   *          terms.push_back(t);
   *      }
   *      invInd.addTerm(terms, d.getID());
   *      terms.clear();
   * }
   */

  std::cout << "" << std::endl;
  std::vector<unsigned int> test;

  std::cout << "Hello CMake" << std::endl;
  return 0;
}
