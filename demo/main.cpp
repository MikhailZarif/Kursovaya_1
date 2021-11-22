// Created by enenra on 19.11.2021.
#include "iostream"
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <header.hpp>
#include <stdexcept>
#include <fstream>

using json = nlohmann::json;

int main() {
    std::cout << "Enter Your data" << std::endl;
    // Пользователь имеет три возможности ввести данные:
    //  - через консоль, тогда пишет строку "open text"
    //  - из файла txt, тогда вводит полный путь к файлу
    //  - из файла json, тогда пишет полный путь к файлу
    std::string in, X, Y;
    std::vector<int> K;
    char **function_table = nullptr;
    // X-открытый текст Y-шифртекст K-множество ключей function_table-функция шифрования, представленная в виде таблицы
    // В конце каждой введённой новой переменной в консоли и в txt файле идёт строка с ключевым словом EOT, EOC, EOK (End Of Text/Cipher/Key)
    std::cin >> in;
    if (in == "open_text") {
        std::cout << "Enter Your text, don't forget EOT on the next line after text" << std::endl;
        std::cin >> in;
        while (in != "EOT") {
            X += in;
            std::cin >> in;
        }
        if (X.empty())
            throw std::logic_error{"Text should not be empty"};
        std::cout << "Enter Your encrypted text, don't forget EOC on the next line after encrypted text" << std::endl;
        std::cin >> in;
        while (in != "EOC") {
            Y += in;
            std::cin >> in;
        }
        if (Y.empty())
            throw std::logic_error{"Cipher should not be empty"};
        std::cout
                << "Enter Your key array, put ' ' or ',' between keys, don't forget EOK on the next line after last key"
                << std::endl;
        std::cin >> in;
        while (in != "EOK") {
            std::string key;
            for (char i: in) {
                if ((i != ' ') && (i != ',')) {
                    key += i;
                } else {
                    K.push_back(std::stoi(key));
                    key = "";
                }
            }
            std::cin >> in;
        }
        if (K.empty())
            throw std::logic_error{"Key array should not be empty"};
        std::cout << "Enter Your encryption function mode, now only available 'table'" << std::endl;
        std::cin >> in;
        if (in == "table") {
            delete function_table;
            std::string cut;
            for (char &i: X)
                if ((std::find(cut.begin(), cut.end(), i) == cut.end()) || (cut.empty()))
                    cut += i;
            function_table = new char *[cut.length()];
            for (size_t i = 0; i < cut.length(); ++i) {
                function_table[i] = new char[K.size()];
                std::cin >> in;
                for (size_t j = 0; j < K.size(); ++j)
                    function_table[i][j] = in[j];
            }
        }
    } else {
        boost::filesystem::path p{in};
        if (p.empty())
            throw std::logic_error{"Given path leads to empty directory"};
        if (is_directory(p))
            throw std::logic_error{"Given path should lead to file, not to directory"};
        if (p.extension() == ".txt") {
            std::fstream is;
            is.open(in);
            if (is.peek() == std::ifstream::traits_type::eof())
                throw std::logic_error{"File must not be empty"};
            getline(is, in);
            while (in != "EOT") {
                X += in;
                getline(is, in);
            }
            if (X.empty())
                throw std::logic_error{"Text should not be empty"};
            getline(is, in);

            getline(is, in);
            while (in != "EOC") {
                Y += in;
                getline(is, in);
            }
            if (Y.empty())
                throw std::logic_error{"Cipher should not be empty"};
            getline(is, in);

            getline(is, in);
            while (in != "EOK") {
                std::string key;
                for (char i: in) {
                    if ((i != ' ') && (i != ',')) {
                        key += i;
                    } else {
                        K.push_back(std::stoi(key));
                        key = "";
                    }
                }
                getline(is, in);
            }
            if (K.empty())
                throw std::logic_error{"Key array should not be empty"};

            delete function_table;
            std::string cut;
            for (char &i: X)
                if ((std::find(cut.begin(), cut.end(), i) == cut.end()) || (cut.empty()))
                    cut += i;
            function_table = new char *[cut.length()];
            for (size_t i = 0; i < cut.length(); ++i) {
                function_table[i] = new char[K.size()];
                getline(is, in);
                for (size_t j = 0; j < K.size(); ++j)
                    function_table[i][j] = in[j];
            }
        }
        if (p.extension() == ".json") {
            json data;
            std::ifstream file{in};
            file >> data;
            if (data.empty())
                throw std::logic_error{"File must not be empty"};

            if (!data.at("text").is_string())
                throw std::logic_error{"Text must be string"};
            X = data.at("text").get<std::string>();
            if (X.empty())
                throw std::logic_error{"Text must not be empty"};

            if (!data.at("cipher").is_string())
                throw std::logic_error{"Cipher must be string"};
            Y = data.at("cipher").get<std::string>();
            if (Y.empty())
                throw std::logic_error{"Cipher must not be empty"};

            if (!data.at("key").is_array())
                throw std::logic_error{"Key must be array"};
            K = data.at("key").get<std::vector<int>>();
            if (K.empty())
                throw std::logic_error{"Key must not be empty"};

            if (!data.at("table").empty()) {
                if (!data.at("table").is_array())
                    throw std::logic_error{"Function table must be array of array of char"};
                delete function_table;
                std::string cut;
                for (char &i: X)
                    if ((std::find(cut.begin(), cut.end(), i) == cut.end()) || (cut.empty()))
                        cut += i;
                function_table = new char *[cut.length()];
                for (size_t i = 0; i < cut.length(); ++i) {
                    function_table[i] = new char[K.size()];
                    std::vector<char> line;
                    line = data.at("table").get<std::vector<char>>();
                    for (size_t j = 0; j < K.size(); ++j)
                        function_table[i][j] = line[j];
                }
            }
        }
    }
    // Проверка
    std::cout << X << std::endl;
    std::cout << Y << std::endl;
    for (int i: K)
        std::cout << i << ' ';
    std::cout << std::endl;
    for (size_t i = 0; i < powerX(X); ++i) {
        for (size_t j = 0; j < K.size(); ++j)
            std::cout << function_table[i][j] << ' ';
        std::cout << std::endl;
    }
    // /home/enenra/kursovaya/Kursovaya_1/data.txt
}
