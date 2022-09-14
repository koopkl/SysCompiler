//
// Created by lsp on 2021/7/21.
//

#pragma once
#include "allheader.h"


typedef struct param{
    bool array;
    vector<string> paramDim; // 默认第一维度 长度无限大
} Param;

class TableRow {
public:

    string name;    // the name of (var or const or fun)
    string type;    // the type can be : "var" or "const" or "fun"
    string symbol;  // the symbol can be : "void", "int"
    string value;   // normal value (not array)
    bool isArr = false;
    bool isKonwn = false;
    vector<string> dimensions;
    map<string, string> arrValue; // key为坐标(0,0,0 or 1,1,5,8,6)，value为该坐标对应的值
    vector<Param> funParams; // 函数定义时的参数表类型

    // 使用map来进行多维数组初始化值的映射
    // 需要考虑是否需要一个单独的type来标识array
    // 首先 考虑多维数组的表现形式，显然需要使用vector来对每一维度进行处理
    // 那么对于不是数组类型 直接添加一个类似zzs的标识方便进行判断？
    // 抑或 对于不是数组类型的vector的维度第一个是zero，但是每次可能都需要判断
    TableRow();
    explicit TableRow(string name);
    TableRow(string name, string type, string symbol);
    TableRow(string name, string type, string symbol, string value);
    void clear();

};
