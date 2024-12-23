#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/split.h>
#include <stack>
#include <tuple>

struct Node {
  std::string name;
  bool name_has_t;
  std::vector<Node *> neighbors;
  int id;
};
using AdjacencyMatrix = std::vector<std::vector<int>>;

struct Data {
  std::map<std::string, Node*> nodes;
  std::map<int, Node*> nodes_by_id;
  AdjacencyMatrix graph;

  ~Data() {
    for (auto node : nodes) {
      delete node.second;
    }
  }
};

AdjacencyMatrix cube(const AdjacencyMatrix& A) {
  AdjacencyMatrix result = A;
  for(int multiplications = 0; multiplications < 2; ++multiplications) {
    AdjacencyMatrix temp(result.size(), std::vector<int>(result.size(), 0));
    for(size_t i = 0; i < result.size(); ++i) {
      for(size_t j = 0; j < result.size(); ++j) {
        for(size_t k = 0; k < result.size(); ++k) {
          temp[i][j] += result[i][k] * A[k][j];
        }
      }
    }
    result = temp;
  }
  return result;
}

int triangles(const AdjacencyMatrix& A) {
  int trace = 0;
  for(int i = 0; i < A.size(); ++i) {
    trace += A[i][i];
  }
  return trace / 6;
}

std::vector<std::tuple<int, int, int>> findTriangles(const AdjacencyMatrix& A, const AdjacencyMatrix& A3) {
    std::vector<std::tuple<int, int, int>> triangles;
    size_t n = A.size();

    // Find triangles
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (A3[i][j] > 0 && A[i][j] == 1) {
                for (size_t k = j + 1; k < n; ++k) {
                    if (A[j][k] == 1 && A[k][i] == 1) { 
                        triangles.emplace_back(i, j, k);
                    }
                }
            }
        }
    }

    return triangles;
}

int part1(const Data &data)
{
  int sum = 0;
  auto cubed = cube(data.graph);
  // std::cout << std::format("Triangles: {}\n", triangles(cubed));
  // for(auto& i : cubed) {
  //   for(auto j : i) {
  //     std::cout << std::format("{:>3}", j);
  //   }
  //   std::cout << std::endl;
  // }
  auto triangles = findTriangles(data.graph, cubed);
  for(auto& [i, j, k] : triangles) {
    bool has_t = data.nodes_by_id.at(i)->name_has_t || data.nodes_by_id.at(j)->name_has_t || data.nodes_by_id.at(k)->name_has_t;
    sum += has_t ? 1 : 0;
  }
  return sum;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day23.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  int id = 0;
  while (std::getline(file, line))
  {
    std::vector<std::string> parts = split(line, "-");
    Node* n1 = nullptr;
    Node* n2 = nullptr;
    if (data.nodes.find(parts[0]) == data.nodes.end()) {
      n1 = new Node();
      n1->name = parts[0];
      n1->id = id++;
      n1->name_has_t = n1->name[0] == 't';
      data.nodes[parts[0]] = n1;
      data.nodes_by_id[n1->id] = n1;
    } else {
      n1 = data.nodes[parts[0]];
    }
    if (data.nodes.find(parts[1]) == data.nodes.end()) {
      n2 = new Node();
      n2->name = parts[1];
      n2->id = id++;
      n2->name_has_t = n2->name[0] == 't';
      data.nodes[parts[1]] = n2;
      data.nodes_by_id[n2->id] = n2;
    } else {
      n2 = data.nodes[parts[1]];
    }
    n1->neighbors.push_back(n2);
    n2->neighbors.push_back(n1);
  }

  AdjacencyMatrix graph(data.nodes.size(), std::vector<int>(data.nodes.size(), 0));
  for(const auto& [id, node] : data.nodes_by_id) {
    for(auto neighbor : node->neighbors) {
      graph[id][neighbor->id] = 1;
    }
  }
  data.graph = graph;

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

  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--benchmark") {
      benchmark::Initialize(&argc, argv);
      benchmark::RunSpecifiedBenchmarks();
      return 0;
    }
  }
}
