//
// Created by berci on 2021. 07. 31..
//

#include <utils.h>
#include <Scanner.h>
#include <TokenType.h>

#include <memory>

std::vector<Token> Scanner::scan_tokens() {
    while (not this->is_at_end()) {
        this->start_ = this->current_;
        scan_token();
    }

    tokens_.emplace_back(TokenType::END, "", std::make_shared<LoxValue>(), this->line_);
    return this->tokens_;
}

bool Scanner::is_at_end() {
    return this->current_ >= this->source_.length();
}

void Scanner::scan_token() {
    auto c = advance();
    switch (c) {
        case '(': put_token(TokenType::LEFT_PAREN); break;
        case ')': put_token(TokenType::RIGHT_PAREN); break;
        case '{': put_token(TokenType::LEFT_BRACE); break;
        case '}': put_token(TokenType::RIGHT_BRACE); break;
        case ',': put_token(TokenType::COMMA); break;
        case '.': put_token(TokenType::DOT); break;
        case '-': put_token(TokenType::MINUS); break;
        case '+': put_token(TokenType::PLUS); break;
        case ';': put_token(TokenType::SEMICOLON); break;
        case '*': put_token(TokenType::STAR); break;
        case '!':
            put_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            put_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            put_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            put_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' and !is_at_end()) {
                    advance();
                }
            } else {
                put_token(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            // ignore whitespace
            break;
        case '\n':
            this->line_ = this->line_ + 1;
            break;
        case '"': string(); break;
        default:
            if (is_digit(c)) {
                number();
            } else if (is_alpha(c)) {

            } else {
                error(this->line_, "Unexpected character");
            }
            break;
    }
}

char Scanner::advance() {
    auto next = this->source_.at(this->current_);
    this->current_ = this->current_ + 1;
    return next;
}

void Scanner::put_token(TokenType type) {
    std::string text = source_.substr(this->start_, this->current_);
    this->tokens_.emplace_back(type, text, std::make_shared<LoxValue>(), this->line_);
}

bool Scanner::match(char c) {
    if (this->is_at_end()) {
        return false;
    }
    auto current = this->source_.at(this->current_);
    if (current != c) {
        return false;
    }

    this->current_ = this->current_ + 1;

    return true;
}

char Scanner::peek() {
    if (is_at_end()) {
        return '\0';
    }
    return this->source_.at(this->current_);
}

void Scanner::string() {
    while (peek() != '"' and not is_at_end()) {
        if (peek() == '\n') {
            this->line_ = this->line_ + 1;
        }
        advance();
    }

    if (is_at_end()) {
        error(this->line_, "unterminated string literal");
        return;
    }

    advance();
    std::string value = this->source_.substr(this->start_ + 1, this->current_ - 1);
    auto loxString = std::make_shared<LoxString>(value);
    this->add_token(TokenType::STRING, loxString);
}

void Scanner::add_token(TokenType type, std::shared_ptr<LoxValue> value) {
    std::string text = source_.substr(this->start_, this->current_);
    this->tokens_.emplace_back(type, text, std::move(value), this->line_);
}

bool Scanner::is_digit(char c) {
    return '0' <= c && c <= '9';
}

void Scanner::number() {
    while (is_digit(peek())) {
        advance();
    }

    if (peek() == '.' and is_digit(peek_next())) {
        advance();
    }

    while (is_digit(peek())) {
        advance();
    }

    auto literal = this->source_.substr(this->start_, this->current_);
    auto number = std::stod(literal);
    auto loxNumber = std::make_shared<LoxNumber>(number);
    this->add_token(TokenType::NUMBER, loxNumber);
}

char Scanner::peek_next() {
    if (this->current_ + 1 < this->source_.size()) {
        return this->source_.at(this->current_ + 1);
    }
    return '\0';
}

bool Scanner::is_alpha(char c) {
    auto isLowerCase = 'a' <= c && c <= 'z';
    auto isUpperCase = 'A' <= c && c <= 'Z';
    auto isUnderScore = c == '_';
    return isLowerCase || isUpperCase || isUnderScore;
}

bool Scanner::is_alphanumeric(char c) {
    return is_alpha(c) || is_digit(c);
}

void Scanner::identifier() {
    while (is_alphanumeric(peek())) {
        advance();
    }

    auto literal = this->source_.substr(this->start_, this->current_);

    TokenType type = TokenType::IDENTIFIER;
    if (this->keywords.count(literal) > 0) {
        type = this->keywords.at(literal);
    }

    put_token(type);
}

