#include "posting_list.hh"

PostingList::PostingList(const float aIDF, const sizet_float_mt& aPosting) : 
    _idf(aIDF),
    _posting(aPosting)
{}

PostingList::~PostingList() {}

PostingList::PostingList(const PostingList& pl) : 
    _idf(pl.getIDF()),
    _posting(pl.getPosting())
{}

void PostingList::pprintPosting(const std::string& aTerm) const {
    std::cout << "\"" << aTerm << "\" -> [";
    std::string sep = ") ";
    for(auto it = _posting.begin(); it != _posting.end(); ++it) {
        if (it == std::prev(_posting.end(), 1)) {
            sep = ")";
        }
        std::cout << "(" << it->first << ", " << it->second << sep;
    }
    std::cout << "]" << std::endl;
}