

#pragma once
#include "allheader.h"
#include "TableRow.h"

enum class QuadOp {

    number,     // 代表是一个常数感觉思源表达式生成代码的时候无法用到此四元
    constDef,   // 声明常量，存储到栈中
    varDef,
    add,        // +
    sub,        // -
    myNot,      // 对num2 取非 ！
    mul,        // *
    div,        // /
    mod,        // %
    lor,        // ||
    land,       // &&
    notEqual,   // !=
    equal,      // ==
    lss,        // <
    leq,        // <=
    gre,        // >
    geq,        // >=
    assign,     // 赋值

    getInt,

    arrRef,         // 需要进行数组的引用，最终栈中存储的是其地址或者是其他的，目前不确定
    arrLoad,        // 之间从栈中获取某个坐标对应的值存储到中间变量中。
    varLoad,        // 变量的引用，即需要从栈中去取出此值
    callBegin,      // 代表函数调用参数传递开始
    callTableRow,   // 代表调用函数时的传递的参数
    callEnd,        // 代表函数调用参数传递结束
    funBegin,
    funEnd,
    funParaDef,
    blockBegin,
    blockEnd,
    main,
    ifBegin,
    ifEnd,
    elseBegin,
    elseEnd,
    condBegin,
    condEnd,
    whileBegin,
    whileEnd,
    myBreak,
    myContinue,
    printfStr, // 在里面特判\n
    printfInt,
    myReturn, // num1的symbol is void 表明void，num1的symbol is int 表明有返回值需要进一步的赋值。
    jMain
};



class Quaternion {
public:
    QuadOp op;
    TableRow num1;
    TableRow num2;
    TableRow result;

    Quaternion(QuadOp op, TableRow num1, TableRow num2, TableRow res);
    Quaternion(QuadOp op, TableRow num1, TableRow num2);
    Quaternion(QuadOp op, TableRow num1);
    Quaternion(QuadOp op);


    Quaternion();
};


