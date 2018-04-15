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
    explicit PostingList() = delete;
    PostingList(PostingList&&) = delete;
    PostingList& operator=(const PostingList&) = delete;
    PostingList& operator=(PostingList&&) = delete;
    ~PostingList();

  public:
    inline sizet_float_mt getPosting() const { return _posting; }
    inline float getIDF() const { return _idf; }
    inline size_t getTF(uint aDocID) const { return _posting.at(aDocID); }

  private:
    float _idf;
    sizet_float_mt _posting; // docID, TF: [(1, 25), (2, 0), ...]
};

typedef std::unordered_map<std::string, PostingList> postinglist_mt; // term, PostingList: [("Frodo", <PostingListObj>), ...]
typedef postinglist_mt::iterator posting_map_iter_t;