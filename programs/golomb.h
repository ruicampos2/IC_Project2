#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <bitset>
#include <cmath>

class Golomb {
    private:
        int minimo;
        int maximo;
        int valores;

        void bit_calc(int m) {
            if (m != 0){
                maximo = ceil(log2(m));
                minimo = maximo - 1;
                valores = pow(2, maximo) - m;
            } else {
                maximo = 0;
                minimo = 0;
                valores = 0;
            }
        }

        std::string string_resto(int n, int rep){
            std::string result = "";
            for (int i = 0; i < rep; i++) {
                result = std::to_string(n % 2) + result;
                n /= 2;
            }

            return result;
        }

        int string_to_int(std::string bit_string) {
            int result = 0;
            for (long unsigned int i = 0; i < bit_string.length(); i++)
                result = result * 2 + (bit_string[i] - '0');

            return result;
        }

    public:
        Golomb() { }

        std::vector<int> decode(std::string string_enc, int m) {
            bit_calc(m);
            std::vector<int> result;
            int i = 0;

            while((long unsigned int) i < string_enc.length()) {
                int quociente = 0;
                while (string_enc[i] == '0') {
                    quociente++;
                    i++;
                }
                i++;
                int resto = 0;
                int j = 0;
                std::string tmp = "";
                if (m != 1){
                    while (j < minimo) {
                        tmp += string_enc[i];
                        i++;
                        j++;
                    }

                    int aux_resto = string_to_int(tmp);
                    if (aux_resto < valores) {
                        resto = aux_resto;
                    } else {
                        tmp += string_enc[i];
                        i++;
                        resto = string_to_int(tmp) - valores;
                    }
                } else {
                    resto = 0;
                    i++;
                }

                int res = quociente * m + resto;
                if (string_enc[i] == '1') {
                    result.push_back(-(res));
                } else {
                    result.push_back(res);
                }

                i++;
            }

            return result;
        }

        std::vector<int> decode_m(std::string string_enc, std::vector<int> m_vector, int block_size) {
            std::vector<int> result;
            int i = 0;
            int m_i = 0;
            int count = 0;
            bit_calc(m_vector[m_i]);
            while((long unsigned int) i < string_enc.length()) {
                    int quociente = 0;
                    while (string_enc[i] == '0') {
                        quociente++;
                        i++;
                    }
                    i++;
                    int resto = 0;
                    int j = 0;
                    std::string tmp = "";
                    if (m_vector[m_i] != 1){
                        while (j < minimo) {
                            tmp += string_enc[i];
                            i++;
                            j++;
                        }
                        
                        int aux_resto = string_to_int(tmp);
                        if (aux_resto < valores) {
                            resto = aux_resto;
                        } else {
                            tmp += string_enc[i];
                            i++;
                            resto = string_to_int(tmp) - valores;
                        }
                    } else {
                        resto = 0;
                        i++;
                    }
                    int res = quociente * m_vector[m_i] + resto;
                    if (string_enc[i] == '1') {
                        result.push_back(-(res));
                    } else {
                        result.push_back(res);
                    }
                    i++;
                    count++;
                   
                    if (count == block_size) {
                        m_i++;
                        count = 0;
                        bit_calc(m_vector[m_i]);
                    }  
                }
            return result;
        }

        std::string encode(int n, int m){
            bit_calc(m);
            std::string result = "";
            int quociente = 0;
            int resto = 0;
            if (m != 0){
                quociente = abs(n) / m;
                resto = abs(n) % m;
            }
            for (int i = 0; i < quociente; i++) {
                result += "0";
            }
            result += "1";
            if (m != 1){
                if (resto < valores) {
                    result += string_resto(resto, minimo);
                } else {
                    result += string_resto(resto + valores, maximo);
                }
            }else{
                result += "0";
            }
            n < 0 ? result += "1" : result += "0";


            return result;
        }
};