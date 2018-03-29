#include "../document.hh"
#include "../document_manager.hh"
#include "../infra/similarity_measures.hh"
#include "../infra/types.hh"

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

  // doc parser here, replace "dev.txt"
  // DocumentManager* docManager = new DocumentManager("dev.txt");

  std::vector<double> doc_a = {1,3,5,8, 100, 100};
  std::vector<double> doc_b = {2,4,5,1,2,0};

  std::cout << similarity_measures::tf_idf(5.0, 10.0) << std::endl;
  std::cout << similarity_measures::euclidean_distance_normalized(doc_a, doc_b) << std::endl;
  std::cout << similarity_measures::cosine_similarity(doc_a, doc_b) << std::endl;
  std::cout << "Hello CMake" << std::endl;
  return 0;
}
