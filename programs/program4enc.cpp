#include <iostream>
#include <fstream>
#include <vector>
#include "golomb.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " input_file output_file m_parameter" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];
    int m = std::stoi(argv[3]);

    // Read the input audio file
    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }

    // Read audio data from the file
    std::vector<int> audioData;
    int sample;
    while (inputFile.read(reinterpret_cast<char*>(&sample), sizeof(int))) {
        audioData.push_back(sample);
    }

    // Close the input file
    inputFile.close();

    // Initialize Golomb codec with parameter m
    golomb golomb(m);

    // Perform Golomb encoding on the audio data
    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return 1;
    }

    // Write Golomb-encoded data to the output file
    for (int sample : audioData) {
        std::string encodedSample = golomb.encode(sample);
        outputFile.write(encodedSample.c_str(), encodedSample.size());
    }

    // Close the output file
    outputFile.close();

    std::cout << "Encoding complete." << std::endl;

    return 0;
}
