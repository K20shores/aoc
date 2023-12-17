#include <aoc2023/position.hpp>
#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Data {
  std::vector<std::string> floor;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

char get_char(const std::vector<std::string> &data, Pos cur) {
  if (cur.i >= 0 && cur.i < data.size() && cur.j >= 0 && cur.j < data[cur.i].size()) return data[cur.i][cur.j];
  return '\0';
}

void advance(Direction d, Pos &p) {
  switch (d) {
    case Direction::UP:
      p.i -= 1;
      break;
    case Direction::DOWN:
      p.i += 1;
      break;
    case Direction::RIGHT:
      p.j += 1;
      break;
    case Direction::LEFT:
      p.j -= 1;
      break;
  }
}

bool can_advance(Direction d, const Data &data, Pos p) {
  advance(d, p);
  char c = get_char(data.floor, p);
  return c != '\0';
}

void reflect(const Data &data, Direction &original, Direction next, Pos &beam, std::queue<std::pair<Pos, Direction>> &rays) {
  if (can_advance(next, data, beam)) {
    advance(next, beam);
    original = next;
  } else {
    rays.pop();
  }
}

int compute_energy(const Data &data, Pos start, Direction direction) {
  std::vector<std::vector<std::pair<bool, bool>>> activated(data.floor.size(), std::vector<std::pair<bool, bool>>(data.floor[0].size(), {false, false}));

  std::queue<std::pair<Pos, Direction>> rays;
  rays.push({start, direction});

  while (rays.size() > 0) {
    auto &s = rays.front();
    auto &beam = s.first;
    auto &direction = s.second;

    char c = data.floor[beam.i][beam.j];
    switch (c) {
      case '.': {
        activated[beam.i][beam.j].first = true;
        if (can_advance(direction, data, beam)) {
          advance(direction, beam);
        } else {
          rays.pop();
        }
        break;
      }
      case '|': {
        if (!activated[beam.i][beam.j].second && (direction == Direction::LEFT || direction == Direction::RIGHT)) {
          activated[beam.i][beam.j].first = true;
          if (can_advance(Direction::UP, data, beam)) {
            activated[beam.i][beam.j].second = true;
            rays.push({{beam.i - 1, beam.j}, Direction::UP});
          }
          if (can_advance(Direction::DOWN, data, beam)) {
            activated[beam.i][beam.j].second = true;
            rays.push({{beam.i + 1, beam.j}, Direction::DOWN});
          }
          rays.pop();
        } else if (direction == Direction::DOWN || direction == Direction::UP) {
          activated[beam.i][beam.j].first = true;
          if (can_advance(direction, data, beam)) {
            advance(direction, beam);
          } else {
            rays.pop();
          }
        } else {
          rays.pop();
        }
        break;
      }
      case '-': {
        if (!activated[beam.i][beam.j].second && (direction == Direction::UP || direction == Direction::DOWN)) {
          activated[beam.i][beam.j].first = true;
          if (can_advance(Direction::LEFT, data, beam)) {
            activated[beam.i][beam.j].second = true;
            rays.push({{beam.i, beam.j - 1}, Direction::LEFT});
          }
          if (can_advance(Direction::RIGHT, data, beam)) {
            activated[beam.i][beam.j].second = true;
            rays.push({{beam.i, beam.j + 1}, Direction::RIGHT});
          }
          rays.pop();
        } else if (direction == Direction::RIGHT || direction == Direction::LEFT) {
          activated[beam.i][beam.j].first = true;
          if (can_advance(direction, data, beam)) {
            advance(direction, beam);
          } else {
            rays.pop();
          }
        } else {
          rays.pop();
        }
        break;
      }
      case '/': {
        activated[beam.i][beam.j].first = true;
        switch (direction) {
          case Direction::UP: {
            reflect(data, direction, Direction::RIGHT, beam, rays);
            break;
          }
          case Direction::DOWN: {
            reflect(data, direction, Direction::LEFT, beam, rays);
            break;
          }
          case Direction::LEFT: {
            reflect(data, direction, Direction::DOWN, beam, rays);
            break;
          }
          case Direction::RIGHT: {
            reflect(data, direction, Direction::UP, beam, rays);
            break;
          }
        }
        break;
      }
      case '\\': {
        activated[beam.i][beam.j].first = true;
        switch (direction) {
          case Direction::UP: {
            reflect(data, direction, Direction::LEFT, beam, rays);
            break;
          }
          case Direction::DOWN: {
            reflect(data, direction, Direction::RIGHT, beam, rays);
            break;
          }
          case Direction::LEFT: {
            reflect(data, direction, Direction::UP, beam, rays);
            break;
          }
          case Direction::RIGHT: {
            reflect(data, direction, Direction::DOWN, beam, rays);
            break;
          }
        }
        break;
      }
    }
  }

  int sum = 0;
  for (auto &row : activated) {
    for (auto &p : row) {
      if (p.first) {
        ++sum;
        // std::cout << "#";
      } else {
        // std::cout << ".";
      }
    }
    // std::cout << std::endl;
  }
  return sum;
}

int part1(const Data &data) { return compute_energy(data, {0, 0}, Direction::RIGHT); }

int part2(const Data &data) {
  int max = 0;
  for (size_t i = 0; i < data.floor.size(); ++i) {
    max = std::max(compute_energy(data, {0, (long)i}, Direction::DOWN), max);
    max = std::max(compute_energy(data, {0, (long)i}, Direction::UP), max);
  }
  for (size_t j = 0; j < data.floor[0].size(); ++j) {
    max = std::max(compute_energy(data, {(long)j, 0}, Direction::DOWN), max);
    max = std::max(compute_energy(data, {(long)j, 0}, Direction::UP), max);
  }
  return max;
}

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day16.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line)) {
    data.floor.push_back(line);
  }

  return data;
}

class BenchmarkFixture : public benchmark::Fixture {
public:
  static Data data;
};

Data BenchmarkFixture::data = parse();

BENCHMARK_DEFINE_F(BenchmarkFixture, Part1Benchmark)
(benchmark::State &state) {
  for (auto _ : state) {
    int s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state) {
  for (auto _ : state) {
    int s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark);

int main(int argc, char **argv) {
  Data data = parse();

  int answer1 = 7242;
  int answer2 = 7572;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}