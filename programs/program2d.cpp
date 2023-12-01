#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void adjustIntensity(const Mat& input, Mat& output, double factor);

int main(int argc, char** argv) {

    if (argc < 4) {
        cout << "Usage: " << argv[0] << " <input file> <output file> <intensity factor>" << endl;
        return -1;
    }

    // load the image
    Mat sourceImage = imread(argv[1]);
    // check if the image is loaded
    if (sourceImage.empty()) {
        cout << "Failed to load image: " << argv[1] << endl;
        return -1;
    }

    double intensityFactor = atof(argv[3]);
    Mat adjustedImage;
    adjustIntensity(sourceImage, adjustedImage, intensityFactor);

    // save the adjusted image
    imwrite(argv[2], adjustedImage);

    cout << "Adjusted intensity image saved in the directory opencv-bin" << endl;

    return 0;
}

void adjustIntensity(const Mat& input, Mat& output, double factor) {
    // validate factor (ensure it's within a valid range)
    if (factor < -255.0 || factor > 255.0) {
        cout << "Invalid intensity factor. Please use a factor between -255.0 and 255.0." << endl;
        return;
    }

    // calculate the alpha and beta values for addWeighted
    double alpha, beta;

    if (factor >= 0.0) {
        alpha = 1.0 + factor / 255.0;
        beta = 0.0;
    } else {
        alpha = 1.0 + factor / 255.0;
        beta = 0.0;
    }

    // apply the intensity adjustment using addWeighted
    addWeighted(input, alpha, Mat::zeros(input.size(), input.type()), 0.0, beta, output);
}

