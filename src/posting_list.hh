#pragma once

#include "types.hh"

#include <string>
#include <tuple>
#include <vector>

class PostingList {
  public:
    struct PostingElement {
        uint _docID;
        uint _tf;
        inline bool operator<(const PostingElement& post) const {
            return _docID < post._docID;
        }
    };

  public:
    explicit PostingList(const std::string& aTerm, const float aIDF);
    explicit PostingList() = delete;
    PostingList(const PostingList&) = delete;
    PostingList(PostingList&&) = delete;
    PostingList& operator=(const PostingList&) = delete;
    PostingList& operator=(PostingList&&) = delete;
    ~PostingList();

  public:
    inline float getIDF() { return _idf; }
    // inline uint getTF(uint aDocID) { return }

  private:
    float _idf;
    std::vector<PostingElement> _posting; // has to be sorted
};
