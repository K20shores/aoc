#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <list>
#include <format>

struct Data {
  std::string map;
  // id, space
  std::list<std::pair<int, int>> diskmap;
};

void formatted(auto list) {
  for(auto& p : list) {
    if (p.first == -1) {
      for(size_t i = 0; i < p.second; ++i) {
        std::cout << ".";
      }
    }
    else {
      for(size_t i = 0; i < p.second; ++i) {
        std::cout << std::format("{}", p.first);
      }
    }
  }
  std::cout << std::endl;
}

void print(auto list) {
  for(auto& p : list) {
    std::cout << std::format("({}, {}) ", p.first, p.second);
  }
  std::cout << std::endl;
  formatted(list);
}

int part1(const Data &data)
{
  auto diskmap = data.diskmap;
  print(diskmap);

  auto left = diskmap.begin();
  auto right = --diskmap.end();
  while (left != right) {
    while(left->first != -1) {
      ++left;
      if (left == right) break;
    }
    while(right->first == -1) {
      --right;
      if (left == right) break;
    }
    int remaining_space = left->second;
    int needed_space = right->second;
    if (remaining_space > needed_space) {
      // there is enough space to entirely fit the right block
      int new_space = remaining_space - needed_space;
      // set the id of this block
      left->first = right->first;
      // set its size
      left->second = right->second;
      // insert new blank space to the right
      diskmap.insert(std::next(left), {-1, new_space});

      // the block at the end is now unallocated
      right->first = -1;
    }
    else if {
      // we can only partially fill this block
      // assign the id
      left->first = right->first;
      // remove part of the space at the end
      right->second -= remaining_space;
      // and add the now blank space to the right
      diskmap.insert(std::next(right), {-1, remaining_space});
    }

    print(diskmap);
  }
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day9.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.map = line;
    for(size_t i = 0; i < line.size(); ++i) {
      if (i % 2 == 0) {
        data.diskmap.push_back({i/2, std::stoi(line.substr(i, 1))});
      }
      else {
        data.diskmap.push_back({-1, std::stoi(line.substr(i, 1))});
      }
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

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kSecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kSecond);

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