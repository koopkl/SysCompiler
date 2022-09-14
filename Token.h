#pragma once

#include "allheader.h"
#include <fstream>
#include <sstream>

enum class TokenType {
    empty, MAIN_TK, CONST_TK, INT_TK, BREAK_TK, CONTINUE_TK, IF_TK,
    ELSE_TK, WHILE_TK, FOR_TK, GETINT_TK, PRINTF_TK, RETURN_TK,
    IDENT,HEX_CON,OCT_CON,DEC_CON, STRCON, PLUS, MINU, MULTI, DIV, MOD, LEQ, LSS, GEQ, GRE, EQL, SEMICOLON,
    COMMA, L_PARENT, R_PARENT, L_BRACK, R_BRACK, L_BRACE, NEQ, AND, NOT, OR

};

class Token {
public:
    string file;    // 输入源码
    string word;    // 对应单词
    //TokenType symbol;  // 对应的token的类型
    string symbol;
    int length;     // 单词长度
    int index = 0;  // token 当前的index
    int lineNum = 1;

    Token(string fileIn);

    void clearToken();
};

