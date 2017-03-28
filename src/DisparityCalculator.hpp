//
// Created by dawid on 28.03.17.
//

#ifndef PROJECT_DISPARITYCALCULATOR_HPP
#define PROJECT_DISPARITYCALCULATOR_HPP

#include <string>
#include <opencv2/core/mat.hpp>

class DisparityCalculator {
public:
    DisparityCalculator(std::string leftImagePath, std::string rightImagePath);
    void calculate(cv::Mat& outputImage);

private:
    std::string mLeftImagePath;
    std::string mRightImagePath;
};


#endif //PROJECT_DISPARITYCALCULATOR_HPP
