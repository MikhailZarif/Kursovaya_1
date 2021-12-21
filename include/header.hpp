// Created by enenra on 19.11.2021.

#ifndef KURSOVAYA_1_HEADER_HPP
#define KURSOVAYA_1_HEADER_HPP

#include "nlohmann/json.hpp"
#include <boost/filesystem.hpp>
#include <algorithm>
#include <regex>
#include <string>
#include <iomanip>
#include <cmath>
#include <ctime>

using json = nlohmann::json;

size_t power(std::string &X);

size_t number_degree(const size_t &val);

int number_degree(const int &val);

class boolean {
private:
    std::vector<int> vec;

public:
    boolean()=default;

    explicit boolean(std::string &init) {
        for (size_t i = 0; i < init.size(); ++i) {
            if ((init[i] == '0') || (init[i] == '1')) {
                if (i == 0)
                    if ((init[i + 1] == '0') || (init[i + 1] == '1')) {
                        if (init[i] == '0')
                            vec.push_back(0);
                        else vec.push_back(1);
                    }
                if ((i > 0) && (i < init.size() - 1))
                    if ((init[i + 1] == '0') || (init[i + 1] == '1') || (init[i - 1] == '0') || (init[i - 1] == '1')) {
                        if (init[i] == '0')
                            vec.push_back(0);
                        else vec.push_back(1);
                    }
                if (i == init.size() - 1) {
                    if ((init[i - 1] == '0') || (init[i - 1] == '1')) {
                        if (init[i] == '0')
                            vec.push_back(0);
                        else vec.push_back(1);
                    }
                }
            }
        }
    }

    boolean(int val, int deg) {
        for (int i = 0; i < deg; ++i)
            vec.push_back(0);
        while (deg > 0) {
            vec[deg - 1] = val % 2;
            val /= 2;
            --deg;
        }
    }

    ~boolean() = default;

    void push_back(int val) {
        vec.push_back(val);
    }

    bool empty() {
        if (vec.empty())
            return true;
        return false;
    }

    int degree() {
        int deg = 0;
        while (pow(2.0, deg) != vec.size())
            deg += 1;
        return deg;
    }

    boolean operator+=(boolean &r) {
        for (size_t i = 0; i < vec.size(); ++i)
            vec[i] ^= r.vec[i];
        return *this;
    }

    boolean operator*=(boolean &r) {
        for (size_t i = 0; i < vec.size(); ++i)
            vec[i] &= r.vec[i];
        return *this;
    }

    int &operator[](size_t &index) {
        return vec[index];
    }

    int &operator[](int index) {
        return vec[index];
    }

    size_t to_number() {
        size_t result = 0;
        for (size_t i = 0; i < vec.size(); ++i)
            result += vec[i] * pow(2.0, vec.size() - 1 - i);
        return result;
    }

    bool operator==(const boolean& r) const {
        if (vec.size() != r.vec.size())
            return false;
        else
            for (size_t i = 0; i < vec.size(); ++i)
                if (vec[i] != r.vec[i])
                    return false;
        return true;
    }

    bool operator!=(const boolean &r) const {
        if (vec.size() != r.vec.size())
            return true;
        else
            for (size_t i = 0; i < vec.size(); ++i)
                if (vec[i] != r.vec[i])
                    return true;
        return false;
    }
};

class cryptalgorithm {
private:
    std::string text;
    std::string cipher;
    std::vector<int> keys;
    std::vector<std::string> functions;

public:
    cryptalgorithm() = default;

    cryptalgorithm(std::string &t, std::string &c, std::vector<int> &k, std::vector<std::string> &f) {
        text = t;
        cipher = c;
        keys = k;
        functions = f;
    }

    ~cryptalgorithm() = default;


    void absolute_stability(std::ofstream &out) {
        out << '\n' << " - ANALYZING ABSOLUTE STABILITY:" << '\n';
        if (power(text) <= keys.size())
            out << '\n' << "Your algorithm is absolutely stable" << '\n';
        else out << '\n' << "Your algorithm is not absolutely stable" << '\n';
        out << '\n';
    }

