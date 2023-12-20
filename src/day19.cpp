#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Part {
  int x;
  int m;
  int a;
  int s;
};

struct PartSpan {
  uint64_t x_min;
  uint64_t x_max;
  uint64_t m_min;
  uint64_t m_max;
  uint64_t a_min;
  uint64_t a_max;
  uint64_t s_min;
  uint64_t s_max;
};

struct Condition {
  int val;
  std::function<bool(int, int)> comparator;
  char which;
  std::string location;
};

struct Data {
  std::vector<Part> parts;
  std::map<std::string, std::vector<Condition>> workflows;
};

long count(const Part &part, const Data &data) {
  auto it = data.workflows.find("in");
  while (true) {
    for (const auto &con : it->second) {
      if (con.which == '\0') {
        if (con.location == "A") {
          return part.x + part.m + part.a + part.s;
        } else if (con.location == "R") {
          return 0;
        } else {
          it = data.workflows.find(con.location);
          break;
        }
      } else {
        if (con.which == 'x' && con.comparator(part.x, con.val)) {
          it = data.workflows.find(con.location);
          break;
        } else if (con.which == 'm' && con.comparator(part.m, con.val)) {
          it = data.workflows.find(con.location);
          break;
        } else if (con.which == 'a' && con.comparator(part.a, con.val)) {
          it = data.workflows.find(con.location);
          break;
        } else if (con.which == 's' && con.comparator(part.s, con.val)) {
          it = data.workflows.find(con.location);
          break;
        }
      }
    }
  }
}

int part1(const Data &data) {
  uint64_t sum = 0;
  for (const auto &part : data.parts) {
    sum += count(part, data);
  }
  return sum;
}

uint64_t part2(const Data &data) {
  using ConstMapIterator = std::map<std::string, std::vector<Condition>>::const_iterator;
  std::vector<PartSpan> parts;
  std::queue<std::pair<PartSpan, ConstMapIterator>> set;
  PartSpan range = {
      .x_min = 1,
      .x_max = 4000,
      .m_min = 1,
      .m_max = 4000,
      .a_min = 1,
      .a_max = 4000,
      .s_min = 1,
      .s_max = 4000,
  };
  auto in = data.workflows.find("in");
  set.push({range, in});
  while (set.size() > 0) {
    // auto [part, it] = set.front();
    auto p = set.front();
    auto part = p.first;
    auto it = p.second;
    set.pop();
    bool done = false;
    while (!done) {
      for (auto &con : it->second) {
        if (con.which == '\0') {
          if (con.location == "A") {
            parts.push_back(part);
            done = true;
            break;
          } else if (con.location == "R") {
            done = true;
            break;
          } else {
            it = data.workflows.find(con.location);
            break;
          }
        } else {
          if (con.which == 'x') {
            if (con.comparator.target<std::less<int>>() && con.comparator(part.x_min, con.val)) {
              PartSpan next = part;
              part.x_max = con.val - 1;
              next.x_min = con.val;
              set.push({next, it});
              it = data.workflows.find(con.location);
              break;
            } else if (con.comparator.target<std::greater<int>>() && con.comparator(part.x_max, con.val)) {
              PartSpan next = part;
              part.x_min = con.val + 1;
              next.x_max = con.val;
              set.push({next, it});
              it = data.workflows.find(con.location);
              break;
            }
          } else if (con.which == 'm') {
            if (con.comparator.target<std::less<int>>() && con.comparator(part.m_min, con.val)) {
              PartSpan next = part;
              part.m_max = con.val - 1;
              next.m_min = con.val;
              set.push({next, it});
              it = data.workflows.find(con.location);
              break;
            } else if (con.comparator.target<std::greater<int>>() && con.comparator(part.m_max, con.val)) {
              PartSpan next = part;
              part.m_min = con.val + 1;
              next.m_max = con.val;
              set.push({next, it});
              it = data.workflows.find(con.location);
              break;
            }
          } else if (con.which == 'a') {
            if (con.comparator.target<std::less<int>>() && con.comparator(part.a_min, con.val)) {
              PartSpan next = part;
              part.a_max = con.val - 1;
              next.a_min = con.val;
              set.push({next, it});
              it = data.workflows.find(con.location);
              break;
            } else if (con.comparator.target<std::greater<int>>() && con.comparator(part.a_max, con.val)) {
              PartSpan next = part;
              part.a_min = con.val + 1;
              next.a_max = con.val;
              set.push({next, it});
              it = data.workflows.find(con.location);
              break;
            }
          } else if (con.which == 's') {
            if (con.comparator.target<std::less<int>>() && con.comparator(part.s_min, con.val)) {
              PartSpan next = part;
              part.s_max = con.val - 1;
              next.s_min = con.val;
              set.push({next, it});
              it = data.workflows.find(con.location);
              break;
            } else if (con.comparator.target<std::greater<int>>() && con.comparator(part.s_max, con.val)) {
              PartSpan next = part;
              part.s_min = con.val + 1;
              next.s_max = con.val;
              set.push({next, it});
              it = data.workflows.find(con.location);
              break;
            }
          }
        }
      }
    }
  }
  uint64_t combinations = 0;
  for(auto& part : parts) {
    uint64_t combos = 
    (part.x_max - part.x_min + 1) *
    (part.m_max - part.m_min + 1) *
    (part.a_max - part.a_min + 1) *
    (part.s_max - part.s_min + 1);
    combinations += combos;
  }
  return combinations;
}

