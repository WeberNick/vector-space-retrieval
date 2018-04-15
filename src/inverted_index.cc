#include "inverted_index.hh"

InvertedIndex::InvertedIndex() : 
    _cb(nullptr),
    _term_posting_map()
{}

InvertedIndex::~InvertedIndex() {}

void InvertedIndex::init() {
    this->buildInvertedIndex();
}

void InvertedIndex::init(const control_block_t& aCB) {
    _cb = &aCB;
}

void InvertedIndex::buildInvertedIndex() {
    const doc_mt& docMap = DocumentManager::getInstance().getDocumentMap();
    str_float_mt idfMap;
    Utility::IR::calcIDFMap(docMap, idfMap);
    for (const auto& [term, idf] : idfMap) {
        sizet_float_mt id_tf_map;
        for (const auto& [id, doc] : docMap) {
            try {
                float tf = doc.getTF(term);
                id_tf_map[id] = tf;
            } catch (InvalidArgumentException iae) {
                continue;
            }
        }
        PostingList posting_list(idf, id_tf_map);
        this->insert(term, posting_list);
    }
}

bool InvertedIndex::insert(const std::string& aTerm, const PostingList& aPostingList) {
    return _term_posting_map.insert(std::make_pair(aTerm, aPostingList)).second; // copy constructor required
}

posting_map_iter_t InvertedIndex::find(const std::string& aKey) {
    return _term_posting_map.find(aKey);
}

void InvertedIndex::erase(const std::string& aKey) {
    _term_posting_map.erase(aKey);
}

void InvertedIndex::erase(const posting_map_iter_t aIterator) {
    _term_posting_map.erase(aIterator);
}

const PostingList& InvertedIndex::getPostingList(const std::string& aTerm) {
    if (_term_posting_map.find(aTerm) != _term_posting_map.end())
        return _term_posting_map.at(aTerm);
    else
        throw InvalidArgumentException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "The term " + aTerm + " doest not appear in the document Collection.");
}

size_t InvertedIndex::getNoDocs(const std::string& aTerm) {
    if (_term_posting_map.find(aTerm) != _term_posting_map.end())
        return _term_posting_map.at(aTerm).getPosting().size();
    else
        throw InvalidArgumentException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "The term " + aTerm + " doest not appear in the document Collection.");
}