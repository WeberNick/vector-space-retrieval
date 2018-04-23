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
    inline size_t getNumTiers() { return _num_tiers; }

  private:
    const control_block_t* _cb;

    bool _init;
    size_t _num_tiers;
};
