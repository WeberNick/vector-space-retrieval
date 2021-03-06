#include "document.hh"

size_t Document::_documentCount = 0;

/**
 * @brief Construct a new Document:: Document object
 * 
 * @param aDocID the document ID of the document
 * @param aContent the content (vector of terms) of the document
 */
Document::Document(const std::string& aDocID, const string_vt& aContent) :
    _ID(Document::_documentCount++),
    _docID(aDocID),
    _content(aContent),
    _term_tf_map(),
    _tf_idf_vec(),
    _rand_proj_vec(),
    _norm_length(0)
{}

/**
 * @brief Construct a new Document:: Document object
 * 
 * @param doc the document to copy
 */
Document::Document(const Document& doc) : 
    _ID(doc.getID()),
    _docID(doc.getDocID()),
    _content(doc.getContent()),
    _term_tf_map(doc.getTermTfMap()),
    _tf_idf_vec(doc.getTfIdfVector()),
    _wordembeddings_vec(doc.getWordEmbeddingsVector()),
    _rand_proj_vec(doc.getRandProjVec()),
    _norm_length(doc.getNormLength())
{}

float Document::getTf(const std::string& aTerm) const {
    if (_term_tf_map.find(aTerm) != _term_tf_map.end())
        return _term_tf_map.at(aTerm);
    else
        return 0;
}

float Document::getTf(const std::string& aTerm){
    return static_cast<const Document&>(*this).getTf(aTerm);
}

std::ostream& operator<<(std::ostream& strm, const Document& doc) {
    strm << "Document: " << doc.getID() << ": ";
    std::string sep = ") ";
    for (auto it = doc.getTermTfMap().begin(); it != doc.getTermTfMap().end(); ++it) {
        if (it == std::prev(doc.getTermTfMap().end(), 1)) { sep = ")"; }
        strm << "(" << it->first << ", " << it->second << sep;
    }
    return strm;
}
