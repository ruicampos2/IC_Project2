#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void rotateImage(const Mat& input, Mat& output, int degrees);

int main(int argc, char** argv) {

    if (argc < 4) {
        cout << "Usage: " << argv[0] << " <input file> <output file> <rotation degrees>" << endl;
        return -1;
    }

    // load the image
    Mat sourceImage = imread(argv[1]);
    // check if the image is loaded
    if (sourceImage.empty()) {
        cout << "Failed to load image: " << argv[1] << endl;
        return -1;
    }

    int rotationDegrees = atoi(argv[3]);
    Mat rotatedImage;
    rotateImage(sourceImage, rotatedImage, rotationDegrees);

    // save the rotated image
    imwrite(argv[2], rotatedImage);

    cout << "Rotated image saved in the directory opencv-bin" << endl;

    return 0;
}

void rotateImage(const Mat& input, Mat& output, int degrees) {
    // validate degrees (ensure it's a multiple of 90)
    if (degrees % 90 != 0) {
        cout << "Invalid rotation degrees. Please use a multiple of 90." << endl;
        return;
    }

    // calculate the number of rotations (0, 1, 2, or 3)
    int numRotations = (degrees / 90) % 4;

    // create an output image with the same size and type as the input
    output = Mat::zeros(input.size(), input.type());

    Mat rotatedInput = input.clone();  // make a copy of the input matrix

    for (int r = 0; r < numRotations; ++r) {
        // rotate 90 degrees clockwise
        transpose(rotatedInput, output);
        flip(output, output, 1);
        rotatedInput = output.clone();  // clone the rotated image to rotatedInput for the next iteration
    }
}
