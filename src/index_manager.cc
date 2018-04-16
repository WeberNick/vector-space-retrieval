#include "index_manager.hh"
#include "measure.hh"

/**
 * @brief Construct a new Index Manager:: Index Manager object
 * 
 */
IndexManager::IndexManager() :
    _cb(nullptr),
    _docs(nullptr),
    _init(false),
    _idf_map(),
    _collection_terms(),
    _invertedIndex(InvertedIndex::getInstance()),
    _tieredIndex(TieredIndex::getInstance()),
    _clusteredIndex(Cluster::getInstance())
{}

/**
 * @brief Destroy the Index Manager:: Index Manager object
 * 
 */
IndexManager::~IndexManager() {}

void IndexManager::init(const control_block_t& aControlBlock, doc_mt& aDocMap) {
    _cb = &aControlBlock;
    _docs = &aDocMap;
    
    if (!_init) {
        _collection_terms.reserve(_docs->size());
        postinglist_mt postinglist_out;
        this->buildIndices(postinglist_out);

        _invertedIndex.init(aControlBlock, postinglist_out);
        _tieredIndex.init(aControlBlock);
        _clusteredIndex.init(aControlBlock);
        _init = true;
    }
}

void IndexManager::buildIndices(postinglist_mt& postinglist_out) {
    str_int_mt idf_occs;
    for (const auto & [ id, doc ] : *(_docs)) {
        str_int_mt tf_counts;
        str_float_mt tf_out;
        sizet_float_mt posting;
        const string_vt& con = doc.getContent();
        for (const std::string& term : con) {
            ++tf_counts[term];
            if (postinglist_out.find(term) == postinglist_out.end()) { // term not in map
                posting[id] = 0;                                      // tf has to be set below
                postinglist_out[term] = PostingList(0, posting);       // idf has to be set below
            }
        }
        int maxFreq = Utility::StringOp::getMaxWordFrequency(con);
        for (const auto & [ term, count ] : tf_counts) { // this loops through the distinct terms of this document
            tf_out[term] = static_cast<float>((1 + log10(count)) / (1 + log10(maxFreq)));
            postinglist_out[term].setTf(id, tf_out.at(term));
            ++idf_occs[term];
        }
        _docs->at(id).setTermTfMap(tf_out);
    }
    const int N = _docs->size();
    for (const auto & [ term, occ ] : idf_occs) { // sizeof idf_occs == distinct_terms
        _idf_map[term] = static_cast<float>(log10(N / occ));
        postinglist_out[term].setIdf(_idf_map[term]);
        _collection_terms.push_back(term);
        // build doc vec with collection terms, does not have to be sorted alphabetically but has to be the same order for every doc. this order is preserved in _collection_terms
    }
}