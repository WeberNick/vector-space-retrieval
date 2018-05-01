/**
 *	@file 	tiered_index.hh
 *	@author	Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *	@brief  Implements the tiered index
 *	@bugs 	Currently no bugs known
 *	@todos	Write DESCRIPTION
 *
 *	@section DESCRIPTION
 *	TODO
 */
#pragma once

#include "document.hh"
#include "posting_list.hh"
#include "types.hh"
#include "utility.hh"

class TieredIndex {
    friend class IndexManager;

  private:
    explicit TieredIndex();
    TieredIndex(const TieredIndex&) = default;
    TieredIndex(TieredIndex&&) = delete;
    TieredIndex& operator=(const TieredIndex&) = delete;
    TieredIndex& operator=(TieredIndex&&) = delete;
    ~TieredIndex();

  private:
    // TODO docs
    /**
     * @brief Insert aTierMap for aTerm, if this term is not in the tiered index yet
     *
     * @param aTerm
     * @param aTierMap
     * @return true
     * @return false
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
     * @brief initialize control block and tiered index
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
    // TODO docs
    /**
     * @brief Get the Posting Lists object
     *
     * @return str_tierplmap_mt&
     */
    inline str_tierplmap_mt& getPostingLists() { return _term_tier_map; }
    // TODO docs
    /**
     * @brief Get the Dictionary Size object
     *
     * @return size_t
     */
    inline size_t getDictionarySize() { return _term_tier_map.size(); }
    // TODO docs
    /**
     * @brief Get the Num Tiers object
     *
     * @return size_t
     */
    inline size_t getNumTiers() { return _num_tiers; }

    // TODO docs
    /**
     * @brief Get the Doc ID List object
     *
     * @param top at least top ids
     * @param terms
     * @return sizet_vt
     */
    sizet_vt getDocIDList(const size_t top, const string_vt& terms);
    /**
     * @brief Get the Posting List object
     *
     * @param aTerm
     * @param aTier
     * @return PostingList&
     */
    PostingList& getPostingList(const std::string& aTerm, const size_t aTier);
    // TODO docs
    /**
     * @brief Get the No Docs object
     *
     * @param aTerm
     * @param aTier
     * @return size_t
     */
    size_t getNoDocs(const std::string& aTerm, const size_t aTier);

  private:
    const control_block_t* _cb;

    bool _init;
    size_t _num_tiers;
    str_tierplmap_mt _term_tier_map;
};
