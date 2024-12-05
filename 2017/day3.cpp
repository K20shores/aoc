#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

void part1()
{
    std::cout << "Part 1:" << std::endl;
    int input = 325489;
    int n_steps = 0;
    int nearest_square = input;
    int side_length = 0;
    int next_lowest_square;
    int starting_side = 0;

    if (input == 1)
    {
        n_steps = 0;
    }
    else
    {
        int root{(int)round(sqrt(nearest_square))};
        while (root * root != nearest_square || root % 2 == 0)
        {
            ++nearest_square;
            root = round(sqrt(nearest_square));
        }
        side_length = (int)sqrt(nearest_square);
        next_lowest_square = (int)(sqrt(nearest_square) - 2);
        starting_side = next_lowest_square * next_lowest_square + 1;
        int mid_point = next_lowest_square * next_lowest_square + (side_length / 2);

        while (input > starting_side + (side_length - 2))
        {
            starting_side += (side_length - 1);
            mid_point += (side_length - 1);
        }
        int dist_to_midpoint = abs(input - mid_point);
        n_steps = dist_to_midpoint + side_length / 2;
    }
    std::cout << "Input: " << input << " // Number of steps: " << n_steps << std::endl;
}

void part2()
{
    std::cout << "Part 2:" << std::endl;
    //int input = 325489;
    int input = 24;
    int nearest_square = input;
    int root{(int)round(sqrt(nearest_square))};
    while (root * root != nearest_square || root % 2 == 0)
    {
        ++nearest_square;
        root = round(sqrt(nearest_square));
    }

    std::vector<std::vector<int>> matrix;
    matrix.reserve(nearest_square);

    for(int i = 0; i < nearest_square; ++i)
    {
        matrix[i].reserve(nearest_square);
    }

    int side_length = (int)sqrt(nearest_square);
    int i = side_length / 2;
    int j = i;
    matrix[i][j] = 1;
    ++j;
    for (int k = 1; k < nearest_square + 1; ++k)
    {
    }
}

int main()
{
    part1();
    part2();
}
