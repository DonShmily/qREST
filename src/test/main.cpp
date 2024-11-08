﻿#include <iostream>
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

    // 测试安全评价
    // test_safty_tagging();

    // 测试EDP计算模块
    // test_edp_library("acceleration_data/accNS.txt");

    // 测试gmp
    // test_gmp();

    // 测试类型大小
    // test_size();

    // 测试地震动参数库计算
    // test_gmp_library("acceleration_data/accNS.txt");

    // 测试可视化模块
    // test_data_visualization();

    test_edp_plot();
    test_gmp_plot();

    return 0;
}