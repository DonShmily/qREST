#ifndef GMP_LIBRARY_H_
#define GMP_LIBRARY_H_

#ifdef __cplusplus
extern "C"
{
#endif

    // 反应谱计算结果
    typedef struct
    {
        // 反应谱计算结果
        double *Sa;
        double *Sv;
        double *Sd;

        // 以下两个参数是希望能从设置中读取，作为参数提供给计算反应谱的函数
        // 不过目前还没有设置这个功能，故暂时在程序内使用了比较通用的值

        // 计算结果长度
        int result_size;
        // 反应谱横轴间隔
        double dt;
    } response_spectrum;

    // 计算反应谱
    __declspec(dllexport) response_spectrum *
    ResponseSpectrum(double frequency,
                     double damping_ratio,
                     const double *acceleration,
                     int size);

    // 计算拟反应谱
    __declspec(dllexport) response_spectrum *
    PseudoResponseSpectrum(double frequency,
                           double damping_ratio,
                           const double *acceleration,
                           int size);

    // 释放反应谱结果内存
    __declspec(dllexport) void FreeMemory(response_spectrum *memory);

    // 计算Fourier幅值谱
    __declspec(dllexport) double *FourierSpectrum(const double *acceleration,
                                                  int size);

    // 释放double数组内存
    __declspec(dllexport) void FreeArray(double *memory);

#ifdef __cplusplus
}
#endif
#endif // CPP_M_FUNC_H/