    void differential_attack(std::ofstream &out) {
        out << '\n' << " - DIFFERENTIAL ATTACK:" << '\n';
        if (functions.size() == 1) {
            out
                    << "Your algorithm does not cannot ba attacked with differential attack, or entered functions are incorrect"
                    << '\n';
            out
                    << "Make sure. that, if Your encryption function is a permutation, You enter it as a system of coordinate boolean functions"
                    << '\n';
            out << "Boolean functions are entered as a vector of 0 and 1" << '\n';
        } else {
            std::vector<boolean> system;
            for (auto &function: functions) {
                boolean f(function);
                if (!f.empty())
                    system.push_back(f);
            }
            if (system.empty()) {
                out
                        << "Your algorithm does not cannot ba attacked with differential attack, or entered functions are incorrect"
                        << '\n';
                out
                        << "Make sure. that, if Your encryption function is a permutation, You enter it as a system of coordinate boolean functions"
                        << '\n';
                out << "Boolean functions are entered as a vector of 0 and 1" << '\n';
            } else {
                unsigned int start = clock();
                int deg = system[0].degree();
                size_t n;
                if (text == "ASCII")
                    n = 256;
                else n = text.size();
                auto **table = new size_t *[n];
                for (size_t k = 0; k < n; ++k) {
                    table[k] = new size_t[n];
                    boolean delta_x(k, deg);
                    for (size_t i = 0; i < n; ++i) {
                        boolean delta_y, y;
                        for (auto &j: system)
                            y.push_back(j[i]);
                        boolean x(i, deg);
                        x += delta_x;
                        size_t index = x.to_number();
                        for (auto &j: system)
                            delta_y.push_back(j[index]);
                        delta_y += y;
                        size_t eq = delta_y.to_number();
                        table[k][i] = eq;
                    }
                    std::vector<size_t> v;
                    for (size_t i = 0; i < n; ++i)
                        v.push_back(0);
                    for (size_t r = 0; r < n; ++r)
                        v[table[k][r]] += 1;
                    for (size_t i = 0; i < n; ++i)
                        table[k][i] = v[i];
                }
                unsigned int end = clock();
                unsigned int time = end - start;
                // Вывод данных
                out << '\n';
                for (size_t i = 0; i <= n; ++i) {
                    for (size_t j = 0; j <= n; ++j) {
                        if (i * j == 0) {
                            if ((i == 0) && (j == 0))
                                out << "|     ";
                            if ((i == 0) && (j != 0)) {
                                size_t val = j - 1;
                                boolean output(val, deg);
                                out << ' ';
                                for (int k = 0; k < deg; ++k)
                                    out << output[k];
                            }
                            if ((i != 0) && (j == 0)) {
                                size_t val = i - 1;
                                boolean output(val, deg);
                                out << "| ";
                                for (int k = 0; k < deg; ++k)
                                    out << output[k];
                            }
                            out << " |";
                        } else {
                            out << "  " << table[i - 1][j - 1];
                            for (size_t h = 0; h < deg - number_degree(table[i - 1][j - 1]); ++h)
                                out << ' ';
                            out << '|';
                        }
                    }
                    out << '\n';
                }
                out
                        << '\n'
                        << "Estimated time of analyzing given S-block to find most probable outcomes from certain entered text is "
                        << time << " ns" << '\n';
                for (size_t i = 0; i < n; ++i)
                    delete[] table[i];
                delete[] table;
            }
        }
    }

    void brute_force(std::ofstream &out) {
        out << '\n' << " - BRUTE FORCE ATTACK:" << '\n';
        size_t n;
        if (text == "ASCII")
            n = 256;
        else
            n = text.size();
        out << '\n' << "This attack is always very long and needs many resources, in your case it will take about ";
        out << n << " factorial, multiplied by the time of one tact." << '\n';
    }

