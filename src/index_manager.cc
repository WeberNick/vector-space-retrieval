#include "index_manager.hh"

IndexManager::IndexManager() :
    _cb(nullptr),
    _invertedIndex(InvertedIndex::getInstance()),
    _tieredIndex(TieredIndex::getInstance()),
    _clusteredIndex(Cluster::getInstance())
{}

IndexManager::~IndexManager() {}

void IndexManager::init(const control_block_t& aCB) {
    _cb = &aCB;
    _invertedIndex.init(aCB);
    _tieredIndex.init(aCB);
    _clusteredIndex.init(aCB);
}