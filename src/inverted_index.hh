#pragma once

#include "exception.hh"
#include "posting_list.hh"
#include "document.hh"
#include "types.hh"

#include <map>
#include <string>

class InvertedIndex {
  public:
    explicit InvertedIndex();
    InvertedIndex(const InvertedIndex&) = delete;
    InvertedIndex(InvertedIndex&&) = delete;
    InvertedIndex& operator=(const InvertedIndex&) = delete;
    InvertedIndex& operator=(InvertedIndex&&) = delete;
    ~InvertedIndex();

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
    /* look for posting list of term  in map and return the vector containing the doc IDs*/
    const size_t_vt& getDocIDs(const std::string& aTerm);
    /* add a document ID to the posting list for the given term. immediately sort */
    void addDocID(const std::string& aTerm, const size_t aDocID);
    /* same as previous but add doc ID for multiple terms */
    void addDocIDToTerms(const string_vt& aTermList, const size_t aDocID);
    /* return the i'th doc ID for the given term */
    void getDocID(const std::string& aTerm, size_t aIndex);
    /* get size of posting list for the given term */
    size_t getNoDocs(const std::string& aTerm);

  public:
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

    /**
     * @brief Get the Posting List object
     *
     * @param term
     * @return const PostingList&
     * @throw InvalidArgumentException if there is no PostingList for the given term
     */
    inline const PostingList& getPostingList(const std::string& term) {
        if (_term_posting_map.find(term) != _term_posting_map.end())
            return _term_posting_map.at(term);
        else
            throw InvalidArgumentException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "The term " + term + " doest not appear in the document Collection.");
    };

  private:
    postinglist_mt _term_posting_map;
};
