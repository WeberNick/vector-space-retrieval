#include "query_processing_engine.hh"
#include "posting_list.hh"
#include "utility.hh"
#include <iostream>
#include <sstream>

std::vector<int> QueryProcessingEngine::cosineScore(const std::string& query, size_t topK) {

    /**int docCount = DocumentManager::getDocumentMap().size();

      std::vector<float> scores(docCount);
      std::vector<float> length(docCount);

      const std::string procQ = IR::stemPorter(query);

      std::istringstream iss(procQ);
      std::string term;

      while (iss >> term) {

          PostingList postingList = getPostingList(term);
          for (auto& posting : postingList._posting) {
              scores[posting.docId] += posting.weight * IR::calcTF(term, procQ);
          }
      }

      for (int i = 0; i < docCount; ++i) {
          scores[i] = scores[i] / length[i];
      }

      std::sort(scores.begin(), scores.end(), std::greater<>());

      auto end = std::next(scores.begin(), std::min(topK, scores.size()));

      return std::vector<int>(scores.begin(), end);**/

    return std::vector<int>();
}
size_t QueryProcessingEngine::cosineScoreCluster(Document* query, std::vector<Document*> collection) {

    /*int docCount = collection.size();

    std::vector<float> scores(docCount);
    std::vector<int> length(docCount);

    // Fill length vector with the lengths of the documents
    for (int i = 0; i < docCount; ++i) {
        length[i] = collection[i]->getContent().size();
    }

    for (int j = 0; j < query->getContent().size(); ++j) {
        const PostingList& postingList = IndexManager::getInstance().getInvertedIndex().getPostingList("termn");
        for (auto& posting : postingList) {
            postingList.getIDF();
        }
    }
    */
    return 2;
}
