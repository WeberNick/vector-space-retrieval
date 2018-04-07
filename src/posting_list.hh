#pragma once

#include "types.hh"

#include <tuple>
#include <vector>
#include <string>

class PostingList {
  public:
    typedef std::tuple<uint, uint> DocTFPair; // Pair<DocID, TF of term>
    typedef std::vector<DocTFPair> Posting;
  
  public:
    explicit PostingList(const std::string& aTerm, const float aIDF);
    explicit PostingList() = delete;
    PostingList(const PostingList&) = delete;
    PostingList(PostingList&&) = delete;
    PostingList& operator=(const PostingList&) = delete;
    PostingList& operator=(PostingList&&) = delete;
    //PostingList& operator<<(Posting& aPosting, DocTFPair aElem) {}; // insert and ensure sort by ID
    ~PostingList();

  private:
    std::string _term;
    float _idf;
    Posting* _posting;
};

