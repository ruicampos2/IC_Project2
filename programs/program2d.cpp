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
    if (factor < 0.0 || factor > 2.0) {
        cout << "Invalid intensity factor. Please use a factor between 0.0 and 2.0." << endl;
        return;
    }

    // create an output image with the same size and type as the input
    output = Mat::zeros(input.size(), input.type());

    for (int i = 0; i < input.rows; ++i) {
        for (int j = 0; j < input.cols; ++j) {
            for (int c = 0; c < input.channels(); ++c) {
                // adjust intensity values
                output.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(factor * input.at<Vec3b>(i, j)[c]);
            }
        }
    }
}
