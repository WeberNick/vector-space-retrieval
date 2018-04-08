#include "document.hh"

size_t Document::_documentCount = 0;

Document::Document(const std::string& aDocID, const std::string& aContent) :
    _ID(++Document::_documentCount),
    _docID(aDocID),
    _content(aContent),
    _tf(nullptr)
{}

Document::~Document(){}

// todo
