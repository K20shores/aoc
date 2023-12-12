#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <queue>

struct Pos
{
  size_t i;
  size_t j;

  bool operator==(const Pos &other) const
  {
    return (i == other.i) && (j == other.j);
  }

  bool operator!=(const Pos &other) const
  {
    return !(*this == other);
  }
};

struct Data
{
  std::vector<std::string> pipes;
  Pos S;
};

char get_char(const std::vector<std::string> &data, Pos cur)
{
  if (cur.i >= 0 && cur.i < data.size() && cur.j >= 0 && cur.j < data[cur.i].size())
    return data[cur.i][cur.j];
  return '\0';
}

std::vector<Pos> connected_pipes(const Pos &cur, const Data &data)
{
  size_t i = cur.i, j = cur.j;

  Pos up = {.i = i - 1, .j = j};
  Pos down = {.i = i + 1, .j = j};
  Pos left = {.i = i, .j = j - 1};
  Pos right = {.i = i, .j = j + 1};

  char curc = data.pipes[cur.i][cur.j];
  char upc = get_char(data.pipes, up);
  char downc = get_char(data.pipes, down);
  char leftc = get_char(data.pipes, left);
  char rightc = get_char(data.pipes, right);

  std::vector<Pos> connected;

  switch (curc)
  {
  case 'S':
    if (upc == '7' || upc == 'F' || upc == '|')
      connected.push_back(up);
    if (downc == 'L' || downc == 'J' || downc == '|')
      connected.push_back(down);
    if (leftc == 'L' || leftc == 'F' || leftc == '-')
      connected.push_back(left);
    if (rightc == '7' || rightc == 'J' || rightc == '-')
      connected.push_back(right);
    break;
  case '|': // is a vertical pipe connecting north and south.
    if (upc == '7' || upc == 'F' || upc == '|' || upc == 'S')
      connected.push_back(up);
    if (downc == 'L' || downc == 'J' || downc == '|' || downc == 'S')
      connected.push_back(down);
    break;
  case '-': // is a horizontal pipe connecting east and west.
    if (leftc == 'L' || leftc == 'F' || leftc == '-' || leftc == 'S')
      connected.push_back(left);
    if (rightc == '7' || rightc == 'J' || rightc == '-' || rightc == 'S')
      connected.push_back(right);
    break;
  case 'L': // is a 90-degree bend connecting north and east.
    if (upc == '|' || upc == '7' || upc == 'F' || upc == 'S')
      connected.push_back(up);
    if (rightc == '7' || rightc == 'J' || rightc == '-' || rightc == 'S')
      connected.push_back(right);
    break;
  case 'J': // is a 90-degree bend connecting north and west.
    if (upc == '|' || upc == '7' || upc == 'F' || upc == 'S')
      connected.push_back(up);
    if (leftc == 'F' || leftc == '-' || leftc == 'L' || leftc == 'S')
      connected.push_back(left);
    break;
  case '7': // is a 90-degree bend connecting south and west.
    if (downc == '|' || downc == 'L' || downc == 'J' || downc == 'S')
      connected.push_back(down);
    if (leftc == 'F' || leftc == '-' || leftc == 'L' || leftc == 'S')
      connected.push_back(left);
    break;
  case 'F': // is a 90-degree bend connecting south and east.
    if (downc == '|' || downc == 'L' || downc == 'J' || downc == 'S')
      connected.push_back(down);
    if (rightc == '7' || rightc == '-' || rightc == 'J' || rightc == 'S')
      connected.push_back(right);
    break;
  case '.': // is ground; there is no pipe in this tile.
    throw std::runtime_error("You shouldn't be here!");
    break;
  }

  if (connected.size() != 2)
  {
    throw std::runtime_error("Too many pipes");
  }
  // connected.size() != 2 ? throw std::runtime_error("Too many pipes") : nullptr;
  return connected;
}

void advance(Pos &cur, Pos &prev, const Data &data)
{
  auto connected = connected_pipes(cur, data);
  if (connected[0] == prev)
  {
    prev = cur;
    cur = connected[1];
  }
  else
  {
    prev = cur;
    cur = connected[0];
  }
}

int part1(const Data &data)
{
  auto connected = connected_pipes(data.S, data);
  Pos last_iter1 = data.S;
  Pos last_iter2 = data.S;
  Pos iter1 = connected[0];
  Pos iter2 = connected[1];

  int steps = 1;
  while (iter1 != iter2)
  {
    advance(iter1, last_iter1, data);
    advance(iter2, last_iter2, data);
    ++steps;
  }
  return steps;
}

int floodfill(const Pos &cur, Data &data, const std::vector<Pos> &loop, char fill_char = '0', char loop_char = '*')
{
  std::queue<Pos> working_set;
  working_set.push(cur);
  int filled = 0;
  while (!working_set.empty())
  {
    Pos p = working_set.front();
    working_set.pop();
    char &c = data.pipes[p.i][p.j];
    if (c != fill_char && c != loop_char)
    {
      ++filled;
      c = fill_char;
      // not
      size_t i = p.i, j = p.j;
      Pos up = {.i = i - 1, .j = j};
      Pos down = {.i = i + 1, .j = j};
      Pos left = {.i = i, .j = j - 1};
      Pos right = {.i = i, .j = j + 1};

      char upc = get_char(data.pipes, up);
      char downc = get_char(data.pipes, down);
      char leftc = get_char(data.pipes, left);
      char rightc = get_char(data.pipes, right);

      if (upc != '*' && upc != '0' && upc != '\0')
        working_set.push(up);
      if (downc != '*' && downc != '0' && downc != '\0')
        working_set.push(down);
      if (leftc != '*' && leftc != '0' && leftc != '\0')
        working_set.push(left);
      if (rightc != '*' && rightc != '0' && rightc != '\0')
        working_set.push(right);
    }
  }
  return filled;
}

int part2(Data data)
{
  // get the path
  auto connected = connected_pipes(data.S, data);
  std::vector<Pos> loop;
  loop.reserve(data.pipes.size() * data.pipes[0].size());
  loop.push_back(data.S);
  Pos last_iter1 = data.S;
  Pos last_iter2 = data.S;
  Pos iter1 = connected[0];
  Pos iter2 = connected[1];
  loop.push_back(iter1);
  loop.push_back(iter2);

  while (true)
  {
    advance(iter1, last_iter1, data);
    advance(iter2, last_iter2, data);
    if (iter1 != iter2) {
      loop.push_back(iter1);
      loop.push_back(iter2);
    }
    else {
      loop.push_back(iter1);
      break;
    }
  }

  // replace all loop cells with '*'
  for(const auto& p : loop) data.pipes[p.i][p.j] = '*';

  int internal = 0;
  // now, traverse the loop counterclockwise 
  // maintain a vector that points orthogonally inward to the loop
  // any cell that this orthogonal vector points towards that is
  // not on the loop, is contained within the loop 
  // count the area that is filled
  for (size_t i = 0; i < data.pipes.size(); ++i)
  {
    for (size_t j = 0; j < data.pipes[0].size(); ++j)
    {
      floodfill({i, j}, data, loop);
    }
  }
  for (const auto &row : data.pipes)
  {
    std::cout << row << std::endl;
  }
  return internal;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day10.txt"));
  std::string line;
  Data data;

  size_t i = 0;
  while (std::getline(file, line))
  {
    data.pipes.push_back(line);
    auto it = line.find('S');
    if (it != std::string::npos)
    {
      data.S = {i, it};
    }
    ++i;
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

  int answer1 = 6697;
  int answer2 = 1;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}