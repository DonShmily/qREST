#include <fstream>
#include <iostream>
#include <string>

#include "header.inc"

#include "safty_tagging/based_on_inter_story_drift.h"
#include "test_function.h"

using namespace std;

void test_safty_tagging()
{
    // 读取数据文件
    string file_name = "acceleration_data/accNS.txt";

    std::vector<double> floor, measurement;
    std::ifstream ifs("building/floor.txt");
    double temp;
    while (ifs >> temp)
    {
        floor.push_back(temp);
    }
    ifs.close();
    ifs.open("building/measurement.txt");
    while (ifs >> temp)
    {
        measurement.push_back(temp);
    }
    ifs.close();

    // 创建计算对象
    auto building = data_structure::Building(measurement, floor);
    auto acceleration = data_structure::Acceleration(
        std::vector<std::vector<double>>(), 50, 0.01);
    acceleration.data() = ReadMatrixFromFile(file_name);

    edp_calculation::ModifiedFilteringIntegral m_filt_integral(
        acceleration, building, 2);
    m_filt_integral.CalculateEdp();

    auto result = m_filt_integral.get_filtering_interp_result();
    auto drift = result.get_inter_story_drift();
    auto displacement = result.get_displacement();

    ofstream ofs1("acceleration_data/test_story_drift.txt");
    ofstream ofs2("acceleration_data/test_displacement.txt");
    // 输出结果
    // for (int i = 0; i < drift.data().front().size(); ++i)
    //{
    //    for (int j = 0; j < drift.data().size(); ++j)
    //    {
    //        ofs1 << drift.data().at(j)[i] << " ";
    //        ofs2 << displacement.data().at(j)[i] << " ";
    //    }
    //    ofs1 << endl;
    //    ofs2 << endl;
    //}
    ofs1.close();
    ofs2.close();

    // 安全评价
    safty_tagging::BasedOnInterStoryDrift safty_tagging(result);
    safty_tagging.TagSafty();

    // 获取安全评价结果
    auto safty_result = safty_tagging.get_max_inter_story_drift_result();

    // 输出安全评价结果
    ofstream ofs3("acceleration_data/test_safty_tagging.txt");
    for (int i = 0; i < safty_result.pos_max_inter_story_drift_.size(); ++i)
    {
        ofs3 << safty_result.pos_max_inter_story_drift_.at(i).first << " "
             << safty_result.neg_max_inter_story_drift_.at(i).first << " "
             << safty_result.abs_max_inter_story_drift_.at(i).first << " "
             << "\n";
    }
}