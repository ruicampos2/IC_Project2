#include <iostream>
#include <bitset>
#include <cmath>

class golomb {
private:
    int m; // Golomb parameter
    bool useSignAndMagnitude; // Flag para determinar a abordagem de codificação para números negativos

public:
    golomb(int parameter_m) : m(parameter_m), useSignAndMagnitude(true) {}

    // Função para encode um inteiro, levando em conta a codificação para números negativos
    std::string encode(int value) const {
    int quotient, remainder;

    if (value >= 0 || useSignAndMagnitude) {
        // Se o valor for positivo ou a abordagem for sign and magnitude, use a codificação padrão
        quotient = value / m;
        remainder = value % m;
    } else {
        // Se o valor for negativo e a abordagem for interleaving, codifique usando interleaving
        quotient = abs(value) / m;
        remainder = abs(value) % m;
        remainder = 2 * remainder + ((value < 0) ? 1 : 0);
    }

    // Calculate Golomb code
    std::string golombCode = std::string(quotient + 1, '1') + '0';
    golombCode += std::bitset<sizeof(int) * 8>(remainder).to_string().substr(std::bitset<sizeof(int) * 8>(remainder).size() - static_cast<size_t>(log2(m)));

    return golombCode;
}

    // Função para decode um Golomb code para um inteiro, levando em conta a codificação para números negativos
    int decode(const std::string& golombCode, size_t& startIdx) const {
        size_t idx = golombCode.find('0', startIdx) + 1;
        int quotient = idx - 1;

        // Extract the binary representation of remainder
        std::string remainderBinary = golombCode.substr(idx, (size_t)log2(m));

        int remainder = std::bitset<sizeof(int) * 8>(remainderBinary).to_ulong();

        // Se a abordagem for interleaving, ajuste o remainder para obter o valor negativo correto
        if (!useSignAndMagnitude && (remainder % 2 == 1)) {
            remainder = (remainder - 1) / 2;
            quotient = -quotient - 1;
        }

        // Update the startIdx for the next decoding
        startIdx = idx + (size_t)log2(m);

        return quotient * m + remainder;
    }


    // Função para toggle encoding/decoding approach for negative numbers
    void setNegativeEncoding(bool useSignAndMagnitudeFlag) {
        useSignAndMagnitude = useSignAndMagnitudeFlag;
    }
};

