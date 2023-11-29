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

    // Read the Golomb-encoded stereo audio file
    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }

    // Read Golomb-encoded data from the file
    std::vector<char> encodedData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    // Close the input file
    inputFile.close();

    // Initialize Golomb codec with parameter m
    golomb golomb(m);

    // Perform Golomb decoding on the encoded data
    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return 1;
    }

    // Decode the entire bit stream for both channels
    std::vector<std::pair<int, int>> decodedValues;
    std::string bitStream = "";
    for (char encodedSample : encodedData) {
        std::bitset<8> bits(encodedSample);
        bitStream += bits.to_string();
    }

    // Decode the entire bit stream for both channels
    for (size_t i = 0; i < bitStream.size();) {
        int decodedValueLeft = golomb.decode(bitStream, i);
        int decodedValueRight = golomb.decode(bitStream, i);
        decodedValues.emplace_back(decodedValueLeft, decodedValueRight);
    }

    // Write decoded stereo audio data to the output file
    for (const auto& decodedValue : decodedValues) {
        outputFile.write(reinterpret_cast<const char*>(&decodedValue.first), sizeof(int));
        outputFile.write(reinterpret_cast<const char*>(&decodedValue.second), sizeof(int));
    }

    // Close the output file
    outputFile.close();

    std::cout << "Decoding complete." << std::endl;

    return 0;
}
