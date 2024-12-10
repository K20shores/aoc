#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/pos.h>
#include <map>
#include <queue>
#include <algorithm>

struct Data {
  std::vector<Pos> zeros;
  std::vector<std::vector<int>> map;
};

bool is_in_grid(const Pos& p, const Data& data) {
  return p.x >= 0 && p.y >= 0 && p.x < data.map[0].size() && p.y < data.map.size();
}

std::pair<std::set<Pos>, int> find_nines(
    Pos cur, 
    std::map<Pos, std::pair<std::set<Pos>, int>>& leads_to_nine, 
    const Data& data) {
  if (leads_to_nine.contains(cur)) {
    return leads_to_nine[cur];
  }

  int cur_val = data.map[cur.y][cur.x];
  if (cur_val == 9) {
    leads_to_nine[cur] = {{cur}, 1};
    return leads_to_nine[cur];
  }

  leads_to_nine[cur] = {};
  for(auto& dir: directions) {
    auto next = cur + dir;
    if (is_in_grid(next, data) && data.map[next.y][next.x] - 1 == cur_val) {
      auto result = find_nines(next, leads_to_nine, data);
      leads_to_nine[cur].first.insert(result.first.begin(), result.first.end());
      leads_to_nine[cur].second += result.second;
    }
  }

  return leads_to_nine[cur];
}

int part1(const Data &data)
{
  std::map<Pos, std::pair<std::set<Pos>, int>> leads_to_nine;

  int sum = 0;
  for(auto& zero : data.zeros) {
    sum += find_nines(zero, leads_to_nine, data).first.size();
  }
  return sum;
}

int part2(const Data &data)
{
  std::map<Pos, std::pair<std::set<Pos>, int>> leads_to_nine;

  int sum = 0;
  for(auto& zero : data.zeros) {
    sum += find_nines(zero, leads_to_nine, data).second;
  }
  return sum;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day10.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  int64_t row = 0;
  while (std::getline(file, line))
  {
    std::vector<int> row_data;
    for(int64_t col = 0; col < line.size(); ++col) {
      char c = line[col];
      row_data.push_back(c - '0');
      if (c == '0') {
        data.zeros.push_back(Pos{.x = col, .y = row});
      }
    }
    data.map.push_back(row_data);
    ++row;
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

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMicrosecond);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 574;
  int answer2 = 1238;

  auto first = part1(data);
  std::cout << "Part 1: " << first << std::endl;
  
  auto second = part2(data);
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
