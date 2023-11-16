#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void createMirrored(const Mat& input, Mat& output, char orientation);

int main(int argc, char** argv) {

    if (argc < 4) {
        cout << "Usage: " << argv[0] << " <input file> <output file> <orientation (h/v)>" << endl;
        return -1;
    }

    // load the image
    Mat sourceImage = imread(argv[1]);
    // check if the image is loaded
    if (sourceImage.empty()) {
        cout << "Failed to load image: " << argv[1] << endl;
        return -1;
    }

    char orientation = argv[3][0];
    Mat mirroredImage;
    createMirrored(sourceImage, mirroredImage, orientation);

    // save the mirrored image
    imwrite(argv[2], mirroredImage);

    cout << "Mirrored image saved in the directory opencv-bin" << endl;

    return 0;
}

void createMirrored(const Mat& input, Mat& output, char orientation) {
    // create an output image with the same size and type as the input
    output = Mat::zeros(input.size(), input.type());

    if (orientation == 'h') {
        // mirror horizontally
        for (int i = 0; i < input.rows; i++) {
            for (int j = 0; j < input.cols; j++) {
                for (int c = 0; c < input.channels(); c++) {
                    output.at<Vec3b>(i, j)[c] = input.at<Vec3b>(i, input.cols - j - 1)[c];
                }
            }
        }
    } else if (orientation == 'v') {
        // mirror vertically
        for (int i = 0; i < input.rows; i++) {
            for (int j = 0; j < input.cols; j++) {
                for (int c = 0; c < input.channels(); c++) {
                    output.at<Vec3b>(i, j)[c] = input.at<Vec3b>(input.rows - i - 1, j)[c];
                }
            }
        }
    } else {
        cout << "Invalid orientation. Use 'h' for horizontal or 'v' for vertical." << endl;
    }
}
