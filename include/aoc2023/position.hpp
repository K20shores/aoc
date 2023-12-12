#pragma once

#include <stddef.h>

struct Pos
{
  size_t i;
  size_t j;

  bool operator==(const Pos &other) const
  {
    return (i == other.i) && (j == other.j);
  }

  bool operator!=(const Pos &other) const
  {
    return !(*this == other);
  }
};