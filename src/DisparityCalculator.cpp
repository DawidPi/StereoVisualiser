//
// Created by dawid on 28.03.17.
//

#include <opencv2/imgcodecs.hpp>
#include "DisparityCalculator.hpp"

DisparityCalculator::DisparityCalculator(std::string leftImagePath, std::string rightImagePath) :
    mLeftImagePath(leftImagePath),
    mRightImagePath(rightImagePath)
{}

void DisparityCalculator::calculate(cv::Mat &outputImage) {
    auto leftImage = cv::imread(mLeftImagePath);
    auto rightImage = cv::imread(mRightImagePath);

    //outputImage = cv::imabsdiff()

}
