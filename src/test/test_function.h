#ifndef TEST_TEST_FUNCTION_H_
#define TEST_TEST_FUNCTION_H_

#include "header.inc"

#include <string>
#include <vector>


// 测试butter函数
int test_butter();

// 测试滤波器
int test_filter();

// 测试滤波积分算法
void test_filter_integrate();

// 测试改进的滤波积分算法
void test_modified_filter_integrate();

// 测试EDP计算模块
void test_edp_library(const std::string &file_name);

// 测试评估模块
void test_safty_tagging();

// 测试地震动参数计算模块
void test_gmp();
void test_gmp_library(const std::string &file_name);

// 测试类型大小
void test_size();

// 测试数据可视化
void test_data_visualization();

// 测试GMP绘图
void test_gmp_plot();

// 测试EDP绘图
void test_edp_plot();

// 读取文件中的矩阵数据
std::vector<std::vector<double>>
ReadMatrixFromFile(const std::string &filename);

#endif // TEST_TEST_FUNCTION_H_
