#include "golomb.h"
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <fftw3.h>
#include <sndfile.hh>
#include "bitStream.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

    //function that predicts the next value in the sequence based on 3 previous values
    auto predict = [](int a, int b, int c) {
        //3*a - 3*b + c
        return 3*a - 3*b + c;
    };

    //function to calculate m based on u
    auto calc_m = [](int u) {
        //u = alpha / 1 - alpha
        //m = - (1 / log(alpha))
        return (int) - (1/log((double) u / (1 + u)));
    };

    if (argc < 3 || argc > 4) {
		cerr << "Usage: " << argv[0] << " <input file> <output file> m\n";
		return 1;
	}


    SndfileHandle sfhIn { argv[1] };
	if(sfhIn.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }
    if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}
	if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

    string output = argv[2];
    short m = atoi(argv[3]);

    short bs = m;

    size_t nFrames { static_cast<size_t>(sfhIn.frames()) };
    size_t nChannels { static_cast<size_t>(sfhIn.channels()) };
    vector<short> samples(nChannels * nFrames);
	sfhIn.readf(samples.data(), nFrames);
	size_t nBlocks { static_cast<size_t>(ceil(static_cast<double>(nFrames) / bs)) };

    // Do zero padding, if necessary
	samples.resize(nBlocks * bs * nChannels);

    //calculate the padding done to the end of the file
    int padding = samples.size() - nFrames * nChannels;

    vector<short> left_samples(samples.size() / 2);
    vector<short> right_samples(samples.size() / 2);

    if (nChannels > 1) {
        //split samples into left and right channels
        for (long unsigned int i = 0; i < samples.size()/2; i++) {
            left_samples[i] = samples[i * nChannels];
            right_samples[i] = samples[i * nChannels + 1];
        }
    }

    vector<int> m_vector;
    vector<int> valuesToBeEncoded;
    // cout << "Predicting..." << endl;
    if (nChannels < 2) {
        for(long unsigned int i = 0; i < samples.size(); i++) {
            if (i >= 3) {
                int difference = samples[i] - predict(samples[i-1], samples[i-2], samples[i-3]);
                valuesToBeEncoded.push_back(difference);
            } else {
                valuesToBeEncoded.push_back(samples[i]);
            }
        }
    } else {
        for (long unsigned int i = 0; i < left_samples.size(); i++) {
            if (i >= 3) {
                int difference = left_samples[i] - predict(left_samples[i-1], left_samples[i-2], left_samples[i-3]);
                valuesToBeEncoded.push_back(difference);
            } else {
                valuesToBeEncoded.push_back(left_samples[i]);
            }

            //calculate m every bs samples
            if (i % bs == 0 && i != 0) {
                int sum = 0;
                for (long unsigned int j = i-bs; j < i; j++) {
                    sum += abs(valuesToBeEncoded[j]);
                }
                int u = round(sum/bs);
                m = calc_m(u);
                if (m < 1) m = 1;
                m_vector.push_back(m);  
            }
            if (i == left_samples.size() - 1) {
                int sum = 0;
                for (long unsigned int j = i - (i % bs); j < i; j++) {
                    sum += abs(valuesToBeEncoded[j]);
                }
                int u = round(sum/(i % bs));
                m = calc_m(u);
                if (m < 1) m = 1;
                m_vector.push_back(m);
            }
        }

        for(long unsigned int i = 0; i < left_samples.size(); i++){
            if (i >= 3) {
                int difference = right_samples[i] - predict(right_samples[i-1], right_samples[i-2], right_samples[i-3]);
                valuesToBeEncoded.push_back(difference);
            }
            
            else valuesToBeEncoded.push_back(right_samples[i]);

            //calculate m every bs samples
            if (i % bs == 0 && i != 0) {
                int sum = 0;
                for (long unsigned int j = i-bs; j < i; j++) {
                    sum += abs(valuesToBeEncoded[j]);
                }
                int u = round(sum/bs);
                m = calc_m(u);
                if (m < 1) m = 1;
                m_vector.push_back(m);
            }
            if (i == left_samples.size() - 1) {
                int sum = 0;
                for (long unsigned int j = i - (i % bs); j < i; j++) {
                    sum += abs(valuesToBeEncoded[j]);
                }
                int u = round(sum/(i % bs));
                m = calc_m(u);
                if (m < 1) m = 1;
                m_vector.push_back(m);
            }
        }
    }

    string encodedString = "";
    Golomb g;

    BitStream bitStream(output, "w");
    vector<int> bits;
    vector<int> encoded_bits;
    
    //the next bits will be the encoded string
    for(long unsigned int i = 0; i < encodedString.length(); i++)
        encoded_bits.push_back(encodedString[i] - '0');
    
    //if the encoded string is not a multiple of 8, add 0s to the end of the string
    //until it is a multiple of 8
    int count_zeros = 0;
    while (encoded_bits.size() % 8 != 0) {
        encoded_bits.push_back(0);
        count_zeros++;
    }

    //the first 16 bits of the file are the number of channels
    for(int i = 15; i >= 0; i--) {
        bits.push_back((sfhIn.channels() >> i) & 1);
    }

    //the next 16 bits of the file are the padding
    for(int i = 15; i >= 0; i--) {
        bits.push_back((padding >> i) & 1);
    }

    //the next 32 bits of the file are the number of frames
    for(int i = 31; i >= 0; i--) {
        bits.push_back((samples.size()/2 >> i) & 1);
    }
    // cout << "nFrames: " << nFrames << endl;

    //the next 16 bits is the block size
    for(int i = 15; i >= 0; i--) {
        bits.push_back((bs >> i) & 1);
    }
    // cout << "block size: " << bs << endl;

    //the next 16 bits is the number of zeros added to the end of the encoded string
    for(int i = 15; i >= 0; i--) {
        bits.push_back((count_zeros >> i) & 1);
    }
    // cout << "number of zeros added: " << count_zeros << endl;

    // the next bits will be the values of m_vector converted to binary
    for (long unsigned int i = 0; i < m_vector.size(); i++) {
        for (int j = 15; j >= 0; j--) {
            bits.push_back((m_vector[i] >> j) & 1);
        }
    }
    
    //the next bits are the encoded bits
    for (long unsigned int i = 0; i < encoded_bits.size(); i++)
        bits.push_back(encoded_bits[i]);

    bitStream.writeBits(bits);
    bitStream.close();

    cout << "Encoded audio saved in opencv-bin folder" << endl;

    return 0;
}