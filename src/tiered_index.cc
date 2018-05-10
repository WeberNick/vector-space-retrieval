#include "tiered_index.hh"

/**
 * @brief Construct a new Tiered Index:: Tiered Index object
 *
 */
TieredIndex::TieredIndex() : _cb(nullptr), _init(false), _num_tiers(), _term_tier_map() {}

/**
 * @brief Destroy the Tiered Index:: Tiered Index object
 *
 */
TieredIndex::~TieredIndex() {}

void TieredIndex::init(const control_block_t& aControlBlock) {
    if (!_init) {
        _cb = &aControlBlock;
        _num_tiers = _cb->tiers();
        _init = true;
    }
}

bool TieredIndex::insert(const std::string& aTerm, const std::map<size_t, PostingList>& aTierMap) {
    return _term_tier_map.insert(std::make_pair(aTerm, aTierMap)).second;
}

tiered_posting_map_iter_t TieredIndex::find(const std::string& aKey) { return _term_tier_map.find(aKey); }

void TieredIndex::erase(const std::string& aKey) { _term_tier_map.erase(aKey); }

void TieredIndex::erase(const tiered_posting_map_iter_t aIterator) { _term_tier_map.erase(aIterator); }

sizet_vt TieredIndex::getDocIDList(const size_t top, const string_vt& terms) const {
    sizet_vt qids;
    size_t tier = 0;

    std::vector<sizet_vt> vecs;
    vecs.resize(terms.size());
    do {
        for (size_t i = 0; i < terms.size(); ++i) {
            try {
                const PostingList& pl2 = this->getPostingList(terms.at(i), tier);
                const sizet_vt& tierIDs = pl2.getIDs();
                sizet_vt& termIDs = vecs.at(i);
                termIDs.insert(termIDs.end(), tierIDs.begin(), tierIDs.end());
                vecs.at(i) = termIDs;

            } catch (const InvalidArgumentException& e) { continue; /* One of the (query) terms does not appear in the document collection. */ }
        }

        Utility::IR::mergePostingLists(vecs, qids);
    } while (qids.size() < top && ++tier < _num_tiers);
    return qids; // may return < top if all tiers are processed and we did not find enough qualifying ids
}

const PostingList& TieredIndex::getPostingList(const std::string& aTerm, const size_t aTier) const {
    if (_term_tier_map.find(aTerm) != _term_tier_map.end())
        return _term_tier_map.at(aTerm).at(aTier);
    else
        throw InvalidArgumentException(FLF, "The term " + aTerm + " does not appear in the document collection.");
}

size_t TieredIndex::getNoDocs(const std::string& aTerm, const size_t aTier) {
    if (_term_tier_map.find(aTerm) != _term_tier_map.end())
        return _term_tier_map.at(aTerm).at(aTier).getPosting().size();
    else
        throw InvalidArgumentException(FLF, "The term " + aTerm + " does not appear in the document collection.");
}

std::ostream& operator<<(std::ostream& strm, const TieredIndex& ti) {
    auto& ttpm = ti.getPostingLists();
    for (auto itt = ttpm.begin(); itt != ttpm.end(); ++itt) {
        std::string termt = itt->first;
        const std::map<size_t, PostingList>& tierplmap = itt->second;
        strm << termt << " -> [ ";
        for (auto itm = tierplmap.begin(); itm != tierplmap.end(); ++itm) {
            size_t tier = itm->first;
            const PostingList& pl = itm->second;
            strm << "T" << tier << ": " << pl << " ";
        }
        strm << "]"
             << "\n";
    }
    return strm;
}
