/**
 *	@file 	posting_list.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  Implements the postingList with idf and posting (docID, tf)
 *	@bugs 	Currently no bugs known
 *	@todos	Write DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include "types.hh"

#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class PostingList {
  public:
    explicit PostingList(const float aIdf, const sizet_float_mt& aPosting);
    explicit PostingList(const PostingList& pl);
    explicit PostingList() = default;
    PostingList(PostingList&&) = delete;
    PostingList& operator=(const PostingList&) = delete;
    PostingList& operator=(PostingList&&) = default;
    ~PostingList();

  public:
    /**
     * @brief Get the term frequency from this terms posting list for document with id aDocID
     *
     * @param aDocID the id of the document
     * @return float the tf of the term in document with id aDocID
     */
    inline float getTf(size_t aDocID) const { return _posting.at(aDocID); }
    /**
     * @brief Get the inverse document frequency for the corresponding term of this posting list
     *
     * @return float the idf of the corresponding term
     */
    inline float getIdf() const { return _idf; }
    /**
     * @brief Get the posting for the corresponding term
     *
     * @return const sizet_float_mt& the posting for the corresponding term
     */
    inline const sizet_float_mt& getPosting() const { return _posting; }

    /**
     * @brief Set the term frequency from this terms posting list to tf for document with id aDocID
     *
     * @param aDocID the id of the document
     * @param aTf the term frequency to set
     */
    inline void setTf(size_t aDocID, float aTf) { _posting[aDocID] = aTf; }
    /**
     * @brief Set the inverse document frequency for this temr to aIdf
     *
     * @param aIdf the inverse document frequency to set
     */
    inline void setIdf(float aIdf) { _idf = aIdf; }

    /**
     * @brief Override operator<< for pretty printing a PostingList object
     *
     * @param strm the output stream
     * @param pl the PostingList Object
     * @return std::ostream& the output stream to be returned
     */
    friend std::ostream& operator<<(std::ostream& strm, const PostingList& pl);

  private:
    float _idf;
    sizet_float_mt _posting; // docID, Tf: [(1, 25), (2, 0), ...]
};

using postinglist_mt = std::unordered_map<std::string, PostingList>; // term, PostingList: [("Frodo", <PostingListObj>), ...]
using posting_map_iter_t = postinglist_mt::iterator;