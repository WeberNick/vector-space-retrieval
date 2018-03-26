#pragma once

#include "infra/types.hh"

#include <string>

class Document
{
    public:
        explicit Document() = delete;
        explicit Document(const size_t aDocID, const std::string& aPath);
        Document(const Document&) = delete;
        Document(Document&&) = delete;
        Document& operator=(const Document&) = delete;
        Document& operator=(Document&&) = delete;
        ~Document();
    
    public:
        /* read document/file into main memory */
        byte*   read();
        /* preprocess document (maybe overwrite old doc or store in new pre processed document */
        void    preprocess();

    public:
        //getter
        inline size_t       getID(){ return _ID; }
        inline std::string  getPath(){ return _path; }
        //setter, if needed

    private:
       size_t       _ID;
       std::string  _path;
};
