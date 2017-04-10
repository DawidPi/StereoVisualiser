//
// Created by dawid on 29.03.17.
//

#ifndef PROJECT_SADDISPARITYCALCULATOR_HPP
#define PROJECT_SADDISPARITYCALCULATOR_HPP

#include "DisparityCalculator.hpp"

class SADDisparityCalculator : public DisparityCalculator {
public:
    SADDisparityCalculator(std::string leftImagePath, std::string rightImagePath);
    void calculate(cv::Mat &outputImage) override;

private:
    std::string mLeftImagePath;
    std::string mRightImagePath;

    int findSmallestSAD(int row, const cv::Mat &block, const cv::Mat &image);
};


#endif //PROJECT_SADDISPARITYCALCULATOR_HPP
