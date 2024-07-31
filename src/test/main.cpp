#include <iostream>
#include "edp_calculation/basic_edp_calculation.h"
#include "edp_calculation/filtering_integral.h"
#include "gmp_calculation/gmp_calculation.h"
#include "test_function.h"


using namespace std;

int main()
{
    // 测试butter函数
    // test_butter();

    // 测试滤波器
    // test_filter();

    // 测试改进的滤波积分算法
    // test_modified_filter_integrate();

    // 测试gmp
    // test_gmp();

    // 测试类型大小
    test_size();

    return 0;
}