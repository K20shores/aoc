#include <unordered_set>
#include <vector>

struct Pos
{
  int64_t x = 0;
  int64_t y = 0;

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

  Pos operator*(const int& x) {
    return {.x = this->x * x, .y = this->y * x};
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

Pos up = {.x = 0, .y = -1};
Pos down = {.x = 0, .y = 1};
Pos left = {.x = -1, .y = 0};
Pos right = {.x = 1, .y = 0};

Pos N = up;
Pos S = down;
Pos E = right;
Pos W = left;
Pos NE = up + right;
Pos NW = up + left;
Pos SE = down + right;
Pos SW = down + left;

std::vector<Pos> directions = {up, down, left, right};