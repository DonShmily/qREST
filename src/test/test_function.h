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

// 测试地震动参数计算模块
void test_gmp();

// 读取文件中的矩阵数据
std::vector<std::vector<double>>
readMatrixFromFile(const std::string &filename);

#endif // TEST_TEST_FUNCTION_H_
