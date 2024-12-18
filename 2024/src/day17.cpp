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
  int64_t A;
  int64_t B;
  int64_t C;
  std::vector<int> instructions;
};

std::vector<int> run_program(const Data& data) {
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
  int64_t A = data.A;
  int64_t B = data.B;
  int64_t C = data.C;
  std::vector<int> output;

  while (instruction_pointer < data.instructions.size()-1) {
    int opcode = data.instructions[instruction_pointer];
    int64_t literal = data.instructions[instruction_pointer + 1];
    int64_t combo = literal;
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

  // std::cout << "A: " << A << std::endl;
  // std::cout << "B: " << B << std::endl;
  // std::cout << "C: " << C << std::endl;

  return output;
}

std::string get_output(std::vector<int> output) {
  std::string out = "";
  for (int i = 0; i < output.size(); ++i) {
    out += std::to_string(output[i]);
    if (i != output.size() - 1) {
      out += ",";
    }
  }
  return out;
}

std::string part1(const Data &data)
{
  return get_output(run_program(data));
}

// heavily (completey) inspired by https://topaz.github.io/paste/#XQAAAQAjAgAAAAAAAAAS4CTwfhtFt1Qb+x3BSmLp5uxYPyC7AVaFaltWuoSiaAWIrsTjczalah0h2YfY6pRB/PlgSLpOkdcBqnm6toPvsyQMeHE4iI21hKacBeqkdH3Dj+grdt7/RFWOR2mo2oSi23gbMJ3SMo1QUmtM/DB2Y8LyWxLm/rZ4tO9W70xVRwt7zNhASE/2gGtN162jU4ZKDeCsqM47eTJaopwkf2wEkSkC9atK9o8cWiWThotyvHLAk52oaOppsc3fJHXvNsVnfVE+ECj8T24//skBKwPPS4XOSe569mlczThL/bw9h00QfWYJbKrdMUMboaRuE86CA1kLSnnED0oLxLJETvFoJqAsnqsk4Jj9FGJH7Fqyf7kgtRbfdIphdsSS+g28en8XgnP/+az4cA==
// and https://github.com/jda5/advent-of-code-2024/blob/main/17/main.py
// and https://topaz.github.io/paste/#XQAAAQD9DwAAAAAAAAARmknGRw8TogB3Oxzri2QTFyHNmsUnNApHN7ntt1+q1p0T20pOn4cQIK/sBI68iGRoU2USsJyZkZC7tLU/hn171Mh4tkCA5b9dQMSP/cVOaCzPQP0AniHC1uXzbdSI1udQPQQTlG/e8+YJaejWPYYsbbflQO0jzrFA4Hs4zC8DykyXGPU4pQWvslp8jUfd/ZyjmnC2Z12/u4gF76+LDwiRxQ7yKM8I4e74JE/IeBNYz3Do1LoqtIf1ptAFAK7aakfkUDns7m2nf3m9WHSZHuFnlJqGx5fWo566cMezpQAcF6/zONCBBEA+E3mA70I8NGjUOscLukdUn2s9U+t8IM8fM6F2iEemPQCB4JUOuWxmlueNQyf7FO/69qXp4knLbxXJmzlJKwAeQvn+gzuqINQXGF4va+1W19t2iUpir3uJ0PdJcVlEKdYr7tKyQjSfji7Muga6M4JksgItQvqutrmONAg0OpLgi6zClvSvJKlKHTgYZIwGuS/HLfH+ZSZSee1CSh4xZcHBNzbEaecWKViUN+AplTkwYKDckia0TZ5QXYZJ56UqPQQWwaQstSK6GlAeXpEth6cIXIoFM/KS4jFbehH76edsfkYVxe+YYU7JOoOfCaq9zBP0Ne2DMSPn9JCTQh3s0LQQZAc/iGq6IWRb1zeV37Guh9ORe6f8rV7idpQE5EVbIsAOS/73WyHwES2MsTIHQ+j07d9YdMGfd+uL83OuxDvk5WoxHGVfA581QWzLa9xeT3ymoPUZuIoB9AZTBXJfrYNUbAjAmvXVDgz2keYZLgf8x+eORolAqCbBGDHTWTr33xy7WhHZrnOCqG1h9pk3Q9V0mqLi+4AstYFGaR5uxnREF5971/z3ajcBKiiSCb3PSI1NHKqO/sJlbdjaLeU3/Jcp1rL6DlxKEoTekqhqpn0Fh/BBmUJGUv5K7ni32ddB/As3rnpl1bnkVVcFm0li8DoaS/AHv3ntNMdLiX88OMjNz5TZAe6YzW9J+ObT2DoYO6cpemvhSRciB7VJOshMY4+tt+jQnoqsMQLLAIVvYaA9HN23XJe2Kw8SLkSF/JKy0CQJ0kK3CUZvEc8Mvx8KFskp+fAyj7CREuWBm+KNCzmm8X6I8xh67BCSdICY4uOUPYw3pBpvGqOBGx8VmeDixfYey0zfGsjWXQyOYi5Qdh8r4YxuVRQe0rcREDmIGlgGlkixZkfvGuyIf3H4RHikAqpvZha7gWP+HsM1nHOd+En3CWHMK9FWrFUSbEGzTm63JQr3iQnk8W3+5n0cRI7fpToO+X7RSdd98coeQrw8bAHc//XijdDAAFqcfMUQJqJgzOOkty56LT50q5jhsfNRBZmihEGF2jCwmCC4mTVWCMLIK/Lx9nbZfGOM5w87zvaepIvKp/u9qv/J0Ean+V/L6bfefYAvE4jzpEwYkG197LXrVkBdyQXwdJz1WqJErZUM84laHFleRGYOyN1pU7LFXkP3gYksRnvVW6y0i3QNUDMPU2TXULa8NsB1jAxPcd77A+bYuVB4PYN40zEkmbOoAoEASWX8hHGyFrzpzn+aWp1jEj/5HtV18aAMS40Kf72/bUKjq5gFT+Q3KAA6tG1jZZ8G36AqXSSnpVtvjwf8n1KK
int64_t check(Data data, int64_t A) {
  for(int64_t i = 0; i < 8; ++i) {
    data.A = A + i;
    data.B = 0;
    data.C = 0;
    auto output = run_program(data);
    bool matches = true;
    for(size_t j = 0; j < output.size(); ++j) {
      if (output[j] != data.instructions[data.instructions.size()-output.size() + j]) {
        matches = false;
        break;
      }
    }
    if (matches) {
      if (output.size() == data.instructions.size()) {
        return data.A;
      }
      int64_t next = data.A*8;
      if (data.A == 0) {
        next = 8;
      }
      int64_t result = check(data, next);
      if (result != 0) {
        return result;
      }
    }
  }
  return 0;
}

int64_t part2(const Data &data)
{
  return check(data, 0);
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
      data.A = std::stoll(line.substr(line.find(":") + 1));
    }
    else if (line.find("Register B:") != std::string::npos)
    {
      data.B = std::stoll(line.substr(line.find(":") + 1));
    }
    else if (line.find("Register C:") != std::string::npos)
    {
      data.C = std::stoll(line.substr(line.find(":") + 1));
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
    auto s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    int64_t s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  std::string answer1 = "3,6,3,7,0,7,0,3,0";
  int64_t answer2 = 136904920099226;

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
