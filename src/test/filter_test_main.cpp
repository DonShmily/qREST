#include "test_function.h"

#include <fstream>
#include "numerical_algorithm/butterworth_filter_design.h"
#include "numerical_algorithm/filter.h"

#include "eigen3/Eigen/Dense"

int filter_test_main()
{
    numerical_algorithm::ButterworthFilterDesign butter(2, 0.002, 0.2);
    numerical_algorithm::Filter filter(butter);

    Eigen::MatrixXd input_signal(500, 1);
    std::ifstream input_file("sig.txt");
    for (int i = 0; i != 500; i++)
    {
        input_file >> input_signal(i, 0);
    }
    input_file.close();

    Eigen::MatrixXd output_signal(500, 1);
    filter.Filtering(input_signal, output_signal);

    std::ofstream output_file("filtered_sig.txt");
    for (int i = 0; i != 500; i++)
    {
        output_file << output_signal(i, 0) << std::endl;
    }
    return 0;
}