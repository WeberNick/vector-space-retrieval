#pragma once

#include "types.hh"

#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class PostingList {
  public:
    typedef std::map<size_t, uint> posting; // docID, TF: [(1, 25), (2, 0), ...]

  public:
    explicit PostingList(const float aIDF);
    explicit PostingList(const PostingList& pl);
    explicit PostingList() = delete;
    PostingList(PostingList&&) = delete;
    PostingList& operator=(const PostingList&) = delete;
    PostingList& operator=(PostingList&&) = delete;
    ~PostingList();

  public:
    inline posting getPosting() const { return _posting; }
    inline float getIDF() const { return _idf; }
    inline size_t getTF(uint aDocID) const { return _posting.at(aDocID); }

  private:
    float _idf;
    posting _posting;
};

typedef std::unordered_map<std::string, PostingList> postinglist_mt; // term, PostingList: [("Frodo", <PostingListObj>), ...]
typedef postinglist_mt::iterator posting_map_iter_t;