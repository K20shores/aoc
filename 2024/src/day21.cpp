#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/pos.h>

struct Data
{
  std::vector<std::string> codes;
};

std::vector<std::string> numeric_keypad = {
  "789",
  "456",
  "123",
  " 0A"
};
Pos _9 = {.x = 2, .y = 0};
Pos _8 = {.x = 1, .y = 0};
Pos _7 = {.x = 0, .y = 0};
Pos _6 = {.x = 2, .y = 1};
Pos _5 = {.x = 1, .y = 1};
Pos _4 = {.x = 0, .y = 1};
Pos _3 = {.x = 2, .y = 2};
Pos _2 = {.x = 1, .y = 2};
Pos _1 = {.x = 0, .y = 2};
Pos _0 = {.x = 1, .y = 3};
Pos _numA = {.x = 2, .y = 3};
std::map<char, Pos> numeric_keyapd_char_to_pos = {
  {'7', _7}, {'8', _8}, {'9', _9},
  {'4', _4}, {'5', _5}, {'6', _6},
  {'1', _1}, {'2', _2}, {'3', _3},
  {'0', _0}, {'A', _numA}
};
std::map<std::pair<Pos, Pos>, std::string> numeric_shortest_paths = {
  {{_7, _9}, ">>"}, {{_7, _8}, ">"}, {{_7, _6}, "v>>"},
  {{_7, _5}, "v>"}, {{_7, _4}, "v"}, {{_7, _3}, "vv>>"},
  {{_7, _2}, "vv>"}, {{_7, _1}, "vv"}, {{_7, _0}, ">vvv"},
  {{_7, _numA}, ">>vvv"},

  {{_8, _9}, ">"}, {{_8, _7}, "<"}, {{_8, _6}, "v>"},
  {{_8, _5}, "v"}, {{_8, _4}, "<v"}, {{_8, _3}, "vv>"},
  {{_8, _2}, "vv"}, {{_8, _1}, "<vv"}, {{_8, _0}, "vvv"},
  {{_8, _numA}, "vvv>"},

  {{_9, _8}, "<"}, {{_9, _7}, "<<"}, {{_9, _6}, "v"},
  {{_9, _5}, "v<"}, {{_9, _4}, "<<v"}, {{_9, _3}, "vv"},
  {{_9, _2}, "<vv"}, {{_9, _1}, "<<vv"}, {{_9, _0}, "<vvv"},
  {{_9, _numA}, "vvv"},

  {{_4, _9}, "^>>"}, {{_4, _8}, "^>"}, {{_4, _7}, "^"},
  {{_4, _6}, ">>"}, {{_4, _5}, ">"}, {{_4, _3}, "v>>"},
  {{_4, _2}, "v>"}, {{_4, _1}, "v"}, {{_4, _0}, ">vv"},
  {{_4, _numA}, ">>vv"},

  {{_5, _9}, "^>"}, {{_5, _8}, "^"}, {{_5, _7}, "<^"},
  {{_5, _6}, ">"}, {{_5, _4}, "<"}, {{_5, _3}, "v>"},
  {{_5, _2}, "v"}, {{_5, _1}, "<v"}, {{_5, _0}, "vv"},
  {{_5, _numA}, "vv>"},

  {{_6, _9}, "^"}, {{_6, _8}, "^<"}, {{_6, _7}, "<<^"},
  {{_6, _5}, "<"}, {{_6, _4}, "<<"}, {{_6, _3}, "v"},
  {{_6, _2}, "<v"}, {{_6, _1}, "<<v"}, {{_6, _0}, "<vv"},
  {{_6, _numA}, "vv"},

  {{_1, _9}, "^^>>"}, {{_1, _8}, "^^>"}, {{_1, _7}, "^^"},
  {{_1, _6}, "^>>"}, {{_1, _5}, "^>"}, {{_1, _4}, "^"},
  {{_1, _3}, ">>"}, {{_1, _2}, ">"}, {{_1, _0}, ">v"},
  {{_1, _numA}, ">>v"},

  {{_2, _9}, "^^>"}, {{_2, _8}, "^^"}, {{_2, _7}, "<^^"},
  {{_2, _6}, "^>"}, {{_2, _5}, "^"}, {{_2, _4}, "<^"},
  {{_2, _3}, ">"}, {{_2, _1}, "<"}, {{_2, _0}, "v"},
  {{_2, _numA}, "v>"},

  {{_3, _9}, "^^"}, {{_3, _8}, "<^^"}, {{_3, _7}, "<<^^"},
  {{_3, _6}, "^"}, {{_3, _5}, "<^"}, {{_3, _4}, "<<^"},
  {{_3, _2}, "<"}, {{_3, _1}, "<<"}, {{_3, _0}, "<v"},
  {{_3, _numA}, "v"},

  {{_0, _9}, "^^^>"}, {{_0, _8}, "^^^"}, {{_0, _7}, "^^^<"},
  {{_0, _6}, "^^>"}, {{_0, _5}, "^^"}, {{_0, _4}, "^^<"},
  {{_0, _3}, "^>"}, {{_0, _2}, "^"}, {{_0, _1}, "^<"},
  {{_0, _numA}, ">"},

  {{_numA, _9}, "^^^"}, {{_numA, _8}, "<^^^"}, {{_numA, _7}, "^^^<<"},
  {{_numA, _6}, "^^"}, {{_numA, _5}, "<^^"}, {{_numA, _4}, "^^<<"},
  {{_numA, _3}, "^"}, {{_numA, _2}, "<^"}, {{_numA, _1}, "^<<"},
  {{_numA, _0}, "<"}
};

