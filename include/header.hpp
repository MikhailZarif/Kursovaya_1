// Created by enenra on 19.11.2021.

#ifndef KURSOVAYA_1_HEADER_HPP
#define KURSOVAYA_1_HEADER_HPP

#include "nlohmann/json.hpp"
#include <boost/filesystem.hpp>
#include <algorithm>
#include <string>

using json = nlohmann::json;

class function {
private:
    std::string func;
public:
    function() = default;

    function(std::string &f) {
        func = f;
    }
};

size_t powerX(std::string &X) {
    size_t res = 0;
    std::string cut;
    for (char &i: X)
        if ((std::find(cut.begin(), cut.end(), i) == cut.end()) || (cut.empty())) {
            cut += i;
            ++res;
        }
    return res;
}

bool absolute_stability (std::string& text, std::vector<int>& keys)
{
    if (powerX(text)<=keys.size())
        return true;
    return false;
}

#endif //KURSOVAYA_1_HEADER_HPP
