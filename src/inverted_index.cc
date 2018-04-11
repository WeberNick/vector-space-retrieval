#include "inverted_index.hh"

InvertedIndex::InvertedIndex(){}

InvertedIndex::~InvertedIndex(){}

bool InvertedIndex::insert(const std::string& aTerm) {
    // return _postings.insert(aTerm, nullptr);
    return false;
}

// posting_map_iter_t InvertedIndex::find(const std::string& aKey) { // return _postings.find(aKey); }

void InvertedIndex::erase(const std::string& aKey) {
    _postings.erase(aKey);
}

void InvertedIndex::erase(const posting_map_iter_t aIterator) {
    _postings.erase(aIterator);
}

// const size_t_vt& InvertedIndex::getDocIDs(const std::string& aTerm) {}

void addDocID(const std::string& aTerm, const size_t aDocID) {

}

void getDocID(const std::string& aTerm, size_t aIndex) {

}

size_t getNoDocs(const std::string& aTerm) {
    // TODO
    return 0;
}