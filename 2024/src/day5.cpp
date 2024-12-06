#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <map>

struct Data {
  std::map<int, std::vector<int>> orderings;
  std::vector<std::vector<int>> pages;
};

int part1(const Data &data)
{
  int sum = 0;
  for (auto page : data.pages) {
    bool correct = true;
    for(size_t i = 0; i < page.size(); ++i) {
      auto elem = page[i];
      if (data.orderings.find(elem) != data.orderings.end()) {
        for(auto& after : data.orderings.at(elem)) {
          for(size_t j = 0; j < i; ++j) {
            if (page[j] == after) {
              correct = false;
              goto done;
            }
          }
        }
      }
    }
    done:
    if (correct) {
      sum += page[page.size()/2];
      // std::cout << "correct: ";
    }
    else {
      // std::cout << "incorrect: ";
    }
    // for(auto& e: page) { 
    //   std::cout << e << " ";
    // }
    // std::cout << std::endl;
  }
  return sum;
}

int part2(const Data &data)
{
  return 0;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day5.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;
  bool fill_pages = false;;

  while (std::getline(file, line))
  {
    if (line.length() == 0)
    {
      fill_pages = true;
      continue;
    }

    if (fill_pages) {
      auto vals = split(line, ",");
      std::vector<int> temp = {};
      for (auto val : vals) {
        temp.push_back(std::stoi(val));
      }
      data.pages.push_back(temp);
    }
    else {
      auto vals = split(line, "|");
      int key = std::stoi(vals[0]);
      if (data.orderings.find(key) == data.orderings.end()) {
        data.orderings[key] = {};
      }
      data.orderings[key].push_back(std::stoi(vals[1]));
    }
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

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 5713;
  int answer2 = 0;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
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