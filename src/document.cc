#include "document.hh"

Document::Document(const size_t aDocID, const std::string& aPath) :
    _ID(aDocID),
    _content(aPath),
    _tfidf(nullptr)
{}

Document::~Document(){}

// todo
