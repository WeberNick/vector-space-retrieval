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
IndexManager::~IndexManager()
{}

void IndexManager::init(const control_block_t& aControlBlock, doc_mt& aDocMap) {    
    if (!_init) {
        _cb = &aControlBlock;
        _docs = &aDocMap;
        _collection_terms.reserve(_docs->size());
        
        _clusteredIndex.chooseLeaders();
        const sizet_vt& leaders = _clusteredIndex.getLeaders();
        cluster_mt* cluster_out = _clusteredIndex.getClusterMap();
        str_postinglist_mt* postinglist_out = _invertedIndex.getTermPostingMap();
        str_tierplmap_mt* tieredpostinglist_out = _tieredIndex.getTermTierPostingMap();

        _invertedIndex.init(aControlBlock);
        _tieredIndex.init(aControlBlock);
        _clusteredIndex.init(aControlBlock);
        this->buildIndices(postinglist_out, tieredpostinglist_out, cluster_out, leaders);
        _init = true;
    }
}

void IndexManager::buildIndices(str_postinglist_mt* postinglist_out,
                                str_tierplmap_mt* tieredpostinglist_out,
                                cluster_mt* cluster_out,
                                const sizet_vt& leaders) {
    str_int_mt idf_occs;
    for (const auto& [id, doc] : *(_docs)) {
        str_int_mt tf_counts;
        str_float_mt tf_out;
        sizet_float_mt posting;
        const string_vt& con = doc.getContent();
        for (const std::string& term : con) {
            ++tf_counts[term];
            if (postinglist_out->find(term) == postinglist_out->end()) { // term not in map
                posting[id] = 0;                                         // tf has to be set below
                (*postinglist_out)[term] = PostingList(0, posting);      // idf has to be set below  
            }
        }
        int maxFreq = Utility::StringOp::getMaxWordFrequency(con);
        for (const auto& [term, count] : tf_counts) { // this loops through the distinct terms of this document
            tf_out[term] = Utility::IR::calcTf(count, maxFreq);
            (*postinglist_out)[term].setTf(id, tf_out.at(term));
            ++idf_occs[term];
        }
        _docs->at(id).setTermTfMap(tf_out);
    }
    const int N = _docs->size();
    for (const auto& [term, occ] : idf_occs) { // sizeof idf_occs == distinct_terms
        _idf_map[term] = Utility::IR::calcIdf(N, occ);
        (*postinglist_out)[term].setIdf(_idf_map[term]);
        (*tieredpostinglist_out)[term] = Utility::IR::calculateTiers(_cb->tiers(), (*postinglist_out)[term]);
        _collection_terms.push_back(term);
    }
    for (auto& elem : *(_docs)) {
        Document& doc = elem.second;  
        float_vt tivec = doc.getTfIdfVector();
        tivec.reserve(_collection_terms.size());
        for (std::string& term : _collection_terms) {
            str_float_mt& termTfMap = doc.getTermTfMap();
            if (termTfMap.find(term) != termTfMap.end())
                tivec.push_back(Utility::IR::calcTfIdf(termTfMap.at(term), _idf_map.at(term)));
            else
                tivec.push_back(0);  
        }
        doc.setNormLength(Utility::SimilarityMeasures::vectorLength(tivec));
        doc.setTfIdfVector(tivec);
    }
    for (auto& elem : *(_docs)) {
        Document& doc = elem.second;
        const size_t index = QueryProcessingEngine::getInstance().searchCollectionCosFirstIndex(&doc, leaders);
        cluster_out->at(index).push_back(doc.getID());
    }
}
