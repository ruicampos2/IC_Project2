#include <iostream>
#include <bitset>
#include <cmath>

class golomb {
private:
    int m; // Golomb parameter

public:
    golomb(int parameter_m) : m(parameter_m) {}

    // Function to encode an integer
    std::string encode(int value) const {
        int quotient = value / m;
        int remainder = value % m;

        // Calculate Golomb code
        std::string golombCode = std::string(quotient + 1, '1') + '0';
        golombCode += std::bitset<sizeof(int) * 8>(remainder).to_string().substr(std::bitset<sizeof(int) * 8>(remainder).size() - static_cast<size_t>(log2(m)));


        return golombCode;
    }

    // Function to decode a Golomb code into an integer
    int decode(const std::string& golombCode, size_t& startIdx) const {
        size_t idx = golombCode.find('0', startIdx) + 1;
        int quotient = idx - 1;

        // Extract the binary representation of remainder
        std::string remainderBinary = golombCode.substr(idx, (size_t)log2(m));

        int remainder = std::bitset<sizeof(int) * 8>(remainderBinary).to_ulong();

        // Update the startIdx for the next decoding
        startIdx = idx + (size_t)log2(m);

        return quotient * m + remainder;
    }


    // Function to toggle encoding/decoding approach for negative numbers
    void setNegativeEncoding(bool useSignAndMagnitude) {
        // Additional implementation if needed
    }
};

