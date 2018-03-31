<<<<<<< HEAD:src/main.cc
#include "args.hh"
#include "measure.hh"
#include "types.hh"

#include "document.hh"
#include "document_manager.hh"


int main(const int argc, const char* argv[]) 
{
    /* How to use class Args is described in args.hh */
    Args lArgs;
    argdesc_vt lArgDesc;
    construct_arg_desc(lArgDesc);

    if(!parse_args<Args>(1, argc, argv, lArgDesc, lArgs)) 
    {
        std::cerr << "error while parsing arguments." << std::endl;
        return -1;
    }
    
    if(lArgs.help()) 
    {
        print_usage(std::cout, argv[0], lArgDesc);
        return 0;
    }

    /* Example how to use Measurement class (also described in measure.hh) */

    Measure lMeasure;
    if(lArgs.measure())
    {
        lMeasure.start();
    }
    //do processing
    lMeasure.stop();
    double lSeconds = lMeasure.mTotalTime();

=======
#include "../document.hh"
#include "../document_manager.hh"
#include "../infra/similarity_measures.hh"
#include "../infra/types.hh"
>>>>>>> similarity-measures:src/main/main.cc

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

<<<<<<< HEAD:src/main.cc
    std::cout << "Hello CMake" << std::endl;
    return 0;
=======
  std::vector<double> doc_a = {1,3,5,8, 100, 100};
  std::vector<double> doc_b = {2,4,5,1,2,0};

  std::cout << similarity_measures::tf_idf(5.0, 10.0) << std::endl;
  std::cout << similarity_measures::euclidean_distance_normalized(doc_a, doc_b) << std::endl;
  std::cout << similarity_measures::cosine_similarity(doc_a, doc_b) << std::endl;
  std::cout << "Hello CMake" << std::endl;
  return 0;
>>>>>>> similarity-measures:src/main/main.cc
}
