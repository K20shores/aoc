#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <omp.h>
#include <vector>

std::vector<std::vector<int>> read_matrix()
{
    std::ifstream ifs("day2_input.txt");
    std::vector<std::vector<int>> m;
    std::string line;
    while (std::getline(ifs, line))
    {
        std::vector<int> row;
        std::stringstream ss(line);
        int entry;
        while (ss >> entry)
        {
            row.push_back(entry);
        }
        m.push_back(row);
    }

    return m;
}

void part1()
{
    std::cout << "Part 1:" << std::endl;
    std::vector<std::vector<int>> m = read_matrix();
    int checksum = 0;

    #pragma omp parallel for
    for (unsigned int i = 0; i < m.size(); ++i)
    {
        std::vector<int> row = m[i];
        int min = row[0];
        int max = row[0];
        for (int i = 1; i < row.size(); ++i)
        {
            min = (row[i] < min ) ? row[i] : min;
            max = (row[i] > max ) ? row[i] : max;
        }
        #pragma omp critical
        {
            checksum += max - min;
        }
    }
    std::cout << "Checksum: " << checksum << std::endl;
}

void part2()
{
    std::cout << "Part 2:" << std::endl;
    std::vector<std::vector<int>> m = read_matrix();
    int checksum = 0;

    #pragma omp parallel for
    for (unsigned int i = 0; i < m.size(); ++i)
    {
        std::vector<int> row = m[i];
        bool found = false;
        int idx = 0;
        do
        {
            for (int i = 0; i < row.size(); ++i)
            {
                if ((i != idx) && (row[i] % row[idx] == 0))
                {
                    found = true;
                    #pragma omp critical
                    {
                        checksum += row[i] / row[idx];
                    }
                }
            }
            ++idx;
        } while (!found);
    }
    std::cout << "Checksum: " << checksum << std::endl;
}

int main()
{
    part1();
    part2();
}
