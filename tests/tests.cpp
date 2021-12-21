// Created by enenra on 19.11.2021.

#include "header.hpp"
#include <string>
#include <gtest/gtest.h>

TEST(Boolean, construct) {
    // Задаём одну и ту же функцию разными строками
    std::string init1 = "f(x)=10011101", init2 = "f1=10011101", init3 = "f(x1, x2, x3)=10011101";
    boolean first(init1), second(init2), third(init3);
    EXPECT_EQ(first[0], 1);
    EXPECT_EQ(first[1], 0);
    EXPECT_EQ(first[2], 0);
    EXPECT_EQ(first[3], 1);
    EXPECT_EQ(first[4], 1);
    EXPECT_EQ(first[5], 1);
    EXPECT_EQ(first[6], 0);
    EXPECT_EQ(first[7], 1);
    EXPECT_EQ(first, second);
    EXPECT_EQ(first, third);

    // Зададим ту же и другую функции через число
    boolean another(157, 8), another_one(157, 9);
    EXPECT_EQ(another_one[0], 0);
    EXPECT_EQ(another, first);
    EXPECT_NE(another, another_one);
}

TEST(Boolean, functions) {
    boolean example;
    EXPECT_TRUE(example.empty());
    example.push_back(1);
    EXPECT_EQ(example[0], 1);
    EXPECT_FALSE(example.empty());

    boolean example1(154, 8);
    EXPECT_EQ(example1.degree(), 3);
    EXPECT_EQ(example1.to_number(), 154);

    std::string init = "f(x1, x2, x3)=11010001";
    boolean example2(init);
    boolean example3(209, 8);
    EXPECT_FALSE(example2.empty());
    EXPECT_EQ(example2.to_number(), 209);
    EXPECT_EQ(example2.degree(), 3);
    EXPECT_EQ(example2, example3);
    example2.push_back(1);
    EXPECT_EQ(example2[8], 1);
    EXPECT_NE(example1, example2);
}

TEST(Boolean, operators) {
    std::string init1 = "11011001";
    std::string init2 = "10101100";
    // Ожидаемый результат ксора - 01110101
    std::string init3 = "01110101";
    boolean one(init1), two(init2), three(init3);
    one += two;
    EXPECT_EQ(one, three);
    EXPECT_NE(two, three);
    // Ожидаемый результат ксора - 00000000
    one += three;
    for (size_t i = 0; i < 8; ++i)
        EXPECT_EQ(one[i], 0);

    boolean four(init1), five(init2);
    // Ожидаемый результат перемножения - 10001000
    std::string init = "10001000";
    boolean six(init);
    four *= five;
    EXPECT_EQ(four, six);
    EXPECT_NE(five, six);
    four *= six;
    // Ожидаемый результат перемножения - 10001000
    EXPECT_EQ(four, six);
    EXPECT_NE(four, five);
}

TEST(Additional, functions) {
    std::string text = "AAAacfhgowbv";
    EXPECT_EQ(power(text), 10);
    std::string other_text = "asdfghjkla";
    EXPECT_EQ(power(other_text), 9);

    size_t val1 = 196, val2 = 19, val3 = 1, val4 = 0;
    EXPECT_EQ(number_degree(val1), 3);
    EXPECT_EQ(number_degree(val2), 2);
    EXPECT_EQ(number_degree(val3), 1);
    EXPECT_EQ(number_degree(val3), number_degree(val4));

    int val_1 = 19600, val_2 = 1953, val_3 = 10, val_4 = 0;
    EXPECT_EQ(number_degree(val_1), 5);
    EXPECT_EQ(number_degree(val_2), 4);
    EXPECT_EQ(number_degree(val_3), 2);
    EXPECT_NE(number_degree(val_3), number_degree(val_4));
}
