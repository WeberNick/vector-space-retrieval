#include "posting_list.hh"

/**
 * @brief Construct a new Posting List:: Posting List object
 * 
 * @param aIdf the idf
 * @param aPosting the posting
 */
PostingList::PostingList(const float aIdf, const sizet_float_mt& aPosting) : 
    _idf(aIdf),
    _posting(aPosting)
{}

/**
 * @brief Destroy the Posting List:: Posting List object
 * 
 */
PostingList::~PostingList() {}

/**
 * @brief Construct a new Posting List:: Posting List object
 * 
 * @param pl the postingList to copy
 */
PostingList::PostingList(const PostingList& pl) : 
    _idf(pl.getIdf()),
    _posting(pl.getPosting())
{}

const sizet_vt PostingList::getIDs() const {
    sizet_vt ids;
    ids.reserve(_posting.size());
    for (auto& item : _posting) {
        ids.push_back(item.first);
    }
    return ids;
}

std::ostream& operator<<(std::ostream& strm, const PostingList& pl) {
    strm << "[ ";
    for (auto it = pl.getPosting().begin(); it != pl.getPosting().end(); ++it) {
        strm << "(" << it->first << ", " << it->second << ") ";
    }
    return strm << "]";
}