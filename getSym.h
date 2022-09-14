#pragma once
void getSym(Token& token);
void writeFile(Token& token);
void writeFile(string message);
void writeError(int index, string message);
vector<string> split(string str, char ch);
vector<string> split(string str, string string1);
extern ofstream os;
extern ofstream error;
extern bool isHasError;