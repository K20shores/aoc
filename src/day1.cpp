#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

int main() {
    std::ifstream file(std::filesystem::path("inputs/day1.txt"));
    std::string str; 
    int sum = 0;
    while (std::getline(file, str))
    {
        // get forward and backward iterators
        auto left = str.begin();
        auto right = str.rbegin();

        // advance them until a number is found
        // take advantage of char's evaluating to strings
        // 0 is ascii 48, 9 is ascii 57, numbers are anything between these two numbers
        while ((*left < 48) || (*left > 57)) {
            ++left;
        }
        while ((*right < 48) || (*right > 57)) {
            ++right;
        }
        // subtract 48 from each number to get its value, recourd the sum
        sum += (int(*left) - 48)*10 + (int(*right)-48);
    }
    std::cout << sum << std::endl;
}