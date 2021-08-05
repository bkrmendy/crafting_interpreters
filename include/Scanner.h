//
// Created by berci on 2021. 07. 31..
//

#pragma once

#include <Token.h>

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

class Scanner {
    std::string source_;
    std::vector<Token> tokens_;

    int start_ = 0;
    int current_ = 0;
    int line_ = 0;

    std::unordered_map<std::string, TokenType> keywords = {
        {"and", TokenType::AND},
        {"or", TokenType::OR},

        {"class", TokenType::CLASS},
        {"this", TokenType::THIS},
        {"super", TokenType::SUPER},

        {"if", TokenType::IF},
        {"else", TokenType::ELSE},

        {"false", TokenType::FALSE},
        {"true", TokenType::TRUE},

        {"for", TokenType::FOR},
        {"while", TokenType::WHILE},

        {"var", TokenType::VAR},

        {"fun", TokenType::FUN},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},

        {"nil", TokenType::NIL},
    };

public:
    Scanner(std::string source) : source_{std::move(source)}, tokens_{} {}

    char advance();

    bool match(char c);

    char peek();

    char peek_next();

    void put_token(TokenType type);

    void add_token(TokenType type, std::shared_ptr<LoxValue> value);

    bool is_at_end();

    void scan_token();

    void string();

    void number();

    void identifier();


    bool is_digit(char c);

    bool is_alpha(char c);

    bool is_alphanumeric(char c);

    std::vector<Token> scan_tokens();
};