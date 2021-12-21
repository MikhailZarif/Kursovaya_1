// Created by enenra on 19.11.2021.

#include "header.hpp"
#include "nlohmann/json.hpp"
#include <algorithm>
#include <string>
#include <cmath>
#include <ctime>

using json = nlohmann::json;

size_t power(std::string &X) {
    size_t res = 0;
    std::string cut;
    for (char &i: X)
        if ((std::find(cut.begin(), cut.end(), i) == cut.end()) || (cut.empty())) {
            cut += i;
            ++res;
        }
    return res;
}

size_t number_degree(const size_t &val) {
    size_t deg = 0, change = val;
    if (val == 0)
        return 1;
    while (change > 0) {
        ++deg;
        change /= 10;
    }
    return deg;
}

int number_degree(const int &val) {
    int deg = 0, change = val;
    if (val == 0)
        return 1;
    while (abs(change) > 0) {
        ++deg;
        change /= 10;
    }
    return deg;
}
