#include "test_function.h"

#include <iostream>
#include <vector>

#include "numerical_algorithm/butterworth_filter_design.h"

using namespace std;

int test_butter()
{
    auto butter = numerical_algorithm::ButterworthFilterDesign(2, 0.1, 0.2);
    vector<double> a, b;
    butter.get_filter_coefficients(a, b);
    for (auto i : a)
    {
        cout << i << " ";
    }
    cout << endl;
    for (auto i : b)
    {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}