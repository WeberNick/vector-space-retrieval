#pragma once

#include "infra/types.hh"
#include "posting.hh"

#include <map>
#include <string>
#include <vector>

class InvertedIndex
{
    public:
        //maybe change Posting to Posting* or Posting&
        typedef std::map<std::string, Posting>  PostingMap;
        typedef PostingMap::iterator            PostingMapIterator;

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
        bool                insert(const std::string& aTerm); 
        /* for each term in the list, add the doc ID to the posting list */
        /* find element in collection */
        PostingMapIterator  find(const std::string& aKey);
        /* erase by key */
        bool                erase(const std::string& aKey);
        /* erase by iterator */
        bool                erase(const PostingMapIterator aIterator);

    public:
        /* look for posting list of term  in map and return the vector containing the doc IDs*/
        const size_t_vt&    getDocIDs(const std::string& aTerm);
        /* add doc ID to the corresponding posting list */
        void                addTerm(const std::string& aTerm, const size_t aDocID);
        /* same as previous but add doc ID for multiple terms */
        void                addTerm(const string_vt& aTermList, const size_t aDocID);

    public:
        //getter
        inline const PostingMap&    getPostingList(){ return _postingList; }
        inline size_t               getNoPostings(){ return _postingList.size(); }
        //setter, if needed

    private:
        PostingMap  _postingLists;

};
