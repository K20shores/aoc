#pragma once

#include <vector>

std::vector<int> get_primes(int n);
std::vector<int> prime_factorize(int a, const std::vector<int> &primes);
long lcm(std::vector<int> vals, const std::vector<int> &primes);
long n_choose_k(long n, long k);
long n_permute_k(long n, long k);