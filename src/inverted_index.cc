#include "inverted_index.hh"

/**
 * @brief Construct a new Inverted Index:: Inverted Index object
 * 
 */
InvertedIndex::InvertedIndex() : 
    _cb(nullptr),
    _init(false),
    _term_posting_map()
{}

/**
 * @brief Destroy the Inverted Index:: Inverted Index object
 * 
 */
InvertedIndex::~InvertedIndex()
{}

void InvertedIndex::init(const control_block_t& aControlBlock) {
    if (!_init) {
        _cb = &aControlBlock;
        _init = true;
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

std::ostream& operator<<(std::ostream& strm, const InvertedIndex& ii) {
    std::cout << "H";
    strm << "[";
    std::string sepout = "\n";
    const auto& tpm = ii.getPostingLists();
    for (auto ito = tpm.begin(); ito != tpm.end(); ++ito) {
        if (ito == std::prev(tpm.end(), 1)) { sepout = ""; }
        std::string term = ito->first;
        const PostingList& pl = ito->second;
        strm << term << " -> " << pl << sepout;
        /*std::string& sep = ") ";
        for (auto it = pl.begin(); it != pl.end(); ++it) {
            if (it == std::prev(pl.end(), 1)) { sep = ")"; }
            strm << "(" << it->first << ", " << it->second << sep;
        }
        strm << "]" << sepout << std::endl;*/
    }
    return strm << "]" << std::endl;
}