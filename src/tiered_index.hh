#pragma once

#include "types.hh"

class TieredIndex {

public:
  explicit TieredIndex();
  TieredIndex(const TieredIndex&) = delete;
  TieredIndex(TieredIndex&&) = delete;
  TieredIndex& operator=(const TieredIndex&) = delete;
  TieredIndex& operator=(TieredIndex&&) = delete;
  ~TieredIndex();

private:
  // todo
};
