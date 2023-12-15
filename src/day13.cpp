#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>


struct Matrix
{
  size_t rows = 0;
  size_t columns = 0;
  std::vector<char> data;

  void transpose()
  {
    std::vector<char> newData(data.size());

    for (size_t i = 0; i < rows; ++i)
    {
      for (size_t j = 0; j < columns; ++j)
      {
        size_t idx = i * columns + j;
        size_t transposedIdx = j * rows + i;
        newData[transposedIdx] = data[idx];
      }
    }

    std::swap(rows, columns);
    data = std::move(newData);
  }

  void flip() {
    // reverse the columns
    for (size_t j = 0; j < columns; ++j)
    {
      for (size_t i = 0, k = rows - 1; i < k; ++i, --k)
      {
        size_t top = i * columns + j;
        size_t bottom = k * columns + j;
        char t = data[top];
        data[top] = data[bottom];
        data[bottom] = t;
      }
    }
  }

  void rotate_left()
  {
    transpose();
    flip();
  }

  void rotate_right()
  {
    flip();
    transpose();
  }

  void print()
  {
    for (size_t i = 0; i < rows; ++i)
    {
      for (size_t j = 0; j < columns; ++j)
      {
        size_t idx = i * columns + j;
        std::cout << data[idx];
      }
      std::cout << std::endl;
    }
  }
};

enum Reflection {
  NONE,
  HORIZONTAL,
  VERTICAL
};

struct Data
{
  std::vector<Matrix> landscapes;
  std::vector<int> original_reflection_values;
  std::vector<Reflection> original_reflection_directions;
};

bool is_reflection(const Matrix &m, size_t left)
{
  size_t j = left, k = m.columns - 1;
  for (; j < k; ++j, --k)
  {
    for (size_t i = 0; i < m.rows; ++i)
    {
      size_t left_idx = i * m.columns + j;
      size_t right_idx = i * m.columns + k;
      if (m.data[left_idx] != m.data[right_idx])
      {
        return false;
      }
    }
  }
  if (j == k) return false;
  return true;
}

bool is_reflection_backward(const Matrix &m, size_t right)
{
  size_t j = right, k = 0;
  for (; j > k; --j, ++k)
  {
    for (size_t i = 0; i < m.rows; ++i)
    {
      size_t left_idx = i * m.columns + j;
      size_t right_idx = i * m.columns + k;
      if (m.data[left_idx] != m.data[right_idx])
      {
        return false;
      }
    }
  }
  if (k == j) return false;
  return true;
}

int get_dividing_line(const Matrix &m, int old_val = -1)
{
  for (size_t left = 0; left < m.columns - 1; ++left)
  {
    if (is_reflection(m, left))
    {
      size_t sz = std::floor((m.columns - (left + 1)) / 2);
      int cur = left + sz + 1;
      if (old_val != -1 && cur == old_val) break;
      return cur;
    }
  }
  for (size_t right = m.columns - 1; right > 0; --right)
  {
    if (is_reflection_backward(m, right))
    {
      size_t sz = std::floor((right + 1) / 2);
      if (old_val != -1 && sz == old_val) break;
      return sz;
    }
  }
  return 0;
}

int part1(Data& data)
{
  int sum = 0;
  for (auto &m : data.landscapes)
  {
    // check for vertical reflections
    int result = get_dividing_line(m);
    int multiplier = 1;
    // check for horizontal reflections
    if (result == 0)
    {
      multiplier = multiplier == 1 ? 100 : 1;
      m.rotate_left();
      result = get_dividing_line(m);
      m.rotate_right();
      data.original_reflection_directions.push_back(Reflection::HORIZONTAL);
    }
    else {
      data.original_reflection_directions.push_back(Reflection::VERTICAL);
    }
    data.original_reflection_values.push_back(multiplier * result);
    sum += multiplier * result;
  }
  return sum;
}

int part2(Data& data)
{
  int sum = 0;
  auto direction_it = data.original_reflection_directions.begin();
  auto val_it = data.original_reflection_values.begin();
  int result = 0;
  for (auto &m : data.landscapes)
  {
    for (size_t i = 0; i < m.columns * m.rows; ++i)
    {
      char c = m.data[i];
      m.data[i] = c == '.' ? '#' : '.';

      // m.print();
      // std::cout << std::endl;

      // try the opposite reflection type first
      if (*direction_it == Reflection::VERTICAL) {
        result = get_dividing_line(m, *val_it);
        if (0 == result) {
          m.rotate_left();
          // m.print();
          result = 100 * get_dividing_line(m);
          m.rotate_right();
        }
      }
      else {
        // try the horizontal rotation first
        m.rotate_left();
        result = 100 * get_dividing_line(m, *val_it / 100);
        m.rotate_right();
        if (0 == result) {
          result = get_dividing_line(m);
        }
      }
      if (result != 0)
      {
        sum += result;
        break;
      }

      m.data[i] = c;
    }
    result == 0 ? throw std::runtime_error("Bad") : nullptr;
    ++direction_it;
    ++val_it;
  }
  return sum;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day13.txt"));
  std::string line;
  Data data;

  Matrix m;
  while (std::getline(file, line))
  {
    if (line.size() == 0)
    {
      data.landscapes.push_back(m);
      m.columns = 0;
      m.rows = 0;
      m.data.clear();
    }
    else
    {
      m.columns = line.size();
      ++m.rows;
      for (auto &c : line)
      {
        m.data.push_back(c);
      }
    }
  }
  data.landscapes.push_back(m);

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

  int answer1 = 29130;
  int answer2 = 0;

  auto first = part1(data);
  std::cout << "Part 1: " << first << std::endl;

  auto second = part2(data);
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}