//
// Created by dawid on 29.03.17.
//

#ifndef PROJECT_DISPARITYCALCULATOR_HPP
#define PROJECT_DISPARITYCALCULATOR_HPP

#include <opencv2/core/mat.hpp>

class DisparityCalculator{
public:
    virtual void calculate(cv::Mat &outputImage)=0;
    virtual ~DisparityCalculator(){};
};

#endif //PROJECT_DISPARITYCALCULATOR_HPP
