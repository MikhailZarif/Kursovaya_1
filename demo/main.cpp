// Created by enenra on 19.11.2021.
#include "iostream"
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include "header.hpp"
#include <cstdlib>


using json = nlohmann::json;

int main() {
    std::cout << "Enter Your data" << std::endl;
    // Пользователь имеет три возможности ввести данные:
    //  - через консоль, тогда пишет строку "open_text"
    //  - из файла txt, тогда вводит полный путь к файлу
    //  - из файла json, тогда пишет полный путь к файлу
    std::string in, X, Y, alphabet;
    std::vector<int> K;
    std::vector<std::string> func;
    // X-открытый текст Y-шифртекст K-множество ключей func - вектор функций шифрования alphabet - кодировка языка
    // В конце каждой введённой новой переменной в консоли и в txt файле идёт строка с ключевым словом EOT, EOC, EOK (End Of Text/Cipher/Key)
    std::cin >> in;
    if (in == "open_text") {
        // Открытый текст
        std::cout << "Enter Your alphabet, don't forget EOT on the next line after it" << std::endl;
        std::cout << "If You are using ASCII language - type in 'ASCII', otherwise enum Your alphabet in direct order" << std::endl;
        getline(std::cin, in);
        while (in != "EOT") {
            X += in;
            getline(std::cin, in);
        }
        if (X.empty())
            throw std::logic_error{"Text should not be empty"};
        // Шифртекст
        std::cout << "Enter Your encrypted text, don't forget EOC on the next line after encrypted text" << std::endl;
        getline(std::cin, in);
        while (in != "EOC") {
            Y += in;
            getline(std::cin, in);
        }
        if (Y.empty())
            throw std::logic_error{"Cipher should not be empty"};
        // Ключи
        std::cout
                << "Enter Your key array, put ' ' or ',' between keys, don't forget EOK on the next line after last key"
                << std::endl;
        getline(std::cin, in);
        while (in != "EOK") {
            std::string key;
            for (size_t i = 0; i < in.size(); ++i) {
                if ((in[i] != ' ') && (in[i] != ',')) {
                    key += in[i];
                }
                if ((in[i] == ' ') || (in[i] == ',') || (i == in.size() - 1)) {
                    K.push_back(std::stoi(key));
                    key = "";
                }
            }
            getline(std::cin, in);
        }
        if (K.empty())
            throw std::logic_error{"Key array should not be empty"};
        // Функции
        std::cout
                << "Enter Your functions, one function - one line, don't forget EOF on the next line after last function"
                << std::endl;
        getline(std::cin, in);
        while (in != "EOF") {
            func.push_back(in);
            getline(std::cin, in);
        }
        if (func.empty())
            throw std::logic_error{"Function array should not be empty"};
    } else {
        boost::filesystem::path p{in};
        if (!exists(p))
            throw std::logic_error{"Given string is neither console entry command nor valid path"};
        if (p.empty())
            throw std::logic_error{"Given path leads to empty directory"};
        if (is_directory(p))
            throw std::logic_error{"Given path should lead to file, not to directory"};
        if (p.extension() == ".txt") {
            std::fstream is;
            is.open(in);
            if (is.peek() == std::ifstream::traits_type::eof())
                throw std::logic_error{"File must not be empty"};
            // Открытый текст
            getline(is, in);
            while (in != "EOT") {
                X += in;
                getline(is, in);
            }
            if (X.empty())
                throw std::logic_error{"Text should not be empty"};
            // Шифртекст
            getline(is, in);
            while (in != "EOC") {
                Y += in;
                getline(is, in);
            }
            if (Y.empty())
                throw std::logic_error{"Cipher should not be empty"};
            // Ключи
            getline(is, in);
            while (in != "EOK") {
                std::string key;
                for (size_t i = 0; i < in.size(); ++i) {
                    if ((in[i] != ' ') && (in[i] != ',')) {
                        key += in[i];
                    }
                    if ((in[i] == ' ') || (in[i] == ',') || (i == in.size() - 1)) {
                        K.push_back(std::stoi(key));
                        key = "";
                    }
                }
                getline(is, in);
            }
            if (K.empty())
                throw std::logic_error{"Key array should not be empty"};
            // Функции
            getline(is, in);
            while (in != "EOF") {
                func.push_back(in);
                getline(is, in);
            }
            if (func.empty())
                throw std::logic_error{"Function array should not be empty"};
        }
        if (p.extension() == ".json") {
            json data;
            std::ifstream file{in};
            file >> data;
            if (data.empty())
                throw std::logic_error{"File must not be empty"};

            if (data.at("text").empty())
                throw std::logic_error{"Text must not be empty"};
            if (!data.at("text").is_string())
                throw std::logic_error{"Text must be string"};
            X = data.at("text").get<std::string>();

            if (data.at("cipher").empty())
                throw std::logic_error{"Cipher must not be empty"};
            if (!data.at("cipher").is_string())
                throw std::logic_error{"Cipher must be string"};
            Y = data.at("cipher").get<std::string>();

            if (data.at("key").empty())
                throw std::logic_error{"Key must not be empty"};
            if (!data.at("key").is_array())
                throw std::logic_error{"Key must be array"};
            K = data.at("key").get<std::vector<int>>();

            if (data.at("functions").empty())
                throw std::logic_error{"Functions array must not be empty"};
            if (!data.at("functions").is_array())
                throw std::logic_error{"Function table must be array of array of char"};
            func = data.at("functions").get<std::vector<std::string>>();
        }
    }
    // /home/enenra/kursovaya/Kursovaya_1/data.txt
    cryptalgorithm entry(X, Y, K, func);
    std::ofstream out("/home/enenra/kursovaya/Kursovaya_1/Report.txt", std::ios::app);
    out << "Report on Your cryptoalgorithm" << '\n';
    out << "Here are the checks that Your algorithm has passed:" << '\n';
    std::cout<<"Now Your cryptoalgorithm is ready to be analyzed. Enter EXIT if you want to exit, while EXIT is not typed? algorithm continues working."<<'\n';
    std::cout<<"Enter absolute_stability or brute_force or differential_attack or linear_attack commands for the program."<<'\n';
    std::cout<<"Your report will be saved at {CURRENT_SOURCE_DIR}/Report.txt Enjoy!"<<'\n';
    std::getline(std::cin, in);
    while (in!="EXIT")
    {
        if (in=="absolute_stability")
            entry.absolute_stability(out);
        if (in=="brute_force")
            entry.brute_force(out);
        if (in=="differential_attack")
            entry.differential_attack(out);
        if (in=="linear_attack")
            entry.linear_attack(out);
        std::getline(std::cin, in);
    }
    out.close();
}
