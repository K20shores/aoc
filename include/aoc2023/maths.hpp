#pragma once

#include <map>
#include <algorithm>
#include <numeric>
#include <cmath>

std::vector<int> get_primes(int n)
{
  std::vector<bool> mask(n, true);
  std::vector<int> primes;

  for (size_t i = 2; i < std::sqrt(n); ++i)
  {
    if (mask[i - 2])
    {
      for (size_t j = i*i; j + i < mask.size(); j += i)
      {
        mask[j - 2] = false;
      }
    }
  }
  for(size_t i = 0; i < mask.size(); ++i) {
    if (mask[i]) {
      primes.push_back(i + 2);
    }
  }
  return primes;
}

std::vector<int> prime_factorize(int a, const std::vector<int> &primes)
{
  std::vector<int> factorization;
  for (auto &p : primes)
  {
    int count = 0;
    while (a % p == 0)
    {
      ++count;
      a /= p;
    }
    factorization.push_back(count);
  }
  return factorization;
}

long lcm(std::vector<int> vals, const std::vector<int> &primes)
{
  std::vector<std::vector<int>> factorizations;
  for (auto val : vals)
  {
    factorizations.push_back(prime_factorize(val, primes));
  }
  long _lcm = 1;

  size_t max = 0;
  for (auto &factorization : factorizations)
  {
    auto last_nonzero = std::find_if(factorization.rbegin(), factorization.rend(), [](int x)
                                     { return x != 0; });
    size_t _max = factorization.rend() - last_nonzero - 1;
    max = _max > max ? _max : max;
  }

  for (size_t i = 0; i < max + 1; ++i)
  {
    int max_pow = 0;
    for (size_t j = 0; j < factorizations.size(); ++j)
    {
      int pow = factorizations[j][i];
      max_pow = pow > max_pow ? pow : max_pow;
    }
    _lcm *= std::pow(primes[i], max_pow);
  }
  return _lcm;
}