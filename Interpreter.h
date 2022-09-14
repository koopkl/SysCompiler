#pragma once
#include "allheader.h"
#include "PcodeInstr.h"

class Interpreter {
public:
    Interpreter();
    explicit Interpreter(vector<PcodeInstr>& instrIn);
    void runPcode(ofstream& pcoderesult);
private:
    // 两个栈顶始终比长度高一个
    vector<PcodeInstr> instr; // 全部指令
    string stack[10000]; // 数据栈空间
    string calStack[500]; // 运算栈空间
    int T = 0; // 当前栈顶指针
    int calT = 0; // 当前运算栈栈顶
    int B = 0; // 当前作用域的基地址，也即基址寄存器
    int P = 0; // 当前指令，从0开始
    int getSl(int B, int level);
    void caluate();
};