#include "tiered_index.hh"

TieredIndex::TieredIndex() {}

TieredIndex::~TieredIndex() {}

void TieredIndex::init(const control_block_t& aControlBlock) {
    _cb = &aControlBlock;
}
