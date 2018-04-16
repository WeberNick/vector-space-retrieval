#include "tiered_index.hh"

/**
 * @brief Construct a new Tiered Index:: Tiered Index object
 * 
 */
TieredIndex::TieredIndex() :
    _cb(nullptr),
    _init(false)
{}

/**
 * @brief Destroy the Tiered Index:: Tiered Index object
 * 
 */
TieredIndex::~TieredIndex() {}

void TieredIndex::init(const control_block_t& aControlBlock) {
    _cb = &aControlBlock;
    if (!_init) {
        // init of tiered index
        _init = true;
    }
}
