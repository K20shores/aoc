#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <iterator>
#include <cmath>
#include <map>

struct Data {
  std::vector<int> left;
  std::vector<int> right;
};

long part1(Data data)
{
  long acc = 0;
  std::sort(data.left.begin(), data.left.end());
  std::sort(data.right.begin(), data.right.end());
  for(size_t i = 0; i < data.left.size(); ++i) {
    acc += std::abs(data.left[i] - data.right[i]);
  }
  return acc;
}

long part2(const Data &data)
{
  std::map<int, int> counts;
  for(auto& e : data.right) {
    if (counts.count(e)) {
      // its in the map, increment
      counts[e] += 1;
    }
    else {
      // it needs to be initilized
      counts[e] = 1;
    }
  }
  long acc = 0;
  for (auto & e : data.left) {
    if (counts.count(e)) {
      // its in the map, increment
      acc += e * counts[e];
    }
  }
  return acc;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day1.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    std::istringstream buffer(line);
    std::vector<int> lists{std::istream_iterator<int>(buffer),
                             std::istream_iterator<int>()};
    data.left.push_back(lists[0]);
    data.right.push_back(lists[1]);
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

  long answer1 = 1258579;
  long answer2 = 23981443;

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