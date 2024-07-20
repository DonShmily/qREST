#ifndef TEST_TEST_FUNCTION_H_
#define TEST_TEST_FUNCTION_H_

#include "header.inc"

// 测试butter函数
int butter_test_main();

// 测试滤波器
int filter_test_main();

// 测试滤波积分算法
void test_filter_integrate();

// 测试改进的滤波积分算法
void test_modified_filter_integrate();

#endif // TEST_TEST_FUNCTION_H_
