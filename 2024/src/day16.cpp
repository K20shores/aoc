#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/pos.h>
#include <queue>
#include <map>
#include <set>

struct Data {
  Pos start;
  Pos end;
  std::vector<std::string> grid;
};

bool in_bounds(const Pos &pos, const Data &data)
{
  return pos.x >= 0 && pos.x < data.grid[0].size() && pos.y >= 0 && pos.y < data.grid.size();
}

struct State {
    Pos pos;
    Pos dir;
    bool operator<(const State& other) const {
        return std::tie(pos.x, pos.y, dir.x, dir.y) < std::tie(other.pos.x, other.pos.y, other.dir.x, other.dir.y);
    }
};

// int64_t count(std::map<State, std:)

std::pair<int64_t, int64_t> djikstra(const Data &data)
{
  int64_t width = data.grid[0].size();
  int64_t height = data.grid.size();
  std::priority_queue<std::pair<int64_t, State>, std::vector<std::pair<int64_t, State>>, std::greater<>> pq;
  std::map<State, int64_t> dist;
  std::map<State, std::vector<State>> prev;
  std::set<State> visited;

  State start = State{.pos = data.start, .dir = E};
  pq.emplace(0, start);
  dist[start] = 0;
  State end_state;

  int64_t distance = 0;
  while (!pq.empty()) {
    auto elem = pq.top();
    pq.pop();
    int64_t cur_dist = elem.first;
    State u = elem.second;
    visited.insert(u);

    if (u.pos == data.end) {
      end_state = u;
      distance = cur_dist;
      break;
    }

    for(auto& new_dir : directions) {
      State v = {.pos = u.pos + new_dir, .dir = new_dir};

      if (!in_bounds(v.pos, data) || visited.contains(v) || data.grid[v.pos.y][v.pos.x] == '#') {
        continue;
      }

      int64_t new_dist = cur_dist;
      if (new_dir != u.dir) {
        new_dist += 1001;
      } else {
        new_dist += 1;
      }
      if (!dist.contains(v) || new_dist < dist[v]) {
        dist[v] = new_dist;
        prev[v] = {u};
        pq.emplace(new_dist, v);
      }
      else if (new_dist == dist[v]) {
        prev[v].push_back(u);
      }
    }
  }

  std::vector<State> stack;
  stack.push_back(end_state);
  std::set<Pos> visited_pos;
  while (stack.size() != 0 && stack[0].pos != data.start) {
    visited_pos.insert(stack.back().pos);
    auto cur = stack.back();
    stack.pop_back();
    for(auto& prev_state : prev[cur]) {
      stack.push_back(prev_state);
    }
  }

  return {distance, visited_pos.size()};
}

int part1(const Data &data)
{
  auto [distance, count] = djikstra(data);
  return distance;
}


int part2(const Data &data)
{
  auto [distance, count] = djikstra(data);
  return count;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day16.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  int64_t j = 0;
  while (std::getline(file, line))
  {
    data.grid.push_back(line);
    auto E = line.find("E");
    auto S = line.find("S");
    if (E != std::string::npos)
    {
      data.end = Pos{.x = static_cast<int64_t>(E), .y = j};
    }
    if (S != std::string::npos)
    {
      data.start = Pos{.x = static_cast<int64_t>(S), .y = j};
    }
    ++j;
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

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 133584;
  int answer2 = 622;

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
