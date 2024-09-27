#include <iostream>
#include <vector>

#include "data_structure/basic_data_structure.h"
#include "test_function.h"

using namespace std;

void test_size()
{
    cout << "size of size_t: " << sizeof(size_t) << endl;
    cout << "size of double: " << sizeof(double) << endl;
    cout << "size of shared_ptr: "
         << sizeof(std::shared_ptr<vector<vector<double>>>) << endl;
    cout << "size of vector<double>: " << sizeof(vector<double>) << endl;
    cout << "size of vector<vector<double>>: " << sizeof(vector<vector<double>>)
         << endl;
    cout << "---------------------------------" << endl;
    cout << "size of GmpCalculation: "
         << sizeof(gmp_calculation::GmpCalculation) << endl;
    cout << "size of BasicEdpCalculation: "
         << sizeof(edp_calculation::BasicEdpCalculation) << endl;
    cout << "size of FilteringIntegralMethod: "
         << sizeof(edp_calculation::FilteringIntegralMethod) << endl;
    cout << "size of InterStoryDriftResult: "
         << sizeof(edp_calculation::EdpResult) << endl;
    cout << "size of FilteringIntegral: "
         << sizeof(edp_calculation::FilteringIntegral) << endl;
    cout << "size of ModifiedFilteringIntegral: "
         << sizeof(edp_calculation::ModifiedFilteringIntegral) << endl;
    cout << "---------------------------------" << endl;
    cout << "size of basic_data: " << sizeof(data_structure::BasicData) << endl;
    cout << "size of Building: " << sizeof(data_structure::Building) << endl;
    cout << "size of Acceleration: " << sizeof(data_structure::Acceleration)
         << endl;
}