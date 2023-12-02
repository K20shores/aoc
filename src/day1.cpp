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
        auto left = str.begin();
        auto right = str.rbegin();

        while ((*left < 48) || (*left > 59)) {
            ++left;
        }
        while ((*right < 48) || (*right > 59)) {
            ++right;
        }
        sum += (int(*left) - 48)*10 + (int(*right)-48);
    }
    std::cout << sum << std::endl;
}