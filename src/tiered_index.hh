/**
 *	@file 	tiered_index.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  Implements the tiered index
 *
 *	@section DESCRIPTION docto_
 */
#pragma once

#include "types.hh"
#include "trace.hh"
#include "exception.hh"
#include "ir_util.hh"
#include "document.hh"
#include "posting_list.hh"

class TieredIndex {
    friend class IndexManager;

  private:
    explicit TieredIndex();
    TieredIndex(const TieredIndex&) = default;
    TieredIndex(TieredIndex&&) = delete;
    TieredIndex& operator=(const TieredIndex&) = delete;
    TieredIndex& operator=(TieredIndex&&) = delete;
    ~TieredIndex() = default;

  private:
    /**
     * @brief Insert aTierMap for aTerm, if this term is not in the tiered index yet
     *
     * @param aTerm the term to insert the tierMap for
     * @param aTierMap the tierMap to insert
     * @return whether the insertion was successful
     */
    bool insert(const std::string& aTerm, const std::map<size_t, PostingList>& aTierMap);
    /**
     * @brief Find the map of tiers to postingLists for aKey and return an iterator
     *
     * @param aKey the term to find in the map
     * @return tiered_posting_map_iter_t the map<tier, postingList>::iterator for aKey (the term)
     */
    tiered_posting_map_iter_t find(const std::string& aKey);
    /**
     * @brief Erase the TieredIndex entry of aKey
     *
     * @param aKey the term to erase
     */
    void erase(const std::string& aKey);
    /**
     * @brief Erase the postingList for aIterator
     *
     * @param aIterator the iterator to erase with
     */
    void erase(const tiered_posting_map_iter_t aIterator);

    /**
     * @brief Get the TieredIndex Singleton instance.
     *
     * @return TieredIndex& a reference to the TieredIndex Singleton instance.
     */
    inline static TieredIndex& getInstance() {
        static TieredIndex instance;
        return instance;
    }
    /**
     * @brief Initialize control block and tiered index
     *
     * @param aControlBlock the control block
     */
    void init(const control_block_t& aControlBlock);
    
    /**
     * @brief Get the Term Tier Posting Map object
     * 
     * @return str_tierplmap_mt* 
     */
    inline str_tierplmap_mt* getTermTierPostingMap() { return &_term_tier_map; }

  public:
    /**
     * @brief Get the posting lists
     *
     * @return str_tierplmap_mt& the posting lists
     */
    inline const str_tierplmap_mt& getPostingLists() const { return _term_tier_map; }
    /**
     * @brief Get the dictionary size
     *
     * @return size_t the dictionary size
     */
    inline size_t getDictionarySize() { return _term_tier_map.size(); }
    /**
     * @brief Get the number of tiers
     *
     * @return size_t the number of tiers
     */
    inline size_t getNumTiers() { return _num_tiers; }

    /**
     * @brief Get the top doc ids
     *
     * @param top the requested amount of results (ids)
     * @param terms the query terms for which to retrieve the ids
     * @return sizet_vt the ids to return
     */
    sizet_vt getDocIDList(const size_t top, const string_vt& terms) const;
    /**
     * @brief Get the Posting List object
     *
     * @param aTerm
     * @param aTier
     * @return PostingList&
     */
    const PostingList& getPostingList(const std::string& aTerm, const size_t aTier) const;
    /**
     * @brief Get the number of documents of a term in a tier
     *
     * @param aTerm the term
     * @param aTier the tier
     * @return size_t the number of docs for this term in this tier
     */
    size_t getNoDocs(const std::string& aTerm, const size_t aTier);
    
    /**
     * @brief Override operator<< for pretty printing a tiered index
     *
     * @param strm the output stream
     * @param ti the tiered index
     * @return std::ostream& the modified output stream
     */
    friend std::ostream& operator<<(std::ostream& strm, const TieredIndex& ti);

  private:
    const control_block_t* _cb;
    size_t _num_tiers;
    str_tierplmap_mt _term_tier_map;
};
