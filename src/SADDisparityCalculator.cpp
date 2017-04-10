//
// Created by dawid on 29.03.17.
//

#include <stdexcept>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv/cv.hpp>
#include <iostream>
#include "SADDisparityCalculator.hpp"


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


    const int blockSize = std::min(imageSize.width, imageSize.height)/50;
    auto cropWidth = imageSize.width%blockSize;
    auto cropHeight = imageSize.height%blockSize;


    leftImage = leftImage(cv::Rect(0,0,imageSize.width-cropWidth, imageSize.height - cropHeight));
    rightImage = rightImage(cv::Rect(0,0,imageSize.width-cropWidth, imageSize.height - cropHeight));
    imageSize = leftImage.size();

    cv::Mat finalDisparity(cv::Mat::zeros(imageSize, CV_32S));

    const int probableDisparityValue = imageSize.width/4;
    size_t processedElements=0;
#pragma omp parallel for
    for(decltype(imageSize.height) currentRow =0; currentRow < imageSize.height; currentRow+=blockSize){
        for(decltype(imageSize.width) currentCol=0; currentCol < imageSize.width; currentCol+=blockSize){
            auto compareBlock = leftImage(cv::Rect(currentCol, currentRow, blockSize, blockSize));
            int disparityValue = static_cast<long int>(currentCol) - static_cast<long int>(findSmallestSAD(currentRow, compareBlock, rightImage));
            if(std::abs(disparityValue) > probableDisparityValue or disparityValue < 0){
                disparityValue=0;
            }
            cv::Mat disparityBlock(cv::Mat::ones(compareBlock.size(), CV_32S)*disparityValue);
            disparityBlock.copyTo(finalDisparity(cv::Rect(currentCol, currentRow, blockSize, blockSize)));
        }


#pragma omp critical
        {
            processedElements++;
            std::cout << "processed: " << processedElements << " out of: " << imageSize.height/blockSize <<
                 "\t\t\t" << static_cast<float>(processedElements)/(imageSize.height/blockSize)*100  << "%" <<std::endl;
        };
    }
    std::cout << "Processing finished" << std::endl;

    finalDisparity.convertTo(finalDisparity, CV_8U);
    cv::medianBlur(finalDisparity,finalDisparity,blockSize*2+1);
    double min,max;
    cv::minMaxLoc(finalDisparity, &min, &max);
    finalDisparity.convertTo(finalDisparity, static_cast<int>(255.0f/(max-min)));
    outputImage = finalDisparity;
}

int SADDisparityCalculator::findSmallestSAD(int row, const cv::Mat &block, const cv::Mat &image) {
    size_t smallestSAD= std::numeric_limits<size_t>::max();
    int smallestSADOffset = 0;

    for(decltype(image.cols) currentX=0; currentX < image.cols - block.cols; ++currentX){
        cv::Mat comparedBlock = image(cv::Rect(currentX, row, block.cols, block.rows));

        size_t SAD = static_cast<size_t>(cv::sum(cv::abs(block - comparedBlock))[0]);
        if(SAD < smallestSAD){
            smallestSAD = SAD;
            smallestSADOffset = currentX;
        }
    }

    return  smallestSADOffset;
}
