#pragma once

#include "document.hh"
#include "types.hh"
#include "utility.hh"
#include <string>
#include <vector>

class QueryProcessingEngine {
  private:
    QueryProcessingEngine();                                                 // Constructor hidden
    QueryProcessingEngine(QueryProcessingEngine const&) = delete;            // Nick: modified to delete
    QueryProcessingEngine& operator=(QueryProcessingEngine const&) = delete; // Nick: modified to delete
    ~QueryProcessingEngine() {}                                              // Destructor hidden

  public:
    /**
     * @brief Get the Instance object
     *
     * @return QueryProcessingEngine&
     */
    inline static QueryProcessingEngine& getInstance() {
        static QueryProcessingEngine lInstance;
        return lInstance;
    }

    void init(const CB& aControlBlock);

  private:
    void read(const std::string& aFile);
    Document createQueryDoc(std::string& query);

  public:
    inline string_vt getStopwordlist() { return _stopword_list; }
    const pair_sizet_float_vt search(std::string& query, size_t topK, IR_MODE searchType);
    const pair_sizet_float_vt searchCollectionCos(const Document* query, const sizet_vt& collectionIds, size_t topK);
    const pair_sizet_float_vt searchClusterCos(const Document* query, const sizet_vt& collectionIds, size_t topK);
    const size_t searchClusterCosFirstIndex(const Document* query, const sizet_vt& collectionIds);
    const pair_sizet_float_vt searchTieredCos(const Document* query, const sizet_vt& collectionIds, size_t topK);
    const pair_sizet_float_vt searchRandomProjCos(const Document* query, const sizet_vt& collectionIds, size_t topK);

  private:
    const CB* _cb;

    bool _init;
    std::string _stopwordFile;
    string_vt _stopword_list;
};
