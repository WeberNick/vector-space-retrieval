#include "tiered_index.hh"

/**
 * @brief Construct a new Tiered Index:: Tiered Index object
 * 
 */
TieredIndex::TieredIndex() :
    _cb(nullptr),
    _init(false),
    _num_tiers()
{}

/**
 * @brief Destroy the Tiered Index:: Tiered Index object
 * 
 */
TieredIndex::~TieredIndex() {}

void TieredIndex::init(const control_block_t& aControlBlock) {
    _cb = &aControlBlock;
    if (!_init) {
        _num_tiers = _cb->tiers();
        _init = true;
    }
}
