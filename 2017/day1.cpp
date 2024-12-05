#include <iostream>
#include <fstream>
#include <string>

void part1()
{
    std::ifstream ifs("day1_input.txt");
    char first;
    char cur;
    char prev;
    int sum = 0;

    ifs.read(&first, 1);
    ifs.read(&cur, 1);
    prev = first;

    do
    {
        int c = cur - '0';
        int p = prev - '0';
        if (c == p)
        {
            sum += c;
        }
        if (ifs.peek() == EOF)
        {
            break;
        }
        prev = cur;
        ifs.read(&cur, 1);
    }
    while(!ifs.eof());

    if (first == prev)
    {
        sum += (prev - '0');
    }
    std::cout << "Part 1" << std::endl;
    std::cout << "Sum: " << sum << std::endl;
}

void part2()
{
    std::ifstream ifs("day1_input.txt");
    std::string input;

    std::getline(ifs, input);

    int offset = input.size()/2;
    int sum = 0;
    std::cout << input.size() << std::endl;
    for (unsigned int i = 0; i < input.size(); ++i)
    {
        if (input[i] == input[(i + offset) % input.size()])
        {
            sum += input[i] - '0';
        }
    }

    std::cout << "Part 2" << std::endl;
    std::cout << "Sum: " << sum << std::endl;
}

int main()
{
    part1();
    part2();
}
