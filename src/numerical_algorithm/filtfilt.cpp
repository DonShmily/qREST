/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/numerical_algorithm/filtfilt.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description：
// 数值算法：filtfilt滤波方法类的实现


// associated header
#include "filtfilt.h"

// stdc++ headers
#include <vector>

// third-party headers
#include "eigen3/Eigen/Dense"

namespace numerical_algorithm
{

// filtfilt滤波算法的入口函数
void FiltFilt::Filtering(const Eigen::MatrixXd &input_signal,
                         Eigen::MatrixXd &output_signal)
{
    output_signal.resize(input_signal.rows(), input_signal.cols());
    for (int i = 0; i != input_signal.cols(); ++i)
    {
        std::vector<double> input_vec(input_signal.rows());
        input_vec.assign(input_signal.col(i).data(),
                         input_signal.col(i).data() + input_signal.rows());
        std::vector<double> output_vec;
        filtfilt_single(input_vec, output_vec);
        output_signal.col(i) =
            Eigen::Map<Eigen::VectorXd>(output_vec.data(), output_vec.size());
    }
}

// filtfilt滤波算法的单个信号处理函数
void FiltFilt::filtfilt_single(const std::vector<double> &input_signal,
                               std::vector<double> &output_signal)
{
    int len = static_cast<int>(input_signal.size()); // length of input
    int nfilt =
        static_cast<int>(coefficients_b_.size() > coefficients_a_.size())
            ? coefficients_b_.size()
            : coefficients_a_.size();
    int nfact = static_cast<int>(3 * (nfilt - 1)); // length of edge transients

    if (len <= nfact)
        throw std::domain_error("Input data too short! Data must have length "
                                "more than 3 times filter order.");

    // set up filter's initial conditions to remove DC offset problems at the
    // beginning and end of the sequence
    coefficients_b_.resize(nfilt, 0);
    coefficients_a_.resize(nfilt, 0);

    std::vector<int> rows, cols;
    // rows = [1:nfilt-1           2:nfilt-1             1:nfilt-2];
    AddIndexRange(rows, 0, nfilt - 2);
    if (nfilt > 2)
    {
        AddIndexRange(rows, 1, nfilt - 2);
        AddIndexRange(rows, 0, nfilt - 3);
    }
    // cols = [ones(1,nfilt-1)         2:nfilt-1          2:nfilt-1];
    AddIndexConst(cols, 0, nfilt - 1);
    if (nfilt > 2)
    {
        AddIndexRange(cols, 1, nfilt - 2);
        AddIndexRange(cols, 1, nfilt - 2);
    }
    // data = [1+a(2)    a(3:nfilt)   ones(1,nfilt-2)   -ones(1,nfilt-2)];

    auto klen = rows.size();
    std::vector<double> data;
    data.resize(klen);
    data[0] = 1 + coefficients_a_[1];
    int j = 1;
    if (nfilt > 2)
    {
        for (int i = 2; i < nfilt; i++)
        {
            data[j++] = coefficients_a_[i];
        }
        for (int i = 0; i < nfilt - 2; i++)
        {
            data[j++] = 1.0;
        }
        for (int i = 0; i < nfilt - 2; i++)
        {
            data[j++] = -1.0;
        }
    }

    std::vector<double> leftpad = SubvectorReverse(input_signal, nfact, 1);
    double _2x0 = 2 * input_signal[0];
    std::transform(leftpad.begin(),
                   leftpad.end(),
                   leftpad.begin(),
                   [_2x0](double val) { return _2x0 - val; });

    std::vector<double> rightpad =
        SubvectorReverse(input_signal, len - 2, len - nfact - 1);
    double _2xl = 2 * input_signal[len - 1];
    std::transform(rightpad.begin(),
                   rightpad.end(),
                   rightpad.begin(),
                   [_2xl](double val) { return _2xl - val; });

    double y0;
    std::vector<double> signal1, signal2, zi;

    signal1.reserve(leftpad.size() + input_signal.size() + rightpad.size());
    AppendVector(signal1, leftpad);
    AppendVector(signal1, input_signal);
    AppendVector(signal1, rightpad);

    // Calculate initial conditions
    Eigen::MatrixXd sp =
        Eigen::MatrixXd::Zero(MaxVal(rows) + 1, MaxVal(cols) + 1);
    for (size_t k = 0; k < klen; ++k)
    {
        sp(rows[k], cols[k]) = data[k];
    }
    auto bb =
        Eigen::VectorXd::Map(coefficients_b_.data(), coefficients_b_.size());
    auto aa =
        Eigen::VectorXd::Map(coefficients_a_.data(), coefficients_a_.size());
    Eigen::MatrixXd zzi =
        (sp.inverse()
         * (bb.segment(1, nfilt - 1) - (bb(0) * aa.segment(1, nfilt - 1))));
    zi.resize(zzi.size());

    // Do the forward and backward filtering
    y0 = signal1[0];
    std::transform(zzi.data(),
                   zzi.data() + zzi.size(),
                   zi.begin(),
                   [y0](double val) { return val * y0; });
    filter(signal1, signal2, zi);
    std::reverse(signal2.begin(), signal2.end());
    y0 = signal2[0];
    std::transform(zzi.data(),
                   zzi.data() + zzi.size(),
                   zi.begin(),
                   [y0](double val) { return val * y0; });
    filter(signal2, signal1, zi);
    output_signal =
        SubvectorReverse(signal1, signal1.size() - nfact - 1, nfact);
}

// filtfilt滤波算法的filter函数
void FiltFilt::filter(const std::vector<double> &input_signal,
                      std::vector<double> &output_signal,
                      std::vector<double> zi)
{
    if (coefficients_a_.empty())
        throw std::domain_error("The feedback filter coefficients are empty.");
    if (std::all_of(coefficients_a_.begin(),
                    coefficients_a_.end(),
                    [](double coef) { return coef == 0; }))
        throw std::domain_error("At least one of the feedback filter "
                                "coefficients has to be non-zero.");
    if (coefficients_a_[0] == 0)
        throw std::domain_error(
            "First feedback coefficient has to be non-zero.");

    // Normalize feedback coefficients if a[0] != 1;
    auto a0 = coefficients_a_[0];
    if (a0 != 1.0)
    {
        std::transform(coefficients_a_.begin(),
                       coefficients_a_.end(),
                       coefficients_a_.begin(),
                       [a0](double v) { return v / a0; });
        std::transform(coefficients_b_.begin(),
                       coefficients_b_.end(),
                       coefficients_b_.begin(),
                       [a0](double v) { return v / a0; });
    }

    size_t input_size = input_signal.size();
    size_t filter_order =
        std::max(coefficients_a_.size(), coefficients_b_.size());
    coefficients_b_.resize(filter_order, 0);
    coefficients_a_.resize(filter_order, 0);
    zi.resize(filter_order, 0);
    output_signal.resize(input_size);

    const double *x = &input_signal[0];
    const double *b = &coefficients_b_[0];
    const double *a = &coefficients_a_[0];
    double *z = &zi[0];
    double *y = &output_signal[0];

    for (size_t i = 0; i < input_size; ++i)
    {
        size_t order = filter_order - 1;
        while (order)
        {
            if (i >= order)
                z[order - 1] = b[order] * x[i - order] - a[order] * y[i - order]
                               + z[order];
            --order;
        }
        y[i] = b[0] * x[i] + z[0];
    }
    zi.resize(filter_order - 1);
}

} // namespace numerical_algorithm