#pragma once
#ifndef COVERAGE_CHECK_H
#define COVERAGE_CHECK_H

#include <iostream>

constexpr unsigned int coverNum = 97;

const char *const coverOut[]{
        "AddExp: 未覆盖 +", // 0
        "AddExp: 未覆盖 -",  // 1
        "MulExp: 未覆盖 *",            // 2 *
        "MulExp: 未覆盖 /",            // 3 /
        "MulExp: 未覆盖 %",            //  4 %
        "RelExp: 未覆盖 <",                 //  5 <
        "RelExp: 未覆盖 <=",             // 6 <=
        "RelExp: 未覆盖 >",                 // 7 >
        "RelExp: 未覆盖 >=",             // 8 >=
        "EqExp: 未覆盖 !=",        // 9 !=
        "EqExp: 未覆盖 ==",           // 10 ==
        "LAndExp: 未覆盖 &&",           // 11 &&
        "LOrExp: 未覆盖 ||",           // 12 ||
        "标识符",          // 13
        "十进制数值常量", // 14
        "八进制数值常量",             // 15
        "十六进制数值常量",                   // 16
        "Decl: 未覆盖 ConstDecl", // 17
        "ConstDef: 未覆盖普通变量", // 18
        "ConstDef: 未覆盖一维数组", // 19
        "ConstDef: 未覆盖二维数组", // 20
        "Decl: 未覆盖 VarDecl",        // 21
        "VarDef: 未覆盖普通变量",  // 22
        "InitVal: 未覆盖常量表达式初值",  // 23
        "VarDef: 未覆盖一维数组",  // 24
        "InitVal: 未覆盖一维数组初值",  // 25
        "VarDef: 未覆盖二维数组", // 26
        "InitVal: 未覆盖二维数组初值", // 27
        "BType: 未覆盖类型 int ", // 28
        "ConstDecl: 未覆盖只声明一次的情况", // 29
        "ConstDecl: 未覆盖声明多次的情况", // 30
        "VarDecl: 未覆盖只声明一次的情况", // 31
        "VarDecl: 未覆盖声明多次的情况", // 32
        "CompUnit: 未覆盖 FuncDef", // 33
        "MainFuncDef: 未覆盖 main 函数", // 34
        "CompUnit: 未覆盖 Decl",// 35
        "FuncFParams: 未覆盖单个参数的情况",// 36
        "FuncFParams: 未覆盖多个参数的情况",// 37
        "FuncDef: 未覆盖无形参的情况",// 38
        "FuncFParam: 未覆盖普通变量",// 39
        "FuncFParam: 未覆盖一维数组",// 40
        "FuncFParam: 未覆盖二维数组",// 41
        "FuncType: 未覆盖 void 类型",// 42
        "FuncType: 未覆盖 int 类型",// 43
        "Block: 未覆盖没有 BlockItem 的情况",// 44
        "Block: 未覆盖有 BlockItem 的情况",// 45
        "BlockItem: 未覆盖 Decl",// 46
        "BlockItem: 未覆盖 Stmt",//  47
        "Stmt: 未覆盖 LVAL '=' Exp ';'",// 48
        "Stmt: 未覆盖 ';'",// 49
        "Stmt: 未覆盖 Exp ';'",// 50
        "Stmt: 未覆盖 Block",// 51
        "Stmt: 未覆盖 if 不含有 else 的情况",// 52
        "Stmt: 未覆盖 if 含有 else 的情况",// 53
        "Stmt: 未覆盖 break",// 54
        "Stmt: 未覆盖 continue",//  55
        "Stmt: 未覆盖 Return 语句不含有 Exp 的情况",// 56
        "Stmt: 未覆盖 Return 语句含有 Exp 的情况",// 57
        "Stmt: 未覆盖输入 getint",// 58
        "Stmt: 未覆盖输出 printf 不含 EXP 的情况",// 59
        "Stmt: 未覆盖输出 printf 含有 EXP 的情况",//  60
        "Exp: 未覆盖 AddExp",//  61
        "Cond: 未覆盖 LOrExp",// 62
        "LVal: 未覆盖普通变量",// 63
        "LVal: 未覆盖一维数组",//  64
        "LVal: 未覆盖二维数组",// 65
        "PrimaryExp: 未覆盖 LVal",// 66
        "PrimaryExp: 未覆盖 Number",// 67
        "PrimaryExp: 未覆盖 '('Exp')'",// 68
        "Number: 未覆盖 IntConst",// 69
        "UnaryExp: 未覆盖 PrimaryExp",// 70
        "UnaryExp: 未覆盖 Ident'('[FuncRParams]')'",// 71
        "UnaryExp: 未覆盖 UnaryOp UnaryExp",// 72
        "UnaryOp: 未覆盖 '!'",// 73
        "UnaryExp: 未覆盖 Ident'('')'",// 74
        "FuncRParams: 未覆盖单个参数的情况",// 75
        "FuncRParams: 未覆盖多个参数的情况",// 76
        "FuncRParams: 未覆盖数组传参",//  77
        "FuncRParams: 未覆盖部分数组传参",//  78
        "AddExp: 未覆盖 MulExp",// 79
        "MulExp: 未覆盖 UnaryExp",// 80
        "RelExp: 未覆盖 AddExp",// 81
        "EqExp: 未覆盖 RelExp",// 82
        "LAndExp: 未覆盖 EqExp",// 83
        "LOrExp: 未覆盖 LAndExp",// 84
        "ConstExp: 未覆盖 AddExp", // 85
        // 补充
        "ConstInitVal: 未覆盖常量表达式初值", // 86
        "ConstInitVal: 未覆盖一维数组初值", // 87
        "ConstInitVal: 未覆盖二维数组初值", // 88
        "FuncDef: 未覆盖有形参的情况", // 89
        "Stmt: 未覆盖 if 语句", // 90
        "Stmt: 未覆盖 while", // 91
        "Stmt: 未覆盖 Return", // 92
        "Stmt: 未覆盖输出 printf", // 93
        "UnaryExp: 未覆盖 Ident'('FuncRParams')'", // 94
        "UnaryOp: 未覆盖 '+'",
        "UnaryOp: 未覆盖 '-'",
};


