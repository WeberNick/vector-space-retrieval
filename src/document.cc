#include "document.hh"
#include "utility.hh"

size_t Document::_documentCount = 0;

Document::Document(const std::string& aDocID, const string_vt& aContent) :
    _ID(++Document::_documentCount),
    _docID(aDocID),
    _content(aContent),
    _term_tf_map()
{
    // this->pprintTFMap();
}

Document::~Document(){}

Document::Document(const Document& doc) : 
    _ID(doc.getID()),
    _docID(doc.getDocID()),
    _content(doc.getContent()),
    _term_tf_map(doc.getTermTFMap())
{}

void Document::pprintTFMap() {
    std::cout << "Document: " << this->getID() << std::endl;
    for (const auto& [term, tf] : _term_tf_map) {
        std::cout << "(" << term << ", " << tf << ") ";
    }
    std::cout << std::endl;
}