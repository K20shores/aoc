#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <unordered_map>
#include <array>
#include <set>

// Specialization of std::hash for std::array<int, 4>
namespace std
{
  template <>
  struct hash<std::array<int, 4>>
  {
    std::size_t operator()(const std::array<int, 4> &arr) const noexcept
    {
      std::size_t h = 0;
      for (int val : arr)
      {
        h ^= std::hash<int>{}(val) + 0x9e3779b9 + (h << 6) + (h >> 2);
      }
      return h;
    }
  };
}

struct Data
{
  std::vector<int64_t> values;
};

int64_t mix(int64_t secret, int64_t subject)
{
  return subject ^ secret;
}

int64_t prune(int64_t secret)
{
  return secret % 16777216;
}

int64_t transform(int64_t secret)
{
  secret = prune(mix(secret, secret * 64));
  secret = prune(mix(secret, secret / 32));
  secret = prune(mix(secret, secret * 2048));
  return secret;
}

int64_t part1(const Data &data)
{
  int64_t sum = 0;
  for (auto secret : data.values)
  {
    auto val = secret;
    for (int i = 0; i < 2000; ++i)
    {
      val = transform(val);
    }
    sum += val;
  }
  return sum;
}

using Changes = std::unordered_map<std::array<int, 4>, int>;
int64_t part2(const Data &data)
{
  std::unordered_map<std::array<int, 4>, int64_t> global_sums;

  for (size_t i = 0; i < data.values.size(); ++i)
  {
    Changes sequences;
    int price = data.values[i] % 10;
    int64_t val = data.values[i];
    int change = 0;
    std::array<int, 4> last_four_changes = {0, 0, 0, 0};

    for (int j = 0; j < 2000; ++j)
    {
      val = transform(val);
      change = val % 10 - price;
      price = val % 10;
      last_four_changes[j % 4] = change;

      if (j >= 3)
      {
        std::array<int, 4> key = {
            last_four_changes[(j - 3 + 0) % 4],
            last_four_changes[(j - 3 + 1) % 4],
            last_four_changes[(j - 3 + 2) % 4],
            last_four_changes[(j - 3 + 3) % 4]};

        if (sequences.find(key) == sequences.end())
        {
          sequences[key] = price;
          global_sums[key] += price;
        }
      }
    }
  }

  int64_t global_max = 0;
  for (const auto &[key, sum] : global_sums)
  {
    global_max = std::max(global_max, sum);
  }

  return global_max;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day22.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.values.push_back(std::stoll(line));
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

  int64_t answer1 = 12979353889;
  int64_t answer2 = 1449;

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
