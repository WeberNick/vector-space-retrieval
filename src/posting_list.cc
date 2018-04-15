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