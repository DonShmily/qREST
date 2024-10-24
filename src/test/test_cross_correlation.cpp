#include "test_function.h"

#include <iostream>

#include "numerical_algorithm/vector_calculation.h"

using namespace std;


void test_cross_correlation()
{
    std::cout << "Test cross correlation." << std::endl;

    // Test cross correlation
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {5, 4, 3, 2, 1};
    auto z = numerical_algorithm::CrossCorrelation(x, y);
    for (auto i : z)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}