#pragma once

#include "document.hh"
#include "exception.hh"
#include "posting_list.hh"
#include "types.hh"
#include "utility.hh"

#include <map>
#include <string>

class InvertedIndex {
    friend class IndexManager;

  private:
    explicit InvertedIndex();
    InvertedIndex(const InvertedIndex&) = default;
    InvertedIndex(InvertedIndex&&) = delete;
    InvertedIndex& operator=(const InvertedIndex&) = delete;
    InvertedIndex& operator=(InvertedIndex&&) = delete;
    ~InvertedIndex();

  private:
    inline static InvertedIndex& getInstance() {
        static InvertedIndex instance;
        return instance;
    }
    void init();
    void init(const control_block_t& aCB);
    void buildInvertedIndex();

  public:
    /* if term is not in inverted index yet: create an empty posting list for the term (key) */
    bool insert(const std::string& aTerm, const PostingList& aPostingList);
    /* find element in collection */
    posting_map_iter_t find(const std::string& aKey);
    /* erase by key */
    void erase(const std::string& aKey);
    /* erase by iterator */
    void erase(const posting_map_iter_t aIterator);

  public:
    /* */
    const PostingList& getPostingList(const std::string& term);
    /* get size of posting list for the given term */
    size_t getNoDocs(const std::string& aTerm);
    /**
     * @brief Get the Postings object
     *
     * @return const postinglist_mt&
     */
    inline const postinglist_mt& getPostings() { return _term_posting_map; }

    /**
     * @brief Get the No Postings object
     *
     * @return size_t
     */
    inline size_t getNoPostings() { return _term_posting_map.size(); }

  private:
    const control_block_t* _cb;
    
    postinglist_mt _term_posting_map; // term, PostingList: [("Frodo", <PostingListObj>), ...]
};
