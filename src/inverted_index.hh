#pragma once

#include "infra/types.hh"

#include <map>
#include <string>

class InvertedIndex {
public:
  // better: Hashing
  typedef std::map<std::string, size_t_vt> PostingMap;
  typedef PostingMap::iterator PostingMapIterator;

public:
  explicit InvertedIndex();
  InvertedIndex(const InvertedIndex&) = delete;
  InvertedIndex(InvertedIndex&&) = delete;
  InvertedIndex& operator=(const InvertedIndex&) = delete;
  InvertedIndex& operator=(InvertedIndex&&) = delete;
  ~InvertedIndex();

public:
  /* The following are just wrapper functions for the respective map container calls */

  /* if term is not in inverted index yet: create an empty posting list for the term (key) */
  bool insert(const std::string& aTerm);
  /* find element in collection */
  PostingMapIterator find(const std::string& aKey);
  /* erase by key */
  bool erase(const std::string& aKey);
  /* erase by iterator */
  bool erase(const PostingMapIterator aIterator);

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
  inline const PostingMap& getPostings() { return _postings; }
  inline size_t getNoPostings() { return _postings.size(); }

private:
  PostingMap _postings;
};
