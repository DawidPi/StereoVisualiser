#include <vector>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include <string>
#include "DisparityRenderingEngine.hpp"
#include "DisparityCalculator.hpp"
#include <boost/program_options.hpp>
#include <opencv/cv.hpp>

namespace program_options=boost::program_options;

bool disparityMapProvided(const char *disparityMapOption, const char *leftImage, const char *rightImage,
                          const program_options::variables_map &availableOptions);

bool disparityMapShouldBeCalculated(const char *disparityMapOption, const char *leftImage, const char *rightImage,
                                    const program_options::variables_map &availableOptions);

int main(int argc, const char* argv[])
{
    const auto helpOption("help");
    const auto disparityMapOption("disparityMapPath");
    const auto leftImage = "leftImage";
    const auto rightImage = "rightImage";

    program_options::options_description description("Welcome to Stereo Visualiser v1.0 created by Dawid Pilarski.\n"
    "Please get familiar with available options");
    description.add_options()
            (helpOption, "display this view")
            (disparityMapOption, program_options::value<std::string>(), "Path to the image"
                    "containing disparity map. If this option is passed, no other should be.")
            (leftImage, program_options::value<std::string>(), "Path to the left image"
                    "of the stereo-image. If this option is passed, the right image should also be passed.")
            (rightImage, program_options::value<std::string>(), "Path to the right image"
                    "of the stereo-image");

    program_options::variables_map availableOptions;
    program_options::store(program_options::parse_command_line(argc,argv, description), availableOptions);
    program_options::notify(availableOptions);

    if (availableOptions.count(helpOption)) {
        std::cout << description << std::endl;
        return 0;
    }

    if(disparityMapProvided(disparityMapOption, leftImage, rightImage, availableOptions)){
        std::string imagePath(availableOptions[disparityMapOption].as<std::string>());
        DisparityRenderingEngine window(imagePath);
        window.startWindow("StereoVisualiser");
        return 0;
    }

    if(disparityMapShouldBeCalculated(disparityMapOption, leftImage, rightImage, availableOptions)){
        std::string leftImagePath = availableOptions[leftImage].as<std::string>();
        std::string rightImagePath = availableOptions[rightImage].as<std::string>();

        DisparityCalculator calculator(leftImagePath, rightImagePath);
        cv::Mat disparityImage;
        calculator.calculate(disparityImage);
        DisparityRenderingEngine engine(disparityImage);
        engine.startWindow("StereoVisualiser");
        return 0;
    }

    std::cout << description << std::endl;

    return 1;
}

bool disparityMapShouldBeCalculated(const char *disparityMapOption, const char *leftImage, const char *rightImage,
                                    const program_options::variables_map &availableOptions) {
    return availableOptions.count(leftImage) and availableOptions.count(rightImage) and !availableOptions.count(disparityMapOption);
}

bool disparityMapProvided(const char *disparityMapOption, const char *leftImage, const char *rightImage,
                          const program_options::variables_map &availableOptions) {
    return availableOptions.count(disparityMapOption) and
           (!availableOptions.count(leftImage) or !availableOptions.count(rightImage));
}