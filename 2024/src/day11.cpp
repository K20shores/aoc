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

struct Node
{
  Node *left;
  Node *right;
  uint64_t value;
};

struct Data
{
  std::vector<int> stones;
};

Node *get_node(uint64_t new_value, std::map<uint64_t, std::unique_ptr<Node>> &splits)
{
  if (!splits.contains(new_value))
  {
    splits[new_value] = std::make_unique<Node>(Node{.left = nullptr, .right = nullptr, .value = new_value});
  }
  return splits[new_value].get();
}

uint64_t count_stones(int blinks, const Data& data) {
  std::map<uint64_t, std::unique_ptr<Node>> splits;
  uint64_t sum = 0;

  for (auto &stone : data.stones)
  {
    // stone value, depth
    std::queue<std::pair<uint64_t, uint64_t>> q;
    q.push({stone, 0});
    sum += 1;

    while (!q.empty())
    {
      auto val = q.front();
      Node *cur = get_node(val.first, splits);


      for (size_t blink = val.second; blink < blinks; ++blink)
      {
        while (cur->left != nullptr && blink < blinks)
        {
          if (cur->right != nullptr)
          {
            ++sum;
            q.push({cur->right->value, blink + 1});
          }
          ++blink;
          cur = cur->left;
        }
        if (blink >= blinks)
          break;
        uint64_t num_digits = std::floor(std::log10(cur->value)) + 1;
        if (cur->value == 0)
        {
          Node *next = get_node(1, splits);
          cur->left = next;
          cur = cur->left;
        }
        else if (num_digits % 2 == 0)
        {
          uint64_t right_half = cur->value % uint64_t(std::pow(10, num_digits / 2));
          uint64_t left_half = cur->value / std::pow(10, num_digits / 2);
          sum += 1;
          Node *left = get_node(left_half, splits);
          Node *right = get_node(right_half, splits);
          cur->left = left;
          cur->right = right;
          cur = cur->left;
          q.push({right_half, blink + 1});
        }
        else
        {
          Node *next = get_node(cur->value * 2024, splits);
          cur->left = next;
          cur = cur->left;
        }
      }
      q.pop();
    }
  }

  return sum;
}

uint64_t part1(const Data &data)
{
  return count_stones(25, data);
}

uint64_t part2(const Data &data)
{
  return count_stones(75, data);
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
    uint64_t s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    uint64_t s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  uint64_t answer1 = 198089;
  uint64_t answer2 = 0;

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
