#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/split.h>
#include <algorithm>

struct Data {
  int A;
  int B;
  int C;
  std::vector<int> instructions;
};

int part1(const Data &data)
{
  // Combo ops 
  // 0,1,2,3 -> 0,1,2,3
  // 4 -> A
  // 5 -> B
  // 6 -> C
  // 7 -> unused

  // operations
  // 0 (adv) -> division | A / pow(2,combo) stored in A 
  // 1 (bxl) -> bitwise XOR | B^combo stored in B
  // 2 (bst) -> combo % 8 stored in B
  // 3 (jnz) -> nothing if A is zero, else jump to combo and don't increment instruction pointer by 2
  // 4 (bxc) -> bitwise XOR | B^C stored in B, reads an operand but ignores it
  // 5 (out) -> output combo % 8
  // 6 (bdv) -> division | A / pow(2,combo) stored in B
  // 7 (cdv) -> division | A / pow(2,combo) stored in C

  int instruction_pointer = 0;
  int A = data.A;
  int B = data.B;
  int C = data.C;
  std::vector<int> output;

  while (instruction_pointer < data.instructions.size()-1) {
    int opcode = data.instructions[instruction_pointer];
    int literal = data.instructions[instruction_pointer + 1];
    int combo = literal;
    if (combo == 4) {
      combo = A;
    }
    else if (combo == 5) {
      combo = B;
    }
    else if (combo == 6) {
      combo = C;
    }

    switch (opcode) {
      case 0:
        A = A / (1 << combo);
        instruction_pointer += 2;
        break;
      case 1:
        B = B ^ literal;
        instruction_pointer += 2;
        break;
      case 2:
        B = combo % 8;
        instruction_pointer += 2;
        break;
      case 3: 
        instruction_pointer = A == 0 ? instruction_pointer + 2 : literal;
        break;
      case 4:
        B = B ^ C;
        instruction_pointer += 2;
        break;
      case 5:
        output.push_back(combo % 8);
        instruction_pointer += 2;
        break;
      case 6:
        B = A / (1 << combo);
        instruction_pointer += 2;
        break;
      case 7:
        C = A / (1 << combo);
        instruction_pointer += 2;
        break;
      default:
        throw std::runtime_error("Invalid opcode");
        break;
    }
  }

  std::cout << "A: " << A << std::endl;
  std::cout << "B: " << B << std::endl;
  std::cout << "C: " << C << std::endl;

  // join the output with commas
  std::string out = "";
  for (int i = 0; i < output.size(); ++i) {
    out += std::to_string(output[i]);
    if (i != output.size() - 1) {
      out += ",";
    }
  }
  std::cout << out << std::endl;

  return 0;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day17.txt")); // Update the file path
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    if (line.find("Register A:") != std::string::npos)
    {
      data.A = std::stoi(line.substr(line.find(":") + 1));
    }
    else if (line.find("Register B:") != std::string::npos)
    {
      data.B = std::stoi(line.substr(line.find(":") + 1));
    }
    else if (line.find("Register C:") != std::string::npos)
    {
      data.C = std::stoi(line.substr(line.find(":") + 1));
    }
    else if (line.find("Program:") != std::string::npos)
    {
      std::string program = line.substr(line.find(":") + 1);
      data.instructions = split_to_int(program, ",");
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
