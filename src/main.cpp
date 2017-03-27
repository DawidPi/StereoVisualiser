#include <vector>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include "DisparityRenderingEngine.hpp"

int main(int argc, char* argv[])
{

    if(argc != 2){
        std::cerr << "No image given as argument";
        return 0;
    }
    std::string imagePath(argv[1]);

    DisparityRenderingEngine window(imagePath);

    window.startWindow(500,500, "StereoVisualiser");
    return 0;
}