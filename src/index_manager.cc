#include "index_manager.hh"

IndexManager::IndexManager() {}

IndexManager::~IndexManager() {}

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