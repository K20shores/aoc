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

struct Data {
  std::map<std::string, Node*> nodes;
  std::map<int, Node*> nodes_by_id;

  ~Data() {
    for (auto node : nodes) {
      delete node.second;
    }
  }
};

using AdjacencyMatrix = std::vector<std::vector<int>>;

// int DFS(const Data& data, const AdjacencyMatrix& graph, std::vector<int>& visited, int n_cycles, int start) {
//   int count = 0;
//   // vertex, cycles, vertex with t in its name
//   std::stack<std::tuple<int, int, bool>> stack;

//   stack.push({start, n_cycles, data.nodes_by_id.at(start)->name_has_t});

//   while(!stack.empty()) {
//     auto& [node, cycles, has_t] = stack.top();
//     visited[node] = 1;
//     stack.pop();
//     if (cycles == 0) {
//       visited[node] = 0;
//       if(graph[node][start] == graph[start][node] && has_t) {
//         ++count;
//       }
//     }

//     for(int i = 0; i < graph.size(); ++i) {
//       if (graph[node][i] == 1 && visited[i] == 0) {
//         stack.push({i, cycles - 1, has_t | data.nodes_by_id.at(i)->name_has_t});
//       }
//     }
//   }

//   return count;
// }

struct Args {
  int start;
  int vert;
  int n;
  bool has_t;
};

void DFS(const Data& data, const AdjacencyMatrix& graph, std::vector<int>& marked, int& count, Args args, int depth = 1) {
  marked[args.vert] = 1;

  if (args.n == 0) {
    marked[args.vert] = 0;
    if(graph[args.vert][args.start] == graph[args.start][args.vert] && args.has_t) {
      ++count;
    }
    return;
  }

  for(auto& neighbor : data.nodes_by_id.at(args.vert)->neighbors) {
    if (marked[neighbor->id] == 0) {
      std::string indent(2*depth, ' ');
      std::cout << indent << neighbor->name << std::endl;
      Args newargs = {
        .start = args.start,
        .vert = neighbor->id,
        .n = args.n - 1,
        .has_t = args.has_t || neighbor->name_has_t
      };
      DFS(data, graph, marked, count, newargs, depth + 1);
    }
  }
  marked[args.vert] = 0;
}

int count_cycles(const Data& data, const AdjacencyMatrix& graph, int n_cycles) {
  std::vector<int> visited(graph.size(), 0);
  int count = 0;
  for(const auto& [id, node] : data.nodes_by_id) {
    bool d = node->name == "co";
    std::cout << node->name << std::endl;
    Args args = {
      .vert = id,
      .start = id,
      .n = n_cycles - 1,
      .has_t = node->name_has_t
    };
    DFS(data, graph, visited, count, args);
    visited[id] = 1;
    std::cout << std::endl;
  }
  return count / 2;
}

int part1(const Data &data)
{
  AdjacencyMatrix graph(data.nodes.size(), std::vector<int>(data.nodes.size(), 0));
  for(const auto& [id, node] : data.nodes_by_id) {
    for(auto neighbor : node->neighbors) {
      graph[id][neighbor->id] = 1;
    }
  }

  return count_cycles(data, graph, 3);
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
      n1->name_has_t = n1->name.find('t') != std::string::npos;
      data.nodes[parts[0]] = n1;
      data.nodes_by_id[n1->id] = n1;
    } else {
      n1 = data.nodes[parts[0]];
    }
    if (data.nodes.find(parts[1]) == data.nodes.end()) {
      n2 = new Node();
      n2->name = parts[1];
      n2->id = id++;
      n2->name_has_t = n2->name.find('t') != std::string::npos;
      data.nodes[parts[1]] = n2;
      data.nodes_by_id[n2->id] = n2;
    } else {
      n2 = data.nodes[parts[1]];
    }
    n1->neighbors.push_back(n2);
    n2->neighbors.push_back(n1);
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

  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--benchmark") {
      benchmark::Initialize(&argc, argv);
      benchmark::RunSpecifiedBenchmarks();
      return 0;
    }
  }
}
