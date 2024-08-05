#include "gmp_library.h"

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning(suppress : 4996)

#define INITIAL_SIZE 1e4 // 初始分配的内存大小

int main()
{
    FILE *file;
    int *array;
    int size = 0;
    int capacity = INITIAL_SIZE; // 当前分配的内存大小
    int num;

    // 分配初始内存
    array = (int *)malloc(capacity * sizeof(int));
    if (array == NULL)
    {
        perror("内存分配失败");
        return EXIT_FAILURE;
    }

    // 打开文件
    file = fopen("acceleration_data/acc0.txt", "r");
    if (file == NULL)
    {
        perror("无法打开文件");
        free(array);
        return EXIT_FAILURE;
    }

    // 读取数据
    while (fscanf(file, "%d", &num) == 1)
    {
        // 如果数组已满，扩大内存
        if (size >= capacity)
        {
            capacity *= 2; // 双倍增加内存
            int *new_array = (int *)realloc(array, capacity * sizeof(int));
            if (new_array == NULL)
            {
                perror("内存重新分配失败");
                fclose(file);
                free(array);
                return EXIT_FAILURE;
            }
            array = new_array;
        }
        array[size++] = num;
    }

    // 关闭文件
    fclose(file);

    respons_spectrum *result_ptr = ResponseSpectrum(50, 0.05, array, size);

    // 释放内存
    free(array);

    return EXIT_SUCCESS;
}
