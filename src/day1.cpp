#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include <aoc2023/trie.hpp>

// finds the first number represented as 0-9 from the left and right, represents them as a two digit number <left><right> and returns that
int part1(std::string line)
{
    // get forward and backward iterators
    auto left = line.begin();
    auto right = line.rbegin();

    // advance them until a number is found
    // take advantage of char's evaluating to strings
    // 0 is ascii 48, 9 is ascii 57, numbers are anything between these two numbers
    while ((*left < 48) || (*left > 57))
    {
        ++left;
    }
    while ((*right < 48) || (*right > 57))
    {
        ++right;
    }
    // subtract 48 from each number to get its value, recourd the sum
    return (int(*left) - 48) * 10 + (int(*right) - 48);
}

// finds the first number represented as 0-9 or zero, one, ..., nine from the left and right, represents them as a two digit number <left><right> and returns that
int part2(std::string line) {
    Trie t;

    return 0;
}

int main()
{
    std::ifstream file(std::filesystem::path("inputs/day1.txt"));
    std::string line;
    int part1_sum{};
    int part2_sum{};
    while (std::getline(file, line))
    {
        part1_sum += part1(line);
    }
    std::cout << "Part 1: " << part1_sum << std::endl;
    std::cout << "Part 2: " << part2_sum << std::endl;
}