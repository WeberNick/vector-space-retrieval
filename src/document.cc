#include "document.hh"

size_t Document::_documentCount = 0;

Document::Document(const std::string& aDocID, const string_vt& aContent) :
    _ID(++Document::_documentCount),
    _docID(aDocID),
    _content(aContent),
    _tf(nullptr)
{}

Document::~Document(){}

Document& Document::operator=(const Document& doc) {
    if (this != &doc) {
        _ID = doc._ID;
        _docID = doc._docID;
        _content = doc._content;
        _tf = doc._tf;
    }
    return *this;
}