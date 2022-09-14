#include <iostream>
#include <algorithm>
#include "Token.h"

#define AC_GO token.word+=ch;++token.index;
#define SET_CH ch=token.file.at(token.index);

using namespace std;
std::ofstream os("output.txt");
std::ofstream error("error.txt");
bool isHasError;

void writeFile(Token &token) {
    if (!token.symbol.empty()) {
        os << token.symbol << " " << token.word << endl;
    }
}

void writeFile(string message) {
    os << message << endl;
}

void writeError(int line, string message) {
    isHasError = true;
    error << to_string(line) + " " + message << endl;
}

bool isKeyWord(string word) {
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    vector<string> keyWords = {"main", "const", "int", "break", "continue", "if",
                               "else", "while", "for", "getint", "printf", "return", "void"};
    return find(keyWords.begin(), keyWords.end(), word) != keyWords.end();
}

bool isHex(char ch) {
    return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

bool isOct(char ch) {
    return (ch >= '0' && ch <= '7');
}

vector<string> split(string str, char ch) {
    vector<string> result;
    string temp;
    for (auto c : str) {
        if (c == ch) {
            result.push_back(temp);
            temp = "";
            continue;
        }
        temp += c;
    }
    return result;
}

vector<string> split(string str, string string1) {
    vector<string> res;
    if (str.size() < string1.size()) {
        res.push_back(str);
    } else if (str.size() == string1.size()) {
        if (str == string1) {
            res.emplace_back("");
            res.emplace_back("");
        } else {
            res.push_back(str);
        }
    } else {
        std::regex ws(string1);
        vector<string> res(sregex_token_iterator(str.begin(), str.end(), ws, -1), sregex_token_iterator());
        return res;
    }
    return res;

}

void getSym(Token &token) {
    unsigned int length = token.length;
    token.clearToken();
    token.symbol.clear();
    char ch = '\0';
    if (token.index < length) {
        SET_CH
    }
    while (isspace(ch) && token.index < length) {
        if (ch == '\n') {
            token.lineNum++;
        }
        ++token.index;
        if (token.index < length) {
            SET_CH
        }
    }
    while (ch == '/') {
        if (token.index + 1 < length) {
            if (token.file.at(token.index + 1) == '/') {
                token.index += 2;
                while (token.index < length && token.file.at(token.index) != '\n') {
                    token.index++;
                }
                if (token.file.at(token.index) == '\n') {
                    token.lineNum++;
                }
                token.index++;
            } else if (token.file.at(token.index + 1) == '*') {
                token.index += 2;
                while (token.index + 1 < length) {
                    SET_CH
                    if (ch == '\n') {
                        token.lineNum++;
                    }
                    if ((token.file.at(token.index) == '*' && token.file.at(token.index + 1) == '/')) {
                        break;
                    }
                    token.index++;
                }
                token.index += 2;
            } else {
                break;
            }
        }
        if (token.index < length) {
            SET_CH
        } else {
            ch = '\0';
        }
        while (isspace(ch) && token.index < length) {
            if (ch == '\n') {
                token.lineNum++;
            }
            ++token.index;
            if (token.index < length) {
                SET_CH
            }
        }
    }

    if (isalpha(ch) || ch == '_') {
        while ((isalpha(ch) || isdigit(ch) || ch == '_') && token.index < length) {
            AC_GO
            if (token.index < length) {
                SET_CH
            }
        }
        if (isKeyWord(token.word)) {
            string str = token.word;
            transform(str.begin(), str.end(), str.begin(), ::toupper);
            /*transform(token.word.begin(), token.word.end(), token.word.begin(), ::tolower);*/

            token.symbol = str + "TK";
        } else {
            token.symbol = "IDENFR";
        }
    } else if (isdigit(ch)) {
        if (ch == '0') {
            AC_GO
            if (token.index < length) {
                SET_CH
                if (ch == 'x' || ch == 'X') {
                    AC_GO
                    SET_CH
                    while (isHex(ch)) {
                        AC_GO
                        if (token.index < length) {
                            SET_CH
                        }
                    }
                    token.symbol = "INTCON";
                    //token.symbol = "HEX_CON";
                } else {
                    while (isOct(ch)) {
                        AC_GO
                        if (token.index < length) {
                            SET_CH
                        }
                    }
                    token.symbol = "INTCON";
                    //token.symbol = "OCT_CON";
                }
            }
        } else {
            while (token.index < length && isdigit(ch)) {
                AC_GO
                if (token.index < length) {
                    SET_CH
                }
            }
            //token.symbol = "DEC_CON";
            token.symbol = "INTCON";
        }
    } else if (ch == '\"') {
        AC_GO
        //++token.index;
        if (token.index < length) {
            SET_CH
            while (ch != '\"' && token.index < length) {
                AC_GO
                if (token.index < length) {
                    SET_CH
                }
            }
            token.symbol = "STRCON";
            AC_GO
        }
    } else if (ch == '+') {
        AC_GO
        token.symbol = "PLUS";
    } else if (ch == '-') {
        AC_GO
        token.symbol = "MINU";
    } else if (ch == '*') {
        AC_GO
        token.symbol = "MULT";
    } else if (ch == '/') {
        AC_GO
        token.symbol = "DIV";
    } else if (ch == '%') {
        AC_GO
        token.symbol = "MOD";
    } else if (ch == '<') {
        AC_GO
        if (token.index < length) {
            SET_CH
            if (ch == '=') {
                AC_GO
                token.symbol = "LEQ";
            } else {
                token.symbol = "LSS";
            }
        }
    } else if (ch == '>') {
        AC_GO
        if (token.index < length) {
            SET_CH
            if (ch == '=') {
                AC_GO
                token.symbol = "GEQ";
            } else {
                token.symbol = "GRE";
            }
        }
    } else if (ch == '=') {
        AC_GO
        if (token.index < length) {
            SET_CH
            if (ch == '=') {
                AC_GO
                token.symbol = "EQL";
            } else {
                token.symbol = "ASSIGN";
            }
        }
    } else if (ch == ';') {
        AC_GO
        token.symbol = "SEMICN";
    } else if (ch == ',') {
        AC_GO
        token.symbol = "COMMA";
    } else if (ch == '(') {
        AC_GO
        token.symbol = "LPARENT";
    } else if (ch == ')') {
        AC_GO
        token.symbol = "RPARENT";
    } else if (ch == '[') {
        AC_GO
        token.symbol = "LBRACK";
    } else if (ch == ']') {
        AC_GO
        token.symbol = "RBRACK";
    } else if (ch == '{') {
        AC_GO
        token.symbol = "LBRACE";
    } else if (ch == '}') {
        AC_GO
        token.symbol = "RBRACE";
    } else if (ch == '!') {
        AC_GO
        if (token.index < length) {
            SET_CH
            if (ch == '=') {
                AC_GO
                token.symbol = "NEQ";
            } else {
                token.symbol = "NOT";
            }
        }
    } else if (ch == '&') {
        AC_GO
        if (token.index < length) {
            SET_CH
            if (ch == '&') {
                AC_GO
                token.symbol = "AND";
            }
        }
    } else if (ch == '|') {
        AC_GO
        if (token.index < length) {
            SET_CH
            if (ch == '|') {
                AC_GO
                token.symbol = "OR";
            }
        }
    }
}

