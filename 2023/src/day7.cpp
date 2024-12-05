#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <unordered_map>

enum Rank { None, FiveOfAKind, FourOfAKind, FullHouse, ThreeOfAKind, TwoPair, OnePair, HighCard };

std::string rank_to_string(Rank rank) {
  switch (rank) {
    case None:
      return "None";
    case FiveOfAKind:
      return "FiveOfAKind";
    case FourOfAKind:
      return "FourOfAKind";
    case FullHouse:
      return "FullHouse";
    case ThreeOfAKind:
      return "ThreeOfAKind";
    case TwoPair:
      return "TwoPair";
    case OnePair:
      return "OnePair";
    case HighCard:
      return "HighCard";
    default:
      return "Unknown Rank";
  }
}

struct Data {
  std::vector<std::string> hands;
  std::vector<long> bets;
};

Rank score_part1(std::unordered_map<char, int> &counts) {
  Rank rank = Rank::None;
  for (const auto &it : counts) {
    if (rank == Rank::None) {
      if (it.second == 5) {
        rank = Rank::FiveOfAKind;
      } else if (it.second == 4) {
        rank = Rank::FourOfAKind;
      } else if (it.second == 3) {
        rank = Rank::ThreeOfAKind;
      } else if (it.second == 2) {
        rank = Rank::OnePair;
      }
    } else {
      if (it.second == 3) {
        rank = Rank::FullHouse;
      } else if (it.second == 2) {
        if (rank == Rank::OnePair) {
          rank = Rank::TwoPair;
        } else if (rank == Rank::ThreeOfAKind) {
          rank = Rank::FullHouse;
        }
      }
    }
  }
  if (rank == Rank::None) rank = Rank::HighCard;
  return rank;
}

Rank score_part2(std::unordered_map<char, int> &counts) {
  Rank rank = Rank::None;
  int jokers = counts.at('J');
  if (jokers > 0) {
    counts['J'] = 0;
    auto max = std::max_element(counts.begin(), counts.end(), [](const auto &a, const auto &b) { return a.second < b.second; });
    max->second += jokers;
    for (const auto &it : counts) {
      if (rank == Rank::None) {
        if (it.second == 5) {
          rank = Rank::FiveOfAKind;
        } else if (it.second == 4) {
          rank = Rank::FourOfAKind;
        } else if (it.second == 3) {
          rank = Rank::ThreeOfAKind;
        } else if (it.second == 2) {
          rank = Rank::OnePair;
        }
      } else {
        if (it.second == 3) {
          rank = Rank::FullHouse;
        } else if (it.second == 2) {
          if (rank == Rank::OnePair) {
            rank = Rank::TwoPair;
          } else if (rank == Rank::ThreeOfAKind) {
            rank = Rank::FullHouse;
          }
        }
      }
    }
  } else {
    return score_part1(counts);
  }
  if (rank == Rank::None) rank = Rank::HighCard;
  return rank;
}

std::vector<Rank> get_rankings(const std::vector<std::string> &hands, std::function<Rank(std::unordered_map<char, int> &)> score) {
  std::vector<Rank> ranks(hands.size());
  std::unordered_map<char, int> counts;
  counts['2'] = 0;
  counts['3'] = 0;
  counts['4'] = 0;
  counts['5'] = 0;
  counts['6'] = 0;
  counts['7'] = 0;
  counts['8'] = 0;
  counts['9'] = 0;
  counts['T'] = 0;
  counts['J'] = 0;
  counts['Q'] = 0;
  counts['K'] = 0;
  counts['A'] = 0;

  for (size_t hand_idx = 0; hand_idx < hands.size(); ++hand_idx) {
    for (auto &c : counts) c.second = 0;
    for (auto &c : hands[hand_idx]) {
      auto it = counts.find(c);
      if (it != counts.end()) {
        ++it->second;
      } else {
        counts[c] = 1;
      }
    }
    int pairs = 0;
    Rank rank = score(counts);
    ranks[hand_idx] = rank;
  }
  return ranks;
}

std::vector<size_t> argsort(const std::vector<Rank> &values, const std::vector<std::string> &hands, std::map<char, int> &ordering) {
  std::vector<size_t> indices(values.size());
  std::iota(indices.begin(), indices.end(), 0);

  std::sort(indices.begin(), indices.end(), [&values, &hands, &ordering](size_t i1, size_t i2) {
    if (i1 == i2) return false;
    if (values[i1] == values[i2]) {
      size_t i = 0;
      while (hands[i1][i] == hands[i2][i]) {
        ++i;
      }
      auto left = ordering[hands[i1][i]];
      auto right = ordering[hands[i2][i]];
      return left < right;
    }
    return values[i1] > values[i2];
  });

  return indices;
}

long part1(const Data &data) {
  std::map<char, int> ordering = {
      {'A', 13}, {'K', 12}, {'Q', 11}, {'J', 10}, {'T', 9}, {'9', 8}, {'8', 7}, {'7', 6}, {'6', 5}, {'5', 4}, {'4', 3}, {'3', 2}, {'2', 1},
  };
  // std::cout << std::endl;
  long sum = 0;

  auto rankings = get_rankings(data.hands, score_part1);
  auto sorter = argsort(rankings, data.hands, ordering);

  size_t rank = 1;
  for (auto i : sorter) {
    sum += rank * data.bets[i];
    // std::cout << data.hands[i] << " " << data.bets[i] << " " << rank_to_string(rankings[i]) << std::endl;
    ++rank;
  }

  return sum;
}

int part2(const Data &data) {
  std::map<char, int> ordering = {
      {'A', 13}, {'K', 12}, {'Q', 11}, {'J', -1}, {'T', 9}, {'9', 8}, {'8', 7}, {'7', 6}, {'6', 5}, {'5', 4}, {'4', 3}, {'3', 2}, {'2', 1},
  };
  // std::cout << std::endl;
  long sum = 0;

  auto rankings = get_rankings(data.hands, score_part2);
  auto sorter = argsort(rankings, data.hands, ordering);

  size_t rank = 1;
  for (auto i : sorter) {
    sum += rank * data.bets[i];
    // std::cout << data.hands[i] << " " << data.bets[i] << " " << rank_to_string(rankings[i]) << std::endl;
    ++rank;
  }

  return sum;
}

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day7.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line)) {
    auto pos = line.find(' ');
    data.hands.push_back(line.substr(0, pos));
    data.bets.push_back(std::stol(line.substr(pos + 1)));
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

  std::cout << "Part 1: " << part1(data) << std::endl;
  std::cout << "Part 2: " << part2(data) << std::endl;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}