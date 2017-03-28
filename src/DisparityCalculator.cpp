//
// Created by dawid on 28.03.17.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv/cv.hpp>
#include <opencv2/ximgproc.hpp>
#include <iostream>
#include "DisparityCalculator.hpp"

DisparityCalculator::DisparityCalculator(std::string leftImagePath, std::string rightImagePath) :
    mLeftImagePath(leftImagePath),
    mRightImagePath(rightImagePath)
{}

void DisparityCalculator::calculate(cv::Mat &outputImage) {
    auto leftImage = cv::imread(mLeftImagePath, CV_LOAD_IMAGE_GRAYSCALE);
    auto rightImage = cv::imread(mRightImagePath, CV_LOAD_IMAGE_GRAYSCALE);

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

    for(size_t beginX=0; beginX < outputImage.cols; ++beginX){
        for(size_t beginY=0; beginY < outputImage.rows; ++beginY){
            if(outputImage.at<float>(beginY, beginX) == std::numeric_limits<float>::infinity()){
                outputImage.at<float>(beginY, beginX) = 1;
            }
            if(outputImage.at<float>(beginY, beginX) == -std::numeric_limits<float>::infinity()){
                outputImage.at<float>(beginY, beginX) = 0;
            }
        }
    }
}