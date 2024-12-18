#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/pos.h>
#include <aoc/2024/split.h>
#include <map>
#include <set>

struct Data {
  std::vector<Pos> positions;
  Pos start;
  Pos end;
};

bool in_bounds(const Pos &pos, int width, int height)
{
  return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

struct State {
    Pos pos;
    Pos dir;
    bool operator<(const State& other) const {
        return std::tie(pos.x, pos.y, dir.x, dir.y) < std::tie(other.pos.x, other.pos.y, other.dir.x, other.dir.y);
    }
};

int64_t djikstra(const Data& data, int64_t width, int64_t height, int64_t limit)
{
  std::set<Pos> occupied;
  for(size_t i = 0; i < limit; ++i) {
    occupied.insert(data.positions[i]);
  }
  std::priority_queue<std::pair<int64_t, State>, std::vector<std::pair<int64_t, State>>, std::greater<>> pq;
  std::map<State, int64_t> dist;
  std::map<State, std::vector<State>> prev;
  std::set<State> visited;

  State start = State{.pos = data.start, .dir = E};
  pq.emplace(0, start);
  dist[start] = 0;

  int64_t distance = 0;
  while (!pq.empty()) {
    auto elem = pq.top();
    pq.pop();
    int64_t cur_dist = elem.first;
    State u = elem.second;
    visited.insert(u);

    if (u.pos == data.end) {
      distance = cur_dist;
      break;
    }

    for(auto& new_dir : directions) {
      State v = {.pos = u.pos + new_dir, .dir = new_dir};

      if (!in_bounds(v.pos, width, height) || visited.contains(v) || occupied.contains(v.pos)) {
        continue;
      }

      int64_t new_dist = cur_dist + 1;
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

  return distance;
}

int part1(const Data &data)
{
  auto copy = data;
  int64_t width = 71;
  int64_t height = 71;
  copy.start = Pos{.x = 0, .y = 0};
  copy.end = Pos{.x = width - 1, .y = height - 1};
  auto distance = djikstra(copy, width, height, 1024);
  return distance;
}

std::string part2(const Data &data)
{
  auto copy = data;
  int64_t width = 71;
  int64_t height = 71;
  copy.start = Pos{.x = 0, .y = 0};
  copy.end = Pos{.x = width - 1, .y = height - 1};

  int left = 0;
  int right = data.positions.size();
  while (left <= right) {
    int m = std::floor((left + right) / 2);
    auto distance = djikstra(copy, width, height, m);
    if (distance > 0) {
      left = m + 1;
    }
    else if (distance == 0) {
      right = m - 1;
    }
  }
  std::string position = "";
  position += std::format("{},{}", data.positions[right].x, data.positions[right].y);
  return position;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day18.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    auto location = split_to_int(line, ",");
    data.positions.push_back(Pos{.x = location[0], .y = location[1]});
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
    auto s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    auto s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 296;
  std::string answer2 = "28,44";

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