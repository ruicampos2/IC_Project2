#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void extractChannel(const Mat& input, Mat& output, int channelNumber);

int main(int argc, char** argv) {

    if (argc < 4) {
        cout << "Usage: " << argv[0] << " <source> <destination> <channel_number>" << endl;
        return -1;
    }

    // load the image
    Mat sourceImage = imread(argv[1]);
    // check if the image is loaded
    if (sourceImage.empty()) {
        cout << "Failed to load image: " << argv[1] << endl;
        return -1;
    }

    int channelNumber = atoi(argv[3]);
    Mat extractedChannel;
    extractChannel(sourceImage, extractedChannel, channelNumber);

    // Convert 
    Mat bgrImage;
    cvtColor(extractedChannel, bgrImage, COLOR_GRAY2BGR);

    // save the BGR image
    imwrite(argv[2], bgrImage);

    cout << "Image saved in the directory opencv-bin" << endl;

    return 0;
}

void extractChannel(const Mat& input, Mat& output, int channelNumber) {
    // create a single-channel image
    output = Mat::zeros(input.size(), CV_8UC1);

    if (input.isContinuous()) {
        // calculate the total number of elements
        int totalElements = input.rows * input.cols * input.channels();

        // extract the specified channel
        for (int i = 0; i < totalElements; i += input.channels()) {
            output.data[i / input.channels()] = input.data[i + channelNumber - 1];
        }
    } else {
        // extract the specified channel
        for (int i = 0; i < input.rows; i++) {
            for (int j = 0; j < input.cols; j++) {
                output.at<uchar>(i, j) = input.at<Vec3b>(i, j)[channelNumber - 1];
            }
        }
    }
}