class CoverageChecker {
    bool coverList[coverNum]{};
public:
    CoverageChecker() {
        for (bool &i : coverList) {
            i = false;
        }
        // that's means the following Four elements do not need to consider,
        // just because they can never show
        coverList[13] = true;
        coverList[14] = true;
        coverList[15] = true;
        coverList[16] = true;
    }

    double getScore() {
        int cnt = -1;
        for (bool i : coverList) {
            if (i) {
                cnt++;
            }
        }
        //std::cout << "\ncnt: " << cnt << std::endl;
        return (1.0 * cnt / (coverNum - 4));
    }

    void showResult(std::ofstream &out) {
        int cnt = 0;
        for (bool i : coverList) {
            if (!i) {
                cnt++;
            }
        }
        if ((1.0 * cnt / (coverNum - 4)) < 1) {
            for (int i = 0; i < coverNum; ++i) {
                if (!coverList[i]) {
                    out << coverOut[i] << endl;
                }
            }
        }

        if (cnt > 0) {
            //out << "\n\nscore: " << getScore() << std::endl;
        } else {
            out << "\n\n已覆盖所有语法成分" << std::endl;
        }
    }

    CoverageChecker operator+(const CoverageChecker &c) const {
        CoverageChecker tmp;
        for (int i = 0; i < coverNum; ++i) {
            if (coverList[i] || c.coverList[i]) {
                tmp.coverList[i] = true;
            }
        }
        return tmp;
    }

    void setPlus() {
        coverList[0] = true;
    }

    void setMinus() {
        coverList[1] = true;
    }

    void setMult() {
        coverList[2] = true;
    }

    void setDiv() {
        coverList[3] = true;
    }

    void setMod() {
        coverList[4] = true;
    }

    void setLss() {
        coverList[5] = true;
    }

    void setLeq() {
        coverList[6] = true;
    }

    void setGre() {
        coverList[7] = true;
    }

    void setGeq() {
        coverList[8] = true;
    }

    void setNeq() {
        coverList[9] = true;
    }

    void setEql() {
        coverList[10] = true;
    }

    void setLAnd() {
        coverList[11] = true;
    }

    void setLOr() {
        coverList[12] = true;
    }

    void setIdent() {
        coverList[13] = true;
    }

    void setDexCon() {
        coverList[14] = true;
    }

    void setOctCon() {
        coverList[15] = true;
    }

    void setHexCon() {
        coverList[16] = true;
    }

    void setConDecl() {
        coverList[17] = true;
    }

    void setNormalConDef() {
        coverList[18] = true;
    }

    void setArrOneDef() {
        coverList[19] = true;
    }

    void setArrTwoDef() {
        coverList[20] = true;
    }

    void setVarDecl() {
        coverList[21] = true;
    }

    void setNormalVarDefNoInit() {
        coverList[22] = true;
    }

    void setNormalVarDefInit() {
        coverList[23] = true;
    }

    void setArrOneVarDefNoInit() {
        coverList[24] = true;
    }

    void setArrOneVarDefInit() {
        coverList[25] = true;
    }

    void setArrTwoVarDefNoInit() {
        coverList[26] = true;
    }

    void setArrTwoVarDefInit() {
        coverList[27] = true;
    }

    void setIntBType() {
        coverList[28] = true;
    }

