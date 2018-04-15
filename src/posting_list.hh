#pragma once

#include "types.hh"

#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class PostingList {
  public:
    explicit PostingList(const float aIDF, const sizet_float_mt& aPosting);
    explicit PostingList(const PostingList& pl);
    explicit PostingList() = default;
    PostingList(PostingList&&) = delete;
    PostingList& operator=(const PostingList&) = delete;
    PostingList& operator=(PostingList&&) = default;
    ~PostingList();

  public:
    void pprintPosting(const std::string& aTerm) const;
    inline sizet_float_mt getPosting() const { return _posting; }
    inline float getIDF() const { return _idf; }
    inline void setIDF(float aIDF) { _idf = aIDF; }
    inline float getTF(size_t aDocID) const { return _posting.at(aDocID); }
    inline void setTF(size_t aDocID, float aTF) { _posting[aDocID] = aTF; }

  private:
    float _idf;
    sizet_float_mt _posting; // docID, TF: [(1, 25), (2, 0), ...]
};

using postinglist_mt = std::unordered_map<std::string, PostingList>; // term, PostingList: [("Frodo", <PostingListObj>), ...]
using posting_map_iter_t = postinglist_mt::iterator;