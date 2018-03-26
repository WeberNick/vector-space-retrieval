#pragma once

#include "infra/types.hh"

#include <string>
#include <vector>

class Posting {
 public:
  typedef std::vector<Posting> posting_vt;

 public:
  explicit Posting() = delete;
  explicit Posting(const std::string& aTerm);
  explicit Posting(const std::string& aTerm,
                   const size_t_vt& aListOfIDs);  // for bulk load
  Posting(const Posting&) = delete;
  Posting(Posting&&) = delete;
  Posting& operator=(const Posting&) = delete;
  Posting& operator=(Posting&&) = delete;
  ~Posting();

 public:
  /* add a document ID to the posting list. immediately sort or implement sort
   * call?? */
  void addDocID(const size_t aDocID);
  /* return the i'th doc ID */
  size_t getDocID(const size_t aIndex);

 public:
  // getter
  inline const std::string& getTerm() { return _term; }
  inline size_t getNoDocIDs() { return _docIDs.size(); }
  // setter, if needed

 private:
  std::string _term;
  size_t_vt _docIDs;  // always has to be sorted
};
