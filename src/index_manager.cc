#include "index_manager.hh"
#include "utility.hh"

IndexManager::IndexManager() {}

IndexManager::~IndexManager() {}

void IndexManager::buildInvertedIndex(const doc_mt& aCollection) {
    str_float_mt idfMap;
    Utility::IR::calcIDFMap(aCollection, idfMap);
    for (const auto& term : idfMap) {
        
    }
}

void IndexManager::buildTieredIndex(const doc_mt& aCollection) {

}
/**
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
