//
// Created by dawid on 28.03.17.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv/cv.hpp>
#include <opencv2/ximgproc.hpp>
#include <iostream>
#include "SGBMDisparityCalculator.hpp"

SGBMDisparityCalculator::SGBMDisparityCalculator(std::string leftImagePath, std::string rightImagePath) :
    mLeftImagePath(leftImagePath),
    mRightImagePath(rightImagePath)
{}

void SGBMDisparityCalculator::calculate(cv::Mat &outputImage) {
    auto leftImage = cv::imread(mLeftImagePath, CV_LOAD_IMAGE_GRAYSCALE);
    auto rightImage = cv::imread(mRightImagePath, CV_LOAD_IMAGE_GRAYSCALE);

    if(leftImage.empty() or rightImage.empty()){
        throw std::invalid_argument("Provided image paths are wrong!");
    }

    leftImage.convertTo(leftImage,CV_8U);
    leftImage.convertTo(leftImage,CV_8U);

    auto leftMatcher = cv::StereoBM::create();
    auto rightMatcher = cv::ximgproc::createRightMatcher(leftMatcher);
    auto filter = cv::ximgproc::createDisparityWLSFilter(leftMatcher);


    cv::Mat leftDisparity;
    cv::Mat rightDisparity;
    leftMatcher->compute(leftImage, rightImage, leftDisparity);
    rightMatcher->compute(rightImage, leftImage, rightDisparity);
    filter->filter(leftDisparity, leftImage, outputImage, rightDisparity);

    cv::ximgproc::getDisparityVis(leftDisparity, outputImage);

    outputImage.forEach<float>([](float &element, const int[]){
        if(element == std::numeric_limits<float>::infinity()) element = 1;
        else if (element == -std::numeric_limits<float>::infinity()) element = 0;
    });
}
