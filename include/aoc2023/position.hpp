#pragma once

#include <stddef.h>
#include <iostream>

struct Pos {
  long i;
  long j;

  bool operator==(const Pos &other) const { return (i == other.i) && (j == other.j); }

  bool operator!=(const Pos &other) const { return !(*this == other); }

  Pos operator+(const Pos &other) const {
    Pos result;
    result.i = this->i + other.i;
    result.j = this->j + other.j;
    return result;
  }

  Pos operator-(const Pos &other) const {
    Pos result;
    result.i = this->i - other.i;
    result.j = this->j - other.j;
    return result;
  }

  Pos &operator+=(const Pos &other) {
    this->i += other.i;
    this->j += other.j;
    return *this;
  }

  Pos &operator-=(const Pos &other) {
    this->i -= other.i;
    this->j -= other.j;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &os, const Pos &pos) {
    os << "(" << pos.i << ", " << pos.j << ")";
    return os;
  }
};