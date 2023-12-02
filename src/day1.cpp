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
int part2(std::string line, const Trie &t)
{
    int left = -1;
    int right = -1;
    Trie::Node *search = nullptr;

    for (size_t i = 0; i < line.size(); ++i)
    {
        char c = line[i];
        if ((c >= 48) && (c <= 57))
        {
            int candidate = int(c) - 48;
            if (left == -1) {
                left = candidate * 10;
            }
            else {
                right = candidate;
            }
        }
        else
        {
            search = t.search(c);
            if (search)
            {
                for (size_t j = i + 1; j < line.size() && search; ++j)
                {
                    char d = line[j];
                    if ((d < 97) || (d > 122))
                    {
                        break;
                    }
                    search = search->children[d -97];
                    if (search && search->value != -1)
                    {
                        int candidate = search->value;
                        if (left == -1) {
                            left = candidate * 10;
                        }
                        else {
                            right = candidate;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (right == -1) {
        right = left / 10;
    }

    return left + right;
}

int main()
{
    std::ifstream file(std::filesystem::path("inputs/day1.txt"));
    std::string line;
    int part1_sum{};
    int part2_sum{};

    Trie t;
    t.insert("zero", 0);
    t.insert("one", 1);
    t.insert("two", 2);
    t.insert("three", 3);
    t.insert("four", 4);
    t.insert("five", 5);
    t.insert("six", 6);
    t.insert("seven", 7);
    t.insert("eight", 8);
    t.insert("nine", 9);
    t.insert("nine", 9);

    while (std::getline(file, line))
    {
        part1_sum += part1(line);
        part2_sum += part2(line, t);
    }
    std::cout << "Part 1: " << part1_sum << std::endl;
    std::cout << "Part 2: " << part2_sum << std::endl;
}