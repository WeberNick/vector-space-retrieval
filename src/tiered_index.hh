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
#include "types.hh"
#include "posting_list.hh"

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
    /**
     * @brief Insert an empty posting list for aTerm, if this term is not in the inverted index yet
     *
     * @param aTerm the term to insert
     * @param aPostingList the posting list to insert
     * @return true if insertion was successful
     * @return false if insertion failed
     */
    //bool insert(const std::string& aTerm, const PostingList& aPostingList);
    /**
     * @brief Find a postingList with aKey and return an iterator
     *
     * @param aKey the term to find in the map
     * @return posting_map_iter_t the postingList for aKey (the term)
     */
    //posting_map_iter_t find(const std::string& aKey);
    /**
     * @brief Erase the postingList of aKey
     *
     * @param aKey the term to erase
     */
    //void erase(const std::string& aKey);
    /**
     * @brief Erase the postingList for aIterator
     *
     * @param aIterator the iterator to erase with
     */
    //void erase(const posting_map_iter_t aIterator);

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

  public:
    // getPostingList(term, tier)
    // getNoDocs(term, tier)
    // getPostingLists() return _term_tier_map
    // getDictionarySize()
    inline size_t getNumTiers() { return _num_tiers; }
    /**
     * @brief Get the Doc I D List object
     * 
     * @param top at least top ids
     * @param terms 
     * @return sizet_vt 
     */
    sizet_vt getDocIDList(uint top, string_vt& terms);

  private:
    const control_block_t* _cb;

    bool _init;
    size_t _num_tiers;
    str_tierplmap_mt _term_tier_map;
};
