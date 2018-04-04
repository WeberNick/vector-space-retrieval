#include "document.hh"

Document::Document(const size_t aID, const std::string& aDocID, const std::string& aContent) :
    _ID(aID),
    _docID(aDocID),
    _content(aContent),
    _tf(nullptr)
{}

Document::~Document(){}

// todo
