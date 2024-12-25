#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/split.h>

enum Gate {
  AND,
  OR,
  XOR
};

std::string gate_to_string(Gate gate) {
  switch(gate) {
    case Gate::AND:
      return "AND";
    case Gate::OR:
      return "OR";
    case Gate::XOR:
      return "XOR";
    default:
      break;
  }
  return "unknown";
}

struct Operation {
  Gate gate;
  std::string arg1;
  std::string arg2;
  std::string target;
};

struct Data {
  std::map<std::string, int64_t> gates;
  std::vector<Operation> operations;
};

bool has_both_inputs(const Operation& op, const std::map<std::string, int64_t>& gates) {
  return gates.at(op.arg1) != -1 && gates.at(op.arg2) != -1;
}

void put_valid_gates_first(std::vector<Operation>& operations, const std::map<std::string, int64_t>& gates, size_t offset = 0) {
  std::sort(operations.begin() + offset, operations.end(), [&](const Operation& a, const Operation& b) {
    int valid_args_a = 0;
    valid_args_a += gates.at(a.arg1) != -1;
    valid_args_a += gates.at(a.arg2) != -1;
    int valid_args_b = 0;
    valid_args_b += gates.at(b.arg1) != -1;
    valid_args_b += gates.at(b.arg2) != -1;
    return valid_args_a > valid_args_b;
  });
}

int64_t form_number(const std::map<std::string, int64_t>& gates) {
  int64_t idx = 0;
  int64_t result = 0;
  for(auto& [gate, value] : gates) {
    if (gate[0] == 'z') {
      result |= value << idx;
      ++idx;
    }
  }
  return result;
}

int64_t part1(const Data &data)
{
  auto operations = data.operations;
  auto gates = data.gates;

  put_valid_gates_first(operations, gates);

  for(size_t i = 0; i < operations.size(); ++i) {
    auto& op = operations[i];
    if (has_both_inputs(op, gates)) {
      int arg1 = gates.at(op.arg1);
      int arg2 = gates.at(op.arg2);
      int result = 0;
      switch(op.gate) {
        case Gate::AND:
          result = arg1 & arg2;
          break;
        case Gate::OR:
          result = arg1 | arg2;
          break;
        case Gate::XOR:
          result = arg1 ^ arg2;
          break;
        default:
          break;
      }
      gates[op.target] = result;
    }
    else {
      put_valid_gates_first(operations, gates, i);
      --i;
    }
  }
  return form_number(gates);
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day24.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  bool gates = false;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      gates = true;
      continue;
    }
    if (!gates)
    {
      auto parts = split(line, ": ");
      data.gates[parts[0]] = std::stoi(parts[1]);
    }
    else
    {
      auto parts = split(line, " ");
      Operation op;
      op.arg1 = parts[0];
      op.arg2 = parts[2];
      op.target = parts[4];
      if (parts[1] == "AND")
      {
        op.gate = Gate::AND;
      }
      else if (parts[1] == "OR")
      {
        op.gate = Gate::OR;
      }
      else if (parts[1] == "XOR")
      {
        op.gate = Gate::XOR;
      }
      else
      {
        throw std::runtime_error("invalid gate");
      }
      if(data.gates.find(op.arg1) == data.gates.end()) {
        data.gates[op.arg1] = -1;
      }
      if(data.gates.find(op.arg2) == data.gates.end()) {
        data.gates[op.arg2] = -1;
      }
      if(data.gates.find(op.target) == data.gates.end()) {
        data.gates[op.target] = -1;
      }
      data.operations.push_back(op);
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

  int64_t answer1 = 45923082839246;
  int64_t answer2 = 0;

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
