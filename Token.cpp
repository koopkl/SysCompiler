#include "Token.h"
Token::Token(string fileIn) : file(std::move(fileIn)) {
    length = file.length();
}
void Token::clearToken() {
    word.clear();
}

