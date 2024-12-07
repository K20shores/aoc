#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <tuple>
#include <unordered_set>

struct Pos {
  int y = 0;
  int x = 0;

  Pos& operator+(const Pos other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
  }
  
  Pos& operator+=(const Pos& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
  }

  bool operator==(const Pos& other) const {
      return this->x == other.x && this->y == other.y;
  }
};

struct PosHash {
    size_t operator()(const Pos& pos) const {
        return std::hash<int>()(pos.y) ^ (std::hash<int>()(pos.x) << 1);
    }
};

struct PosPairHash {
    size_t operator()(const std::pair<Pos, Pos>& key) const {
        const auto& [pos, dir] = key;
        return std::hash<int>()(pos.y) ^ (std::hash<int>()(pos.x) << 1) ^ 
               (std::hash<int>()(dir.y) << 2) ^ (std::hash<int>()(dir.x) << 3);
    }
};

struct Data {
  std::vector<std::string> map;
  Pos p;
  Pos dir;
  std::unordered_set<Pos, PosHash> path;
  std::unordered_set<std::pair<Pos, Pos>, PosPairHash> position_direction_set;
};

Pos up = {-1, 0};
Pos down = {1, 0};
Pos left = {0, -1};
Pos right = {0, 1};

Pos next_dir(Pos original) {
  return Pos{.y = original.x, .x = -original.y};
}

char get_next_grid_char(const std::vector<std::string>& map, const Pos& p, const Pos& dir, const Pos& obstacle = {-1, -1}) {
  if (p.y + dir.y < map.size() && p.x + dir.x < map[0].size()) {
    if (p.y + dir.y == obstacle.y && p.x + dir.x == obstacle.x) {
      return '#';
    }
    return map[p.y + dir.y][p.x + dir.x];
  }
  return '\0';
}

int part1(Data &data)
{
  Pos p = data.p;
  Pos dir = data.dir;
  std::unordered_set<Pos, PosHash> visited;

  while (true) {
    char next = get_next_grid_char(data.map, p, dir);
    data.position_direction_set.insert({p, dir});
    data.path.insert(p);
    visited.insert(p);
    if (next == '\0') break;
    if (next == '#') {
      dir = next_dir(dir);
    }
    p += dir;
  }

  return visited.size();
}

int part2(const Data &data)
{
  return 0;
  int cycles = 0;
  auto path = data.path;

  // for (const auto& obstacle : path) {
    std::unordered_set<std::pair<Pos, Pos>, PosPairHash> visited;

    Pos p = data.p;
    Pos dir = data.dir;
    Pos o = {.y = p.y, .x = p.x - 1};

    // if (obstacle == p) continue; // ignore the startin location
    while (true) {
      char next = get_next_grid_char(data.map, p, dir, o);
      if (visited.find({p, dir}) == visited.end()) {
        // haven't been here before
        visited.insert({p, dir});
      }
      else {
        cycles += 1;
      }
      if (next == '\0') break;
      if (next == '#') {
        dir = next_dir(dir);
      }
      p += dir;
    }
  // }

  return cycles;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day6.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  size_t i = 0;
  while (std::getline(file, line))
  {
    data.map.push_back(line);
    auto p1 = line.find('^');
    auto p2 = line.find('v');
    auto p3 = line.find('>');
    auto p4 = line.find('<');
    auto a = {p1, p2, p3, p4};
    for(auto& p : a) {
      if (p != std::string::npos) {
        data.p.y = i;
        data.p.x = p;
        break;
      }
    }
    ++i;
  }

  char c = data.map[data.p.y][data.p.x];
  switch(c) {
    case '^':
      data.dir = up;
      break;
    case 'v':
      data.dir = down;
      break;
    case '>':
      data.dir = right;
      break;
    case '<':
      data.dir = left;
      break;
  }

  return data;
}

class BenchmarkFixture : public benchmark::Fixture
{
public:
  static Data data;
};

Data BenchmarkFixture::data = parse();

BENCHMARK_DEFINE_F(BenchmarkFixture, Part1Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    int s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    int s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 5312;
  int answer2 = 0;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--benchmark") {
      benchmark::Initialize(&argc, argv);
      benchmark::RunSpecifiedBenchmarks();
      return 0;
    }
  }
}
