#pragma once

#include "types.hh"

#include <string>
#include <tuple>
#include <vector>

class PostingList {
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
    //inline uint getTF(uint aDocID) { return };

  private:
    float _idf;
    posting _posting; // has to be sorted
};

typedef std::map<std::string, PostingList*> postinglist_mt;
typedef postinglist_mt::iterator posting_map_iter_t;
typedef std::map<size_t, uint> posting; // docID, TF