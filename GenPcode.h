

#pragma once

#include "allheader.h"
#include "Quaternion.h"
#include "PcodeInstr.h"
#include "parsing.h"
#include "getSym.h"

typedef struct addr {
    bool isRef = false;
    string address; // 有两种含义：当不是数组时，表示普通意义上的地址，当是数组时表明该数组引用的起始位置的地址。
    // 以下是是数组引用时，需要记录的信息。
    vector<string> dims;
} Addr;


// 需要增加一条指令来满足能够通过栈顶的数值来取值，因为有数组，具体偏移通过计算得出，无法已知。

// LOD l
class GenPcode {
public:
    void genPcode();

private:
    /**
     * 变量的定义和常量的定义存在一些问题：
     * 变量的定义需要知道变量在哪一个分层，需要保存层次差。
     *
     *
     */

    vector <Quaternion> quaterions;
    vector <PcodeInstr> instr; // 指令的集合
    vector<map<string, Addr>> varMap; // vector的下标代表层级，map为变量名字和相对地址偏移量。
    vector<int> addressStack; // 地址空间的栈顶永远是空的。
    // 比如某个函数的开始地址，比如if的开始和结束地址，while的开始和结束地址，else的开始和结束地址
    // *****有这个可能需要对if、else、以及while等语句进行标记**********
    map <string, string> nameToAddr;
    int deep = 0;
public:
    explicit GenPcode(vector <Quaternion>& quaterions);
    GenPcode();
    vector<PcodeInstr>& getInstr();
private:
    int findVar(const string& name);
    int getAbAddr(int level);

    void genConstDef(Quaternion quater);

    void genVarDef(Quaternion quater);

    void genCalculate(const Quaternion& quater, const string& number);

    void genLor(Quaternion quater);

    void genLand(Quaternion quater);

    void genAssign(const Quaternion& quater);

    void genGetInt(const Quaternion& quater);

    void genArrRef(const Quaternion& quater);

    void genArrLoad(const Quaternion& quater);

    void genVarLoad(Quaternion quater);

    void genCallBegin(Quaternion quater);

    void genCallEnd(const Quaternion& quater);

    void genCallTableRow(const Quaternion& quater);

    void genFunBegin(const Quaternion& quater);

    void genFunEnd(Quaternion quater);

    void genFunParaDef(const Quaternion& quater);

    void genMain(Quaternion quater);

    void genIfBegin(Quaternion quater);

    void genIfEnd(const Quaternion& quater);

    void genElseBegin(Quaternion quater);

    void genElseEnd(const Quaternion& quater);

    void genCondBegin(Quaternion quater);

    void genCondEnd(const Quaternion& quater);

    void genWhileBegin(const Quaternion& quater);

    void genWhileEnd(const Quaternion& quater);

    void genMyBreak(const Quaternion& quater);

    void genMyContinue(const Quaternion& quater);

    void genPrintfStr(const Quaternion& quater);

    void genPrintfInt(const Quaternion& quater);

    void genMyReturn(const Quaternion& quater);

    void genBlockBegin(const Quaternion& quater);

    void genBlockEnd();

};


