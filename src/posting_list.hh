#pragma once

#include "types.hh"

#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class PostingList {
  public:
    using posting = std::map<size_t, uint>; // docID, TF: [(1, 25), (2, 0), ...]
  public:
    explicit PostingList(const std::string& aTerm, const float aIDF);
    explicit PostingList() = delete;
    PostingList(const PostingList&) = delete;
    PostingList(PostingList&&) = delete;
    PostingList& operator=(const PostingList&) = delete;
    PostingList& operator=(PostingList&&) = delete;
    ~PostingList();

  public:
    inline float getIDF() const { return _idf; }
    inline size_t getTF(uint aDocID) { return _posting.at(aDocID); }

  private:
    float _idf;
    posting _posting;
};

using postinglist_mt = std::unordered_map<std::string, PostingList>; // "term", PostingList; for Example: [("Frodo", <PostingListObj>), ...]
using posting_map_iter_t = postinglist_mt::iterator;
