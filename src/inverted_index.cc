#include "inverted_index.hh"

/**
 * @brief Construct a new Inverted Index:: Inverted Index object
 * 
 */
InvertedIndex::InvertedIndex() : 
    _cb(nullptr),
    _term_posting_map()
{}

void InvertedIndex::init(const control_block_t& aControlBlock) {
    if (!_cb) {
        _cb = &aControlBlock;
        TRACE("InvertedIndex: Initialized");
    }
}

bool InvertedIndex::insert(const std::string& aTerm, const PostingList& aPostingList) {
    return _term_posting_map.insert(std::make_pair(aTerm, aPostingList)).second; // copy constructor required
}

posting_map_iter_t InvertedIndex::find(const std::string& aKey) {
    return _term_posting_map.find(aKey);
}

void InvertedIndex::erase(const std::string& aKey) {
    _term_posting_map.erase(aKey);
}

void InvertedIndex::erase(const posting_map_iter_t aIterator) {
    _term_posting_map.erase(aIterator);
}

const PostingList& InvertedIndex::getPostingList(const std::string& aTerm) const {
    if (_term_posting_map.find(aTerm) != _term_posting_map.end())
        return _term_posting_map.at(aTerm);
    else
        throw InvalidArgumentException(FLF, "The term " + aTerm + " does not appear in the document collection.");
}

size_t InvertedIndex::getNoDocs(const std::string& aTerm) {
    if (_term_posting_map.find(aTerm) != _term_posting_map.end())
        return _term_posting_map.at(aTerm).getPosting().size();
    else
        throw InvalidArgumentException(FLF, "The term " + aTerm + " does not appear in the document collection.");
}

sizet_vt InvertedIndex::getDocIDList(const string_vt& terms) const {
    sizet_vt qids;

    std::vector<sizet_vt> vecs;
    vecs.resize(terms.size());
    for (size_t i = 0; i < terms.size(); ++i) {
        try {
            const sizet_vt& ids = this->getPostingList(terms.at(i)).getIDs();
            sizet_vt& termIDs = vecs.at(i);
            termIDs.insert(termIDs.end(), ids.begin(), ids.end());
            vecs.at(i) = termIDs;
        } catch (const InvalidArgumentException& e) { continue; /* One of the (query) terms does not appear in the document collection. */ }
    }
    Util::orPostingLists(vecs, qids);
    return qids;
}

std::ostream& operator<<(std::ostream& strm, const InvertedIndex& ii) {
    std::string sepout = "\n";
    const auto& tpm = ii.getPostingLists();
    for (auto ito = tpm.begin(); ito != tpm.end(); ++ito) {
        std::string term = ito->first;
        const PostingList& pl = ito->second;
        strm << term << " -> " << pl << sepout;
    }
    return strm;
}
