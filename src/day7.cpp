#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <numeric>

enum Rank
{
  None,
  FiveOfAKind,
  FourOfAKind,
  FullHouse,
  ThreeOfAKind,
  TwoPair,
  OnePair,
  HighCard
};

std::string rank_to_string(Rank rank)
{
  switch (rank)
  {
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

struct Data
{
  std::vector<std::string> hands;
  std::vector<long> bets;
};

std::vector<Rank> get_rankings(const std::vector<std::string> &hands)
{
  std::vector<Rank> ranks(hands.size());
  for (size_t hand_idx = 0; hand_idx < hands.size(); ++hand_idx)
  {
    auto hand = hands[hand_idx];
    Rank rank = Rank::None;
    std::sort(hand.begin(), hand.end());
    int pairs = 0;
    for (size_t i = 0; i < hand.size(); ++i)
    {
      char c = hand[i];
      int count = 1;
      while (c == hand[i + 1] && i + 1 < hand.size())
      {
        ++i;
        ++count;
      }
      if (rank == Rank::None)
      {
        if (count == 5)
        {
          rank = Rank::FiveOfAKind;
        }
        else if (count == 4)
        {
          rank = Rank::FourOfAKind;
        }
        else if (count == 3)
        {
          rank = Rank::ThreeOfAKind;
        }
        else if (count == 2)
        {
          rank = Rank::OnePair;
        }
      }
      else
      {
        if (count == 3)
        {
          rank = Rank::FullHouse;
        }
        else if (count == 2)
        {
          if (rank == Rank::OnePair)
          {
            rank = Rank::TwoPair;
          }
          else if (rank == Rank::ThreeOfAKind)
          {
            rank = Rank::FullHouse;
          }
        }
      }
    }
    if (rank == Rank::None)
      rank = Rank::HighCard;
    ranks[hand_idx] = rank;
  }
  return ranks;
}

std::map<char, int> facecard_map = {{'A' , 5}, {'K', 4}, {'Q', 3}, {'J', 2}, {'T', 1}};

std::vector<size_t> argsort(const std::vector<Rank> &values, const std::vector<std::string> &hands)
{
  std::vector<size_t> indices(values.size());
  std::iota(indices.begin(), indices.end(), 0);

  std::sort(indices.begin(), indices.end(),
            [&values, &hands](size_t i1, size_t i2)
            {
              if (i1 == i2)
                return false;
              if (values[i1] == values[i2])
              {
                size_t i = 0;
                while (hands[i1][i] == hands[i2][i])
                {
                  ++i;
                }
                if (std::isdigit(hands[i1][i]) && std::isdigit(hands[i2][i]))
                {
                  // both are digits
                  return hands[i1][i] < hands[i2][i];
                }
                else if (!std::isdigit(hands[i1][i]) && !std::isdigit(hands[i2][i]))
                {
                  // both are letters
                  return facecard_map[hands[i1][i]] < facecard_map[hands[i2][i]];
                }
                else
                {
                  // the one that is the digit must come first
                  return (bool)std::isdigit(hands[i1][i]);
                }
              }
              return values[i1] > values[i2];
            });

  return indices;
}

long part1(const Data &data)
{
  // std::cout << std::endl;
  long sum = 0;

  auto rankings = get_rankings(data.hands);
  auto sorter = argsort(rankings, data.hands);

  size_t rank = 1;
  for (auto i : sorter)
  {
    sum += rank * data.bets[i];
    // std::cout << data.hands[i] << " " << data.bets[i] << " " << rank_to_string(rankings[i]) << std::endl;
    ++rank;
  }

  return sum;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day7.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    auto pos = line.find(' ');
    data.hands.push_back(line.substr(0, pos));
    data.bets.push_back(std::stol(line.substr(pos + 1)));
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

  std::cout << "Part 1: " << part1(data) << std::endl;
  std::cout << "Part 2: " << part2(data) << std::endl;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}