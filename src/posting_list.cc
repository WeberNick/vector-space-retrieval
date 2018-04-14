#include "posting_list.hh"

PostingList::PostingList(const float aIDF) : 
    _idf(aIDF)
{}

PostingList::~PostingList() {}

PostingList::PostingList(const PostingList& pl) : 
    _idf(pl.getIDF()),
    _posting(pl.getPosting())
{}