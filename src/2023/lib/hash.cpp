#include <aoc/2023/hash.hpp>

size_t hash(const std::vector<std::string> &strings) {
  std::hash<std::string> hasher;

  size_t combinedHash = 0;
  for (const auto &str : strings) {
    combinedHash ^= hasher(str) + 0x9e3779b9 + (combinedHash << 6) + (combinedHash >> 2);
  }

  return combinedHash;
}