    void linear_attack(std::ofstream &out) {
        out << '\n' << " - LINEAR ATTACK:" << '\n';
        if (functions.size() == 1) {
            out
                    << "Your algorithm does not cannot ba attacked with linear attack, or entered functions are incorrect"
                    << '\n';
            out
                    << "Make sure. that, if Your encryption function is a permutation, You enter it as a system of coordinate boolean functions"
                    << '\n';
            out << "Boolean functions are entered as a vector of 0 and 1" << '\n';
        } else {
            std::vector<boolean> system;
            for (auto &function: functions) {
                boolean f(function);
                if (!f.empty())
                    system.push_back(f);
            }
            if (system.empty()) {
                out
                        << "Your algorithm does not cannot ba attacked with linear attack, or entered functions are incorrect"
                        << '\n';
                out
                        << "Make sure. that, if Your encryption function is a permutation, You enter it as a system of coordinate boolean functions"
                        << '\n';
                out << "Boolean functions are entered as a vector of 0 and 1" << '\n';
            } else {
                unsigned int start = clock();
                size_t n, deg = system[0].degree();
                if (text == "ASCII")
                    n = 256;
                else n = text.size();
                auto **table = new int *[n];
                for (size_t i = 0; i < n; ++i) {
                    table[i] = new int[n];
                    boolean x_linear(i, deg);
                    for (size_t j = 0; j < n; ++j) {
                        table[i][j] = 0;
                        boolean y_linear(j, deg);
                        for (size_t h = 0; h < n; ++h) {
                            boolean alfa(h, deg);
                            boolean beta;
                            for (auto &k: system)
                                beta.push_back(k[h]);
                            alfa *= x_linear;
                            beta *= y_linear;
                            int x = 0, y = 0;
                            for (size_t t = 0; t < deg; ++t) {
                                x ^= alfa[t];
                                y ^= beta[t];
                            }
                            if (x == y)
                                table[i][j] += 1;
                        }
                        table[i][j] = table[i][j] - n / 2;
                    }
                }
                unsigned int end = clock();
                unsigned int time = end - start;
                // Вывод данных
                out << '\n';
                for (size_t i = 0; i <= n; ++i) {
                    for (size_t j = 0; j <= n; ++j) {
                        if (i * j == 0) {
                            if ((i == 0) && (j == 0))
                                out << "|     ";
                            if ((i == 0) && (j != 0)) {
                                size_t val = j - 1;
                                boolean output(val, deg);
                                out << ' ';
                                for (size_t k = 0; k < deg; ++k)
                                    out << output[k];
                            }
                            if ((i != 0) && (j == 0)) {
                                size_t val = i - 1;
                                boolean output(val, deg);
                                out << "| ";
                                for (size_t k = 0; k < deg; ++k)
                                    out << output[k];
                            }
                            out << " |";
                        } else {
                            if (table[i - 1][j - 1] > 0) {
                                out << " +" << table[i - 1][j - 1];
                                for (size_t h = 0; h < deg - number_degree(table[i - 1][j - 1]); ++h)
                                    out << ' ';
                                out << '|';
                            }
                            if (table[i - 1][j - 1] == 0) {
                                out << "  " << table[i - 1][j - 1];
                                for (size_t h = 0; h < deg - 1; ++h)
                                    out << ' ';
                                out << '|';
                            }
                            if (table[i - 1][j - 1] < 0) {
                                out << ' ' << table[i - 1][j - 1];
                                for (size_t h = 0; h < deg - number_degree(table[i - 1][j - 1]); ++h)
                                    out << ' ';
                                out << '|';
                            }
                        }
                    }
                    out << '\n';
                }
                out
                        << '\n'
                        << "Estimated time to build needed approximations and analyse the probability of destabilization of given S-block is "
                        << time << " ns" << '\n';
                for (size_t i = 0; i < n; ++i)
                    delete[] table[i];
                delete[] table;
            }
        }
    }
};

#endif //KURSOVAYA_1_HEADER_HPP
