#include <aoc/2023/hash.hpp>
#include <aoc/2023/position.hpp>
#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

struct Data {
  std::vector<std::string> path;
  Pos start;
};

long long mod(long long k, long long n) { return ((k %= n) < 0) ? k + n : k; }

std::vector<Pos> out_of_bounds_neighbors(const Pos &p, const std::vector<std::string> &path) {
  std::vector<Pos> directions = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

  std::vector<Pos> r;
  for (auto &direc : directions) {
    auto s = p + direc;
    if (s.i < 0 || s.i >= path.size() - 1 || s.j < 0 || s.j >= path[0].size() - 1) {
      r.push_back(direc);
    }
  }
  return r;
}

std::vector<Pos> neighbors(const Pos &p, const std::vector<std::string> &path) {
  std::vector<Pos> directions = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

  std::vector<Pos> r;
  for (auto &direc : directions) {
    auto s = p + direc;
    if (s.i >= 0 && s.i < path.size() && s.j >= 0 && s.j < path[0].size() && path[s.i][s.j] != '#') {
      r.push_back(direc);
    }
  }
  return r;
}

std::pair<int, std::set<std::pair<Pos, int>>> count(const std::vector<std::string> &path, Pos start, int max_depth = 10, int initial_depth = 0) {
  if (path[start.i][start.j] == '#') return {0, {}};
  std::vector<std::vector<bool>> visited(path.size(), std::vector<bool>(path[0].size(), false));
  std::queue<Pos> working_set;
  std::queue<int> depths;
  working_set.push(start);
  std::set<std::pair<Pos, int>> next_positions;

  depths.push(initial_depth);
  int plots_reached = 0;
  visited[start.i][start.j] = true;
  while (working_set.size() > 0) {
    Pos cur = working_set.front();
    working_set.pop();
    int depth = depths.front();
    depths.pop();

    if (depth % 2 == 0) ++plots_reached;

    for (auto &neighbor : neighbors(cur, path)) {
      auto next = cur + neighbor;
      if (!visited[next.i][next.j] && (depth + 1) <= max_depth) {
        visited[next.i][next.j] = true;
        working_set.push(next);
        depths.push(depth + 1);
      }
    }

    if (cur.i == 0 || cur.i == path.size() - 1 || cur.j == 0 || cur.j == path[0].size() - 1) {
      if (depth + 1 <= max_depth) {
        for (auto neighbor : out_of_bounds_neighbors(cur, path)) {
          auto next = cur + neighbor;
          next_positions.insert({{static_cast<long>(mod(next.i, path.size())), static_cast<long>(mod(next.j, path[0].size()))}, depth + 1});
        }
      }
    }
  }

  return {plots_reached, next_positions};
}

int part1(const Data &data, int max_depth = 64) { return count(data.path, data.start, max_depth).first; }

int part2(const Data &data, int max_depth = 10) {
  auto next = count(data.path, data.start, max_depth);
  std::queue<std::set<std::pair<Pos, int>>> next_positions;

  uint64_t total = next.first;
  next_positions.push(next.second);
  while (next_positions.size() > 0) {
    auto set = next_positions.front();
    next_positions.pop();
    for (const auto &p : set) {
      next = count(data.path, p.first, max_depth, p.second);
      total += next.first;
      next_positions.push(next.second);
    }
  }

  return total;
};

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day21.txt"));
  std::string line;
  Data data;

  long i = 0;
  while (std::getline(file, line)) {
    data.path.push_back(line);
    auto it = line.find('S');
    if (it != std::string::npos) {
      data.start = {i, static_cast<long>(it)};
    }
    ++i;
  }

  return data;
}

class BenchmarkFixture : public benchmark::Fixture {
public:
  static Data data;
};

Data BenchmarkFixture::data = parse();

BENCHMARK_DEFINE_F(BenchmarkFixture, Part1Benchmark)
(benchmark::State &state) {
  for (auto _ : state) {
    int s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state) {
  for (auto _ : state) {
    int s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark);

int main(int argc, char **argv) {
  Data data = parse();

  int answer1 = 3820;
  int answer2 = 0;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}