Pos _up = {.x = 1, .y = 0};
Pos _dirA = {.x = 2, .y = 0};
Pos _left = {.x = 0, .y = 1};
Pos _down = {.x = 1, .y = 1};
Pos _right = {.x = 2, .y = 1};
std::vector<std::string> directional_keypad = {
  " ^A",
  "<v>"
};
std::map<char, Pos> directional_keyapd_char_to_pos = {
  {'^', _up}, {'A', _dirA},
  {'<', _left}, {'v', _down}, {'>', _right}
};
std::map<std::pair<Pos, Pos>, std::string> directional_shortest_paths = {
  {{_up, _dirA}, ">"}, {{_up, _right}, "v>"}, {{_up, _down}, "v"},
  {{_up, _left}, "v<"},

  {{_dirA, _up}, "<"}, {{_dirA, _right}, "v"}, {{_dirA, _down}, "<v"},
  {{_dirA, _left}, "v<<"},

  {{_left, _dirA}, ">>^"}, {{_left, _up}, ">^"}, {{_left, _right}, ">>"},
  {{_left, _down}, ">"},

  {{_down, _dirA}, "^>"}, {{_down, _right}, ">"}, {{_down, _up}, "^"},
  {{_down, _left}, "<"},

  {{_right, _dirA}, "^"}, {{_right, _up}, "<^"}, {{_right, _down}, "<"},
  {{_right, _left}, "<<"}
};

std::string get_path(std::string path, std::map<char, Pos> char_to_pos, std::map<std::pair<Pos, Pos>, std::string> shortest_path, std::vector<std::string> keypad) {
  std::string output_path = "";
  Pos current = char_to_pos['A'];
  for (char c : path) {
    Pos next = char_to_pos[c];
    output_path += shortest_path[{current, next}];
    output_path.push_back('A');
    // std::cout << std::format("-- moving from ({}) to ({}) :: {}", keypad[current.y][current.x], keypad[next.y][next.x], output_path) << std::endl;
    current = next;
  }
  return output_path;
}

int part1(const Data &data)
{
  int n_robots = 2;
  int sum = 0;
  for(const auto& code : data.codes) {
    int val = std::stoi(code.substr(0, code.size() - 1));
    std::cout << std::format("{} ({})\n", code, val);
    std::cout << code << std::endl;
    auto numeric_path = get_path(code, numeric_keyapd_char_to_pos, numeric_shortest_paths, numeric_keypad);
    for(auto c : numeric_path) {
      std::cout << c;
    }
    std::cout << " " << numeric_path.size() << std::endl;
    std::string directional_path = numeric_path;
    for(int i = 0; i < n_robots; ++i) {
      directional_path = get_path(directional_path, directional_keyapd_char_to_pos, directional_shortest_paths, directional_keypad);
      for(auto c : directional_path) {
        std::cout << c;
      }
      std::cout << " " << directional_path.size() << std::endl;
    }
    sum += val * directional_path.size();
    std::cout << std::endl;
  }

  return sum;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day21.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.codes.push_back(line);
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
