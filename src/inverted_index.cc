#include "inverted_index.hh"

InvertedIndex::InvertedIndex() {}

InvertedIndex::~InvertedIndex() {}

bool InvertedIndex::insert(const std::string& aTerm, const PostingList& aPostingList) {
    return _term_posting_map.insert(std::make_pair(aTerm, aPostingList)).second; // copy required
    // _term_posting_map.insert(std::pair<std::string, PostingList>(aTerm, aPostingList)); // copy required
    // _term_posting_map[aTerm] = aPostingList; // default, assignment required
}

posting_map_iter_t InvertedIndex::find(const std::string& aKey) {
    return _term_posting_map.find(aKey)
}

void InvertedIndex::erase(const std::string& aKey) {
    _term_posting_map.erase(aKey);
}

void InvertedIndex::erase(const posting_map_iter_t aIterator) {
    _term_posting_map.erase(aIterator);
}

// const size_t_vt& InvertedIndex::getDocIDs(const std::string& aTerm) {}

void InvertedIndex::addDocID(const std::string& aTerm, const size_t aDocID) {

}

void InvertedIndex::getDocID(const std::string& aTerm, size_t aIndex) {

}

size_t InvertedIndex::getNoDocs(const std::string& aTerm) {
    // TODO
    return 0;
}