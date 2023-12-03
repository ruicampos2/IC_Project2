#include "golomb.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <fftw3.h>
#include <sndfile.hh>
#include "bitStream.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; 

auto previsão = [](int a, int b, int c) {
    return 3 * a - 3 * b + c;
};

auto calc_m = [](int u) {
    return static_cast<int>(-1 / log(static_cast<double>(u) / (1 + u)));
};

int main(int argc, char *argv[]) {

    if (argc < 4 || argc > 6) {
        cerr << "Usage: " << argv[0] << " <input file> <output file> <m | bs> [auto] [q] \n";
        return 1;
    }

    int q = 0;
    bool modo = false;
    bool quant = false;

    if (argc >= 5) {
        if (strcmp(argv[4], "auto") == 0) {
            modo = true;
        } else {
            q = atoi(argv[4]);
            quant = true;
        }
    }

    if (argc == 6) {
        q = atoi(argv[5]);
        quant = true;
    }

    if (q > 16 || q < 0) {
        cerr << "[q] must be between 1 and 15\n";
        return 1;
    }

    SndfileHandle sfhIn{argv[1]};
    if (sfhIn.error()) {
        cerr << "Error: invalid input file\n";
        return 1;
    }

    if ((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        cerr << "Error: file is not in WAV format\n";
        return 1;
    }

    if ((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        cerr << "Error: file is not in PCM_16 format\n";
        return 1;
    }

    string output = argv[2];
    short m = atoi(argv[3]);

    short bs = m;
    short og = m;

    size_t nFrames{static_cast<size_t>(sfhIn.frames())};
    size_t nChannels{static_cast<size_t>(sfhIn.channels())};
    vector<short> samples(nChannels * nFrames);
    sfhIn.readf(samples.data(), nFrames);
    size_t nBlocks{static_cast<size_t>(ceil(static_cast<double>(nFrames) / bs))};

    samples.resize(nBlocks * bs * nChannels);

    int padding = samples.size() - nFrames * nChannels;

    vector<short> left_samples(samples.size() / 2);
    vector<short> right_samples(samples.size() / 2);

    if (quant) {
        for (long unsigned int i = 0; i < samples.size(); i++) {
            samples[i] = samples[i] >> q; 
        }
    }

    if (nChannels > 1) {
        for (long unsigned int i = 0; i < samples.size() / 2; i++) {
            left_samples[i] = samples[i * nChannels];
            right_samples[i] = samples[i * nChannels + 1];
        }
    }

    vector<int> m_vector;
    vector<int> valores;

    if (nChannels < 2) {
        for (long unsigned int i = 0; i < samples.size(); i++) {
            if (i >= 3) {
                int dif = samples[i] - previsão(samples[i - 1], samples[i - 2], samples[i - 3]);
                valores.push_back(dif);
            } else {
                valores.push_back(samples[i]);
            }
        }
    } else {
        for (long unsigned int i = 0; i < left_samples.size(); i++) {
            if (i >= 3) {
                int dif = left_samples[i] - previsão(left_samples[i - 1], left_samples[i - 2], left_samples[i - 3]);
                valores.push_back(dif);
            } else {
                valores.push_back(left_samples[i]);
            }
            if (i % bs == 0 && i != 0) {
                int sum = 0;
                for (long unsigned int j = i - bs; j < i; j++) {
                    sum += abs(valores[j]);
                }
                int u = round(sum / bs);
                m = calc_m(u);
                if (m < 1) m = 1;
                m_vector.push_back(m);
            }
            if (i == left_samples.size() - 1) {
                int sum = 0;
                for (long unsigned int j = i - (i % bs); j < i; j++) {
                    sum += abs(valores[j]);
                }
                int u = round(sum / (i % bs));
                m = calc_m(u);
                if (m < 1) m = 1;
                m_vector.push_back(m);
            }
        }

        for (long unsigned int i = 0; i < left_samples.size(); i++) {
            if (i >= 3) {
                int dif = right_samples[i] - previsão(right_samples[i - 1], right_samples[i - 2], right_samples[i - 3]);
                valores.push_back(dif);
            } else {
                valores.push_back(right_samples[i]);
            }
            if (i % bs == 0 && i != 0) {
                int sum = 0;
                for (long unsigned int j = i - bs; j < i; j++) {
                    sum += abs(valores[j]);
                }
                int u = round(sum / bs);
                m = calc_m(u);
                if (m < 1) m = 1;
                m_vector.push_back(m);
            }
            if (i == left_samples.size() - 1) {
                int sum = 0;
                for (long unsigned int j = i - (i % bs); j < i; j++) {
                    sum += abs(valores[j]);
                }
                int u = round(sum / (i % bs));
                m = calc_m(u);
                if (m < 1) m = 1;
                m_vector.push_back(m);
            }
        }
    }

    string string_enc = "";
    Golomb g;

    if (!modo) {
        for (long unsigned int i = 0; i < valores.size(); i++)
            string_enc += g.encode(valores[i], og);
    } else {
        int m_index = 0;
        for (long unsigned int i = 0; i < valores.size(); i++) {
            if (i % bs == 0 && i != 0) m_index++;
            string_enc += g.encode(valores[i], m_vector[m_index]);
        }
    }

    BitStream bitStream(output, "w");
    vector<int> bits;
    vector<int> encoded_bits;
    for (long unsigned int i = 0; i < string_enc.length(); i++)
        encoded_bits.push_back(string_enc[i] - '0');

    int count_zeros = 0;
    while (encoded_bits.size() % 8 != 0) {
        encoded_bits.push_back(0);
        count_zeros++;
    }

    for (int i = 15; i >= 0; i--) {
        bits.push_back((sfhIn.channels() >> i) & 1);
    }

    for (int i = 15; i >= 0; i--) {
        bits.push_back((padding >> i) & 1);
    }

    for (int i = 15; i >= 0; i--) {
        bits.push_back((q >> i) & 1);
    }

    for (int i = 31; i >= 0; i--) {
        bits.push_back((samples.size() / 2 >> i) & 1);
    }

    for (int i = 15; i >= 0; i--) {
        bits.push_back((bs >> i) & 1);
    }

    for (int i = 15; i >= 0; i--) {
        bits.push_back((count_zeros >> i) & 1);
    }

    if (!modo) {
        m_vector.clear();
        m_vector.push_back(og);
    }
    for (int i = 15; i >= 0; i--) {
        bits.push_back((m_vector.size() >> i) & 1);
    }

    for (long unsigned int i = 0; i < m_vector.size(); i++) {
        for (int j = 15; j >= 0; j--) {
            bits.push_back((m_vector[i] >> j) & 1);
        }
    }

    for (long unsigned int i = 0; i < encoded_bits.size(); i++)
        bits.push_back(encoded_bits[i]);

    bitStream.writeBits(bits);
    bitStream.close();

    cout << "File has been stored in the 'opencv-bin' directory." << endl;

    return 0;
}
