#include "tiered_index.hh"

TieredIndex::TieredIndex() {}

TieredIndex::~TieredIndex() {}

void TieredIndex::init(const control_block_t& aCB) {
    _cb = &aCB;
}
