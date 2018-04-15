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
    inline static TieredIndex& getInstance() {
        static TieredIndex instance;
        return instance;
    }
    void init(const control_block_t& aControlBlock);
    void init();
    void buildTieredIndex(const doc_mt& aCollection);

  private:
    const control_block_t* _cb;
};
