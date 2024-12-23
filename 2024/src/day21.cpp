#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/pos.h>

struct Data
{
  std::vector<std::string> codes;
};

std::vector<std::string> numeric_keypad = {
  "789",
  "456",
  "123",
  " 0A"
};
std::map<char, Pos> numeric_keyapd_char_to_pos = {
  {'7', Pos{.x = 0, .y = 0}}, {'8', Pos{.x = 1, .y = 0}}, {'9', Pos{.x = 2, .y = 0}},
  {'4', Pos{.x = 0, .y = 1}}, {'5', Pos{.x = 1, .y = 1}}, {'6', Pos{.x = 2, .y = 1}},
  {'1', Pos{.x = 0, .y = 2}}, {'2', Pos{.x = 1, .y = 2}}, {'3', Pos{.x = 2, .y = 2}},
                              {'0', Pos{.x = 1, .y = 3}}, {'A', Pos{.x = 2, .y = 3}}
};

std::vector<std::string> directional_keypad = {
  " ^A",
  "<v>"
};
std::map<char, Pos> directional_keyapd_char_to_pos = {
  {'^', Pos{.x = 1, .y = 0}}, {'A', Pos{.x = 2, .y = 0}},
  {'<', Pos{.x = 0, .y = 1}}, {'v', Pos{.x = 1, .y = 1}}, {'>', Pos{.x = 2, .y = 1}}
};

bool in_bounds(int64_t x, int64_t y, const std::vector<std::string> &keypad)
{
  return x >= 0 && x < keypad[0].size() && y >= 0 && y < keypad.size();
}

using Vertices = std::vector<Pos>;
using Edges = std::map<Pos, std::vector<Pos>>;

std::pair<Vertices, Edges> build_graph(const std::vector<std::string> &keypad)
{
  std::vector<Pos> vertices;
  std::map<Pos, std::vector<Pos>> edges;

  for (int64_t j = 0; j < keypad.size(); ++j)
  {
    for (int64_t i = 0; i < keypad[j].size(); ++i)
    {
      if (keypad[j][i] != ' ')
      {
        vertices.push_back(Pos{.x = i, .y = j});
      }
    }
  }

  for (const auto &v : vertices)
  {
    for (const auto &dir : directions)
    {
      Pos next = v + dir;
      if (in_bounds(next.x, next.y, keypad) && keypad[next.y][next.x] != ' ')
      {
        edges[v].push_back(next);
      }
    }
  }
  return {vertices, edges};
}

// after writing ths, I realized that I could have just computed the manhattan distance 
// between each pair of vertices and stored that in a map
using Distances = std::map<Pos, std::map<Pos, int>>;
using Paths = std::map<Pos, std::map<Pos, Pos>>;
using DirectionChanges = Distances;

std::pair<Distances, Paths> floyd_warshall(const std::pair<Vertices, Edges>& graph)
{
  const auto& [vertices, edges] = graph;
  Distances dist;
  Paths prev;
  DirectionChanges direction_changes;

  for (const auto &u : vertices)
  {
    for (const auto &v : vertices)
    {
      dist[u][v] = INT_MAX;
      prev[u][v] = Pos{.x = -1, .y = -1};
      direction_changes[u][v] = 0;
    }
  }

  for (const auto &u : vertices)
  {
    for (const auto &v : edges.at(u))
    {
      dist[u][v] = 1;
      prev[u][v] = u;
      direction_changes[u][v] = 0;
    }
    dist[u][u] = 0;
    prev[u][u] = u;
    direction_changes[u][u] = 0;
  }

  for (const auto &k : vertices)
  {
    for (const auto &i : vertices)
    {
      for (const auto &j : vertices)
      {
        auto &dik = dist[i][k];
        auto &dkj = dist[k][j];
        auto &dij = dist[i][j];

        auto &dik_dir = direction_changes[i][k];
        auto &dkj_dir = direction_changes[k][j];
        auto &dij_dir = direction_changes[i][j];
        if (dik != INT_MAX && dkj != INT_MAX && dij > dik + dkj)
        {
          dij = dik + dkj;
          prev[i][j] = prev[k][j];
        }
      }
    }
  }

  return {dist, prev};
}

std::vector<Pos> shortest_path(const Pos &start, const Pos &end, const Paths &paths)
{
  std::vector<Pos> path;
  Pos current = end;
  while (current != start)
  {
    path.push_back(current);
    current = paths.at(start).at(current);
  }
  path.push_back(start);
  std::reverse(path.begin(), path.end());
  return path;
}

std::string get_path(std::string path, const Paths& shortest_paths, const Distances& distances, std::vector<std::string> keypad, std::map<char, Pos> char_to_pos) {
  Pos start = char_to_pos['A'];
  std::string numeric_path;
  Pos current = start;
  for (char c : path) {
    Pos next = char_to_pos[c];
    auto path = shortest_path(current, next, shortest_paths);
    for(size_t i = 0; i < path.size() - 1; ++i) {
      Pos dir = path[i + 1] - path[i];
      if (dir == up) {
        numeric_path += '^';
      } else if (dir == right) {
        numeric_path += '>';
      } else if (dir == down) {
        numeric_path += 'v';
      } else {
        numeric_path += '<';
      }
    }
    numeric_path.push_back('A');
    current = next;
  }
  return numeric_path;
}

int part1(const Data &data)
{
  auto [numeric_distances, numeric_paths] = floyd_warshall(build_graph(numeric_keypad));
  auto [directional_distances, directional_paths] = floyd_warshall(build_graph(directional_keypad));
  int n_robots = 2;

  for(const auto& code : data.codes) {
    std::cout << code << std::endl;
    auto numeric_path = get_path(code, numeric_paths, numeric_distances, numeric_keypad, numeric_keyapd_char_to_pos);
    for(auto c : numeric_path) {
      std::cout << c;
    }
    std::cout << " " << numeric_path.size() << std::endl;
    std::string directional_path = numeric_path;
    for(int i = 0; i < n_robots; ++i) {
      directional_path = get_path(directional_path, directional_paths, directional_distances, directional_keypad, directional_keyapd_char_to_pos);
      for(auto c : directional_path) {
        std::cout << c;
      }
      std::cout << " " << directional_path.size() << std::endl;
    }
    std::cout << std::endl;
  }

  return 0;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day21.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.codes.push_back(line);
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

  int answer1 = 0;
  int answer2 = 0;

  auto first = part1(data);
  std::cout << "Part 1: " << first << std::endl;

  auto second = part2(data);
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  for (int i = 1; i < argc; ++i)
  {
    if (std::string(argv[i]) == "--benchmark")
    {
      benchmark::Initialize(&argc, argv);
      benchmark::RunSpecifiedBenchmarks();
      return 0;
    }
  }
}