Condition parse_condition(size_t &i, const std::string &line) {
  Condition con;
  con.which = line[i++];
  char op = line[i++];
  switch (op) {
    case '>':
      con.comparator = std::greater<int>();
      break;
    case '<':
      con.comparator = std::less<int>();
      break;
  }
  size_t j = i;
  while (line[i] != ':') ++i;
  con.val = std::stoi(line.substr(j, i - j));
  ++i;
  j = i;
  while (line[i] != ',' && line[i] != '}') ++i;
  con.location = line.substr(j, i - j);
  return con;
}

std::vector<int> parse_numbers(const std::string &line) {
  std::vector<int> numbers;
  size_t num_start = -1;
  for (size_t i = 0; i <= line.size(); ++i) {
    if (std::isdigit(line[i])) {
      if (num_start == -1) {
        num_start = i;
      }
    } else if (num_start != -1) {
      numbers.push_back(std::stol(line.substr(num_start, i - num_start)));
      num_start = -1;
    }
  }
  return numbers;
}

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day19.txt"));
  std::string line;
  Data data;

  bool parsing_conditions = true;
  while (std::getline(file, line)) {
    if (line.size() == 0) {
      parsing_conditions = false;
      continue;
    }
    if (parsing_conditions) {
      std::vector<Condition> conditions;
      size_t i = 0;
      while (line[i] != '{') ++i;
      std::string label = line.substr(0, i);
      ++i;
      while (i <= line.size()) {
        char c = line[i];
        if ((c == 'x' || c == 'm' || c == 'a' || c == 's') && (line[i + 1] == '<' || line[i + 1] == '>')) {
          conditions.push_back(parse_condition(i, line));
          continue;
        }
        if (c == ',') {
          ++i;
          continue;
        }
        if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) {
          size_t j = i;
          while (line[i] != '}') ++i;
          Condition con;
          con.which = '\0';
          con.location = line.substr(j, i - j);
          conditions.push_back(con);
        }
        ++i;
      }
      data.workflows[label] = conditions;
    } else {
      auto v = parse_numbers(line);
      data.parts.push_back({.x = v[0], .m = v[1], .a = v[2], .s = v[3]});
    }
  }
  Condition done;
  done.which = '\0';
  done.location = "A";
  Condition reject;
  reject.which = '\0';
  reject.location = "R";
  data.workflows["A"] = {done};
  data.workflows["R"] = {reject};

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

  int answer1 = 374873;
  uint64_t answer2 = 122112157518711;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}