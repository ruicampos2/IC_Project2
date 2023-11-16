#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void createNegative(const Mat& input, Mat& output);

int main(int argc, char** argv) {

    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return -1;
    }

    // load the image
    Mat sourceImage = imread(argv[1]);
    // check if the image is loaded
    if (sourceImage.empty()) {
        cout << "Failed to load image: " << argv[1] << endl;
        return -1;
    }

    Mat negativeImage;
    createNegative(sourceImage, negativeImage);

    // save the negative image
    imwrite(argv[2], negativeImage);

    cout << "Negative image saved in the directory opencv-bin" << endl;

    return 0;
}

void createNegative(const Mat& input, Mat& output) {
    output = Mat::zeros(input.size(), input.type());

    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            for (int c = 0; c < input.channels(); c++) {
                output.at<Vec3b>(i, j)[c] = 255 - input.at<Vec3b>(i, j)[c];
            }
        }
    }
}
