#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/split.h>
#include <stack>
#include <queue>

struct Node {
  Node* left;
  Node* right;
  int value;
  int steps_to_split;
};

struct Data
{
  std::vector<int> stones;
};

int part1(const Data &data)
{
  std::map<int, std::unique_ptr<Node>> splits;
  int sum = 0;

  for (auto &stone : data.stones)
  {
    // stone value, depth
    std::queue<std::pair<int, int>> q;
    q.push({stone, 0});
    sum += 1;
    int n_steps = 6;

    while (!q.empty())
    {
      auto val = q.front();
      Node* cur = nullptr;

      if (!splits.contains(val.first)) {
        splits[val.first] = std::make_unique<Node>(Node{.left = nullptr, .right = nullptr, .value = val.first, .steps_to_split = 0});
      }
      cur = splits[val.first].get();

      for (size_t blink = val.second; blink < n_steps; ++blink)
      {
        if (splits.contains(val.first) && cur->left != nullptr)
        {
          cur = splits[val.first].get();
          if (cur->left != nullptr && cur->right != nullptr)
          {
            if (cur->steps_to_split + blink > n_steps)
            {
              break;
            }
            sum += 1;
            blink += cur->steps_to_split;
            cur = cur->left;
            val.first = cur->value;
            continue;
          }
        }

        int num_digits = std::floor(std::log10(val.first)) + 1;
        if (val.first == 0)
        {
          val.first = 1;
          cur->steps_to_split += 1;
        }
        else if (num_digits % 2 == 0)
        {
          int right_half = val.first % int(std::pow(10, num_digits / 2));
          val.first /= std::pow(10, num_digits / 2);
          sum += 1;
          if (!splits.contains(right_half))
          {
            splits[right_half] = std::make_unique<Node>(Node{.left = nullptr, .right = nullptr, .value = right_half, .steps_to_split = 0});
          }
          if (!splits.contains(val.first))
          {
            splits[val.first] = std::make_unique<Node>(Node{.left = nullptr, .right = nullptr, .value = val.first, .steps_to_split = 0});
          }
          cur->left = splits[val.first].get();
          cur->right = splits[right_half].get();
          cur = cur->left;
          q.push({right_half, blink + 1});
        }
        else
        {
          val.first *= 2024;
          cur->steps_to_split += 1;
        }
      }
      q.pop();
    }
  }

  return sum;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day11.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.stones = split_to_int(line, " ");
  }
  std::cout << "Parsed " << data.stones.size() << " stones" << std::endl;

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
