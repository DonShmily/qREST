#include <string>
#include <vector>

#include "gmp_library/gmp_library.h"
#include "test_function.h"

using namespace std;

void test_gmp_library()
{
    // 读取数据文件
    string file_name = "acceleration_data/accNS.txt";
    // 创建计算对象
    std::vector<std::vector<double>> test_acceleration =
        readMatrixFromFile(file_name);
    auto &acc = test_acceleration[0];
    auto sa_result = ResponseSpectrum(50, 0.05, acc.data(), acc.size());
}