    void setOneConstDef() {
        coverList[29] = true;
    }

    void setManyConstDef() {
        coverList[30] = true;
    }

    void setOneVarDef() {
        coverList[31] = true;
    }

    void setManyVarDef() {
        coverList[32] = true;
    }

    void setFuncDef() {
        coverList[33] = true;
    }

    void setMainFuncDef() {
        coverList[34] = true;
    }

    void setDecl() {
        coverList[35] = true;
    }

    void setOneParaFunc() {
        coverList[36] = true;
    }

    void setManyParaFunc() {
        coverList[37] = true;
    }

    void setNoneParaFunc() {
        coverList[38] = true;
    }

    void setNormalParaFunc() {
        coverList[39] = true;
    }

    void setArrOneParaFunc() {
        coverList[40] = true;
    }

    void setArrTwoParaFunc() {
        coverList[41] = true;
    }

    void setVoidFunc() {
        coverList[42] = true;
    }

    void setIntFunc() {
        coverList[43] = true;
    }

    void setNoneItemBlock() {
        coverList[44] = true;
    }

    void setHasItemBlock() {
        coverList[45] = true;
    }

    void setDeclItem() {
        coverList[46] = true;
    }

    void setStmtItem() {
        coverList[47] = true;
    }

    void setAssignStmt() {
        coverList[48] = true;
    }

    void setNoneStmt() {
        coverList[49] = true;
    }

    void setExpStmt() {
        coverList[50] = true;
    }

    void setBlockStmt() {
        coverList[51] = true;
    }

    void setIfStmt() {
        coverList[52] = true;
    }

    void setIfElseStmt() {
        coverList[53] = true;
    }

    void setBreakStmt() {
        coverList[54] = true;
    }

    void setContinueStmt() {
        coverList[55] = true;
    }

    void setReturnNoExpStmt() {
        coverList[56] = true;
    }

    void setReturnHasExpStmt() {
        coverList[57] = true;
    }

    void setGetintStmt() {
        coverList[58] = true;
    }

    void setPrintfNoExpStmt() {
        coverList[59] = true;
    }

    void setPrintfHasExpStmt() {
        coverList[60] = true;
    }

    void setExp() {
        coverList[61] = true;
    }

    void setCondExp() {
        coverList[62] = true;
    }

    void setNormalLval() {
        coverList[63] = true;
    }

    void setArrOneLval() {
        coverList[64] = true;
    }

    void setArrTwoLval() {
        coverList[65] = true;
    }

    void setLvalPrimaryExp() {
        coverList[66] = true;
    }

    void setNumberPrimaryExp() {
        coverList[67] = true;
    }

    void setParentPrimaryExp() {
        coverList[68] = true;
    }

    void setIntConstExp() {
        coverList[69] = true;
    }

    void setPrimaryExpUnaryExp() {
        coverList[70] = true;
    }

    void setFunCallUnaryExp() {
        coverList[71] = true;
    }

    void setUnaryOpUnaryExp() {
        coverList[72] = true;
    }

    void setNotUnaryExp() {
        coverList[73] = true;
    }

    void setNoneParaFunCall() {
        coverList[74] = true;
    }

    void setOneParaFunCall() {
        coverList[75] = true;
    }

    void setManyParaFunCall() {
        coverList[76] = true;
    }

    void setArrRefAllFunCall() {
        coverList[77] = true;
    }

    void setArrRefPartFunCall() {
        coverList[78] = true;
    }

    void setOneAddExp() {
        coverList[79] = true;
    }


    void setOneMultExp() {
        coverList[80] = true;
    }


    void setOneRelExp() {
        coverList[81] = true;
    }


    void setOneEqlExp() {
        coverList[82] = true;
    }


    void setOneLAndExp() {
        coverList[83] = true;
    }


    void setOneLOrExp() {
        coverList[84] = true;
    }


    void setConstExp() {
        coverList[85] = true;
    }

    void setNormalConstInit() {
        coverList[86] = true;
    }

    void setArrOneConstInit() {
        coverList[87] = true;
    }

    void setArrTwoConstInit() {
        coverList[88] = true;
    }

    void setHasParaFunc() {
        coverList[89] = true;
    }

    void setHasIfStmt() {
        coverList[90] = true;
    }
    void setHasWhileStmt() {
        coverList[91] = true;
    }

    void setHasReturnStmt() {
        coverList[92] = true;
    }
    void setHasPrintfStmt() {
        coverList[93] = true;
    }

    void setOneParaFunCallUnaryExp() {
        coverList[94] = true;
    }

    void setPlusUnaryExp() {
        coverList[95] = true;
    }

    void setMinuUnaryExp() {
        coverList[96] = true;
    }



};

#endif // !COVERAGE_CHECK_H
