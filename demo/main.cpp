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
    std::string in, X, Y;
    std::vector<int> K;
    std::vector<std::string> func;
    // X-открытый текст Y-шифртекст K-множество ключей func - вектор функций шифрования
    // В конце каждой введённой новой переменной в консоли и в txt файле идёт строка с ключевым словом EOT, EOC, EOK (End Of Text/Cipher/Key)
    std::cin >> in;
    if (in == "open_text") {
        // Открытый текст
        std::cout << "Enter Your text, don't forget EOT on the next line after text" << std::endl;
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
    std::ofstream out("/home/enenra/kursovaya/Kursovaya_1/Report.txt", std::ios::app);
    out << "Report on Your cryptoalgorythm" << '\n';
    out << "Here are the checks that Your algorithm has passed" << '\n';
    out << " - Absolute stability: ";
    if (absolute_stability(X, K))
        out << "true" << '\n';
    else
        out << "false" << '\n';
    out.close();
}
