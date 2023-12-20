#include <aoc2023/get_char.hpp>
#include <aoc2023/position.hpp>
#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <string>
#include <vector>

struct Node {
  int step_length;
  Pos direction;
};

struct Data {
  std::vector<std::string> grid;
  std::vector<std::vector<Node>> nodes;
};

void print_path(Pos start, Pos end, const Data &d, const std::vector<std::vector<int>> &dist, const std::vector<std::vector<Pos>> &prev) {
  std::stack<Pos> path;
  Pos cur = end;
  int i = 0;
  while (cur != start) {
    i += d.grid[cur.i][cur.j] - '0';
    path.push(cur);
    cur = prev[cur.i][cur.j];
  }
  path.push(start);
  while (!path.empty()) {
    auto s = path.top();
    path.pop();
    std::cout << s << " " << std::endl;
  }
  std::cout << i << std::endl;
}

std::vector<Pos> neighbors(const Pos &p, const Data &d) {
  std::vector<Pos> directions = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

  std::vector<Pos> r;
  auto cur = d.nodes[p.i][p.j];
  Pos reverse = {-1 * cur.direction.i, -1 * cur.direction.j};
  for (auto &direc : directions) {
    auto s = p + direc;
    if (s.i >= 0 && s.i < d.grid.size() && s.j >= 0 && s.j < d.grid[0].size()) {
      if (cur.direction == direc && cur.step_length <= 3) {
        r.push_back(direc);
      } else if (cur.direction != direc && direc != reverse) {
        r.push_back(direc);
      }
    }
  }
  return r;
}

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<Pos>>> dijkstra(Data d) {
  std::vector<std::vector<int>> dist(d.grid.size(), std::vector<int>(d.grid[0].size(), std::numeric_limits<int>::max()));
  std::vector<std::vector<Pos>> prev(d.grid.size(), std::vector<Pos>(d.grid[0].size(), {-1, -1}));

  dist[0][0] = 0;
  auto compare = [&dist](const Pos &a, const Pos &b) { return dist[a.i][a.j] > dist[b.i][b.j]; };

  std::vector<Pos> open_set;

  for (long i = 0; i < d.grid.size(); ++i) {
    for (long j = 0; j < d.grid[0].size(); ++j) {
      open_set.push_back({i, j});
    }
  }

  std::make_heap(open_set.begin(), open_set.end(), compare);

  while (!open_set.empty()) {
    std::pop_heap(open_set.begin(), open_set.end(), compare);
    auto u = open_set.back();
    open_set.pop_back();

    for (auto &next : neighbors(u, d)) {
      auto v = u + next;
      auto it = std::find(open_set.begin(), open_set.end(), v);
      if (it == open_set.end()) continue;

      int alt = dist[u.i][u.j] + (d.grid[v.i][v.j] - '0');
      if (alt < dist[v.i][v.j]) {
        dist[v.i][v.j] = alt;
        prev[v.i][v.j] = u;
        if (d.nodes[u.i][u.j].direction == next) {
          d.nodes[v.i][v.j].step_length = d.nodes[u.i][u.j].step_length + 1;
        } else {
          d.nodes[v.i][v.j].step_length = 1;
        }
        d.nodes[v.i][v.j].direction = next;
      }
    }
    std::make_heap(open_set.begin(), open_set.end(), compare);
  }

  return {dist, prev};
}

int part1(const Data &data) {
  auto s = dijkstra(data);
  std::vector<Pos> path;
  path.push_back({0, 0});
  // print_path({0, 0}, {long(data.grid.size() - 1), long(data.grid[0].size() - 1)}, data, s.first, s.second);
  return 0;
}

int part2(const Data &data) { return 0; }

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day17.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line)) {
    data.grid.push_back(line);
    std::vector<Node> nodes;
    for (auto &c : line) {
      Node n;
      n.step_length = 0;
      n.direction = {0, 0};
      nodes.push_back(n);
    }
    data.nodes.push_back(nodes);
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

  int answer1 = 0;
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