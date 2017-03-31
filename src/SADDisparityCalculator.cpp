//
// Created by dawid on 29.03.17.
//

#include <stdexcept>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv/cv.hpp>
#include <iostream>
#include "SADDisparityCalculator.hpp"
#include <omp.h>


SADDisparityCalculator::SADDisparityCalculator(std::string leftImagePath, std::string rightImagePath)
    : mLeftImagePath(leftImagePath),
      mRightImagePath(rightImagePath)
{}

void SADDisparityCalculator::calculate(cv::Mat &outputImage) {
    auto leftImage = cv::imread(mLeftImagePath, CV_LOAD_IMAGE_GRAYSCALE);
    auto rightImage = cv::imread(mRightImagePath, CV_LOAD_IMAGE_GRAYSCALE);

    if(leftImage.empty() or rightImage.empty()){
        throw std::invalid_argument("Provided image paths are wrong!");
    }

    if(leftImage.size() != rightImage.size()){
        throw std::invalid_argument("Provided images are of different sizes");
    }
    cv::Size imageSize = leftImage.size();

    leftImage.convertTo(leftImage, CV_32S);
    rightImage.convertTo(rightImage, CV_32S);


    const size_t blockSize = std::min(imageSize.width, imageSize.height)/50;//5 ;
    auto cropWidth = imageSize.width%blockSize;
    auto cropHeight = imageSize.height%blockSize;


    leftImage = leftImage(cv::Rect(0,0,imageSize.width-cropWidth, imageSize.height - cropHeight));
    rightImage = rightImage(cv::Rect(0,0,imageSize.width-cropWidth, imageSize.height - cropHeight));
    imageSize = leftImage.size();

    cv::Mat finalDisparity(cv::Mat::zeros(imageSize, CV_32S));

    const size_t probableDisparityValue = imageSize.width/4;
    size_t processedElements=0;
#pragma omp parallel for
    for(size_t currentRow =0; currentRow < imageSize.height; currentRow+=blockSize){
        for(size_t currentCol=0; currentCol < imageSize.width; currentCol+=blockSize){
            auto compareBlock = leftImage(cv::Rect(currentCol, currentRow, blockSize, blockSize));
            long int disparityValue = static_cast<long int>(currentCol) - static_cast<long int>(findSmallestSAD(currentRow, compareBlock, rightImage));
            if(std::abs(disparityValue) > probableDisparityValue or disparityValue < 0){
                disparityValue=0;
            }
            cv::Mat disparityBlock(cv::Mat::ones(compareBlock.size(), CV_32S)*disparityValue);
            disparityBlock.copyTo(finalDisparity(cv::Rect(currentCol, currentRow, blockSize, blockSize)));
        }
        processedElements++;

        if(omp_get_thread_num()==0)
        {
            std::cout << "processed: " << processedElements << " out of: " << imageSize.height/blockSize << std::endl;
        }
    }
    std::cout << "Processing finished" << std::endl;

    finalDisparity.convertTo(finalDisparity, CV_8U);
    cv::medianBlur(finalDisparity,finalDisparity,blockSize*2+1);
    double min,max;
    cv::minMaxLoc(finalDisparity, &min, &max);
    finalDisparity.convertTo(finalDisparity, 255.0/(max-min));
    outputImage = finalDisparity;
}

size_t SADDisparityCalculator::findSmallestSAD(size_t row, const cv::Mat &block, const cv::Mat &image) {
    size_t smallestSAD= std::numeric_limits<size_t>::max();
    size_t smallestSADOffset = 0;

    for(size_t currentX=0; currentX < image.cols - block.cols; ++currentX){
        cv::Mat comparedBlock = image(cv::Rect(currentX, row, block.cols, block.rows));

        size_t SAD = cv::sum(cv::abs(block - comparedBlock))[0];
        if(SAD < smallestSAD){
            smallestSAD = SAD;
            smallestSADOffset = currentX;
        }
    }

    return  smallestSADOffset;
}
