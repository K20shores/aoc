#include <unordered_set>

struct Pos
{
  int64_t y = 0;
  int64_t x = 0;

  Pos operator+(const Pos other)
  {
    return {.x = this->x + other.x, .y = this->y + other.y};
  }

  Pos &operator+=(const Pos &other)
  {
    this->x += other.x;
    this->y += other.y;
    return *this;
  }

  Pos operator-(const Pos &other)
  {
    return {.x = this->x - other.x, .y = this->y - other.y};
  }

  bool operator==(const Pos &other) const
  {
    return this->x == other.x && this->y == other.y;
  }

  bool operator<(const Pos &other) const
  {
    if (this->x != other.x)
    {
      return this->x < other.x;
    }
    return this->y < other.y;
  }
};

struct PosHash
{
  size_t operator()(const Pos &pos) const
  {
    return std::hash<int>()(pos.y) ^ (std::hash<int>()(pos.x) << 1);
  }
};

struct PosPairHash
{
  size_t operator()(const std::pair<Pos, Pos> &key) const
  {
    const auto &[pos, dir] = key;
    return std::hash<int>()(pos.y) ^ (std::hash<int>()(pos.x) << 1) ^
           (std::hash<int>()(dir.y) << 2) ^ (std::hash<int>()(dir.x) << 3);
  }
};
