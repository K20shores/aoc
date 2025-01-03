#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <aoc/2023/maths.hpp>

struct Node {
  std::string val;
  Node *left = nullptr;
  Node *right = nullptr;

  Node(std::string v) : val(v) {}
};

struct Data {
  std::string directions;
  Node *root;
  std::vector<Node *> a_nodes;
  std::vector<Node *> nodes;

  ~Data() {
    for (auto &n : nodes) {
      delete n;
    }
  }
};

int part1(const Data &data) {
  int steps = 0;
  auto node = data.root;
  size_t direction = 0;
  while (node->val != "ZZZ") {
    if (data.directions[direction] == 'L') {
      node = node->left;
    } else {
      node = node->right;
    }
    direction = (direction + 1) % data.directions.size();
    ++steps;
  }
  return steps;
}

long part2(const Data &data) {
  std::vector<int> paths;
  for (Node *node : data.a_nodes) {
    int steps = 0;
    size_t direction = 0;
    while (node->val[2] != 'Z') {
      if (data.directions[direction] == 'L') {
        node = node->left;
      } else {
        node = node->right;
      }
      direction = (direction + 1) % data.directions.size();
      ++steps;
    }
    paths.push_back(steps);
  }
  size_t max = 0;
  for (auto p : paths) max = p > max ? p : max;
  const auto primes = get_primes(max);
  return lcm(paths, primes);
}

std::tuple<std::string, std::string, std::string> parse_node(const std::string &line) {
  std::tuple<std::string, std::string, std::string> node;
  for (size_t i = 0; i < line.size();) {
    size_t start = i;
    while ('A' <= line[i] && line[i] <= 'Z') {
      ++i;
    }
    if (std::get<0>(node).empty()) {
      std::get<0>(node) = line.substr(start, i - start);
    } else if (std::get<1>(node).empty()) {
      std::get<1>(node) = line.substr(start, i - start);
    } else if (std::get<2>(node).empty()) {
      std::get<2>(node) = line.substr(start, i - start);
    }
    while (line[i] == ' ' || line[i] == '=' || line[i] == '(' || line[i] == ',' || line[i] == ')') {
      ++i;
    }
  }
  return node;
}

void set_node_info(std::map<std::string, Node *> &m, const std::tuple<std::string, std::string, std::string> &node_info, Data &d) {
  auto node = std::get<0>(node_info);
  Node *root = nullptr;
  if (m.find(node) == m.end()) {
    root = new Node(node);
    d.nodes.push_back(root);
    m[node] = root;
  } else {
    root = m[node];
  }

  node = std::get<1>(node_info);
  auto it = m.find(node);
  if (it == m.end()) {
    auto left = new Node(node);
    d.nodes.push_back(left);
    m[node] = left;
    root->left = left;
  } else {
    root->left = (*it).second;
  }

  node = std::get<2>(node_info);
  it = m.find(node);
  if (it == m.end()) {
    auto right = new Node(node);
    d.nodes.push_back(right);
    m[node] = right;
    root->right = right;
  } else {
    root->right = (*it).second;
  }
}

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day8.txt"));
  std::string line;
  Data data;

  std::getline(file, line);
  data.directions = line;

  // skip the blank line
  std::getline(file, line);

  std::map<std::string, Node *> m;

  while (std::getline(file, line)) {
    auto node_info = parse_node(line);
    set_node_info(m, node_info, data);
  }

  auto aaa = m.find("AAA");
  if (aaa != m.end()) {
    data.root = m["AAA"];
  }

  for (auto &it : m) {
    if (it.second->val.ends_with('A')) {
      data.a_nodes.push_back(it.second);
    }
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

  std::cout << "Part 1: " << part1(data) << std::endl;
  std::cout << "Part 2: " << part2(data) << std::endl;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}