//
// Created by dawid on 28.03.17.
//

#ifndef PROJECT_SBGMDISPARITYCALCULATOR_HPP
#define PROJECT_SBGMDISPARITYCALCULATOR_HPP

#include <string>
#include <opencv2/core/mat.hpp>
#include "DisparityCalculator.hpp"

class SGBMDisparityCalculator : public DisparityCalculator {
public:
    SGBMDisparityCalculator(std::string leftImagePath, std::string rightImagePath);
    void calculate(cv::Mat &outputImage) override ;

private:
    std::string mLeftImagePath;
    std::string mRightImagePath;
};


#endif //PROJECT_DISPARITYCALCULATOR_HPP
