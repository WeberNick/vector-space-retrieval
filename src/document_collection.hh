#pragma once

#include "infra/types.hh"
#include "document.hh"

#include <string>
#include <map>

class DocumentCollection
{
    public:
        typedef std::map<size_t, Document>      DocMap;
        typedef DocMap::iterator                DocMapIterator;
        typedef std::pair<size_t, Document>     DocMapElem;
        //typedef std::pair<DocMapIterator, bool> ...;

    public:
        explicit DocumentCollection() = delete;
        explicit DocumentCollection(const std::string& aRootDirectory); //path to root directory where the collection starts?
        DocumentCollection(const DocumentCollection&) = delete;
        DocumentCollection(DocumentCollection&&) = delete;
        DocumentCollection& operator=(const DocumentCollection&) = delete;
        DocumentCollection& operator=(DocumentCollection&&) = delete;
        ~DocumentCollection();
    
    public:
        /* start the scan for files at the root directory and add all found docs to map */
        void collect();


    public:
        /* The following are just wrapper functions for the respective map container calls */

        /* insert element into collection (by std::pair element) */
        bool            insert(const DocMapElem& aElement); 
        /* insert element into collection (by values) */
        bool            insert(const size_t aKey, const Document& aDocument)
        /* find element in collection */
        DocMapIterator  find(const size_t aKey);
        /* erase by key */
        bool            erase(const size_t aKey);
        /* erase by iterator */
        bool            erase(const DocMapIterator aIterator);



    public:
        //getter
        inline const DocMap&    getCollection(){ return _docs; }
        inline size_t           getNoDocuments(){ return _docs.size(); }
        //setter, if needed

    private:
        static size_t   _countID;
        DocMap          _docs;
};
