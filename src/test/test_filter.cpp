#include <fstream>
#include <iosfwd>
#include <ostream>
#include <vector>

#include "numerical_algorithm/butterworth_filter_design.h"
#include "numerical_algorithm/filtfilt.h"

int test_filter()
{
    numerical_algorithm::ButterworthFilterDesign butter(2, 0.002, 0.2);
    numerical_algorithm::FiltFilt filter(butter);

    std::vector<std::vector<double>> input_signal(
        1, std::vector<double>(500, 0.0));
    std::ifstream input_file("acceleration_data/sig.txt");
    for (int i = 0; i != 500; i++)
    {
        input_file >> input_signal[0][i];
    }
    input_file.close();

    std::vector<std::vector<double>> output_signal =
        filter.Filtering(input_signal);

    std::ofstream output_file("acceleration_data/filtered_sig.txt");
    for (int i = 0; i != 500; i++)
    {
        output_file << output_signal[0][i] << std::endl;
    }
    return 0;
}