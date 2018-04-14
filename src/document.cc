#include "document.hh"
#include "utility.hh"

size_t Document::_documentCount = 0;

Document::Document(const std::string& aDocID, const string_vt& aContent) :
    _ID(++Document::_documentCount),
    _docID(aDocID),
    _content(aContent),
    _term_tf_map()
{
    this->buildTFMap();
}

Document::~Document(){}

Document::Document(const Document& doc) : 
    _ID(doc.getID()),
    _docID(doc.getDocID()),
    _content(doc.getContent()),
    _term_tf_map(doc.getTermTFMap())
{}

void Document::buildTFMap() { Utility::IR::calcTFMap(_content, _term_tf_map); }