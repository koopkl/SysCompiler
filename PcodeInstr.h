#pragma once
#include "allheader.h"
enum class PcodeOp {
    LIT,
    OPR,
    LOD,
    STO,
    CAL, // cal B  addr , B代表新的基础寄存器的地址
    INT,
    JMP,
    JTP, // 为真则跳转
    JPC, // 为假则跳转
    LODS, // LOD  // 将绝对地址为栈顶值，值取出放到栈顶打上
    STOS,
    LA,  // 取当前基地址寄存器的值放到数据栈顶
    PRT,  // PRT 0 str 输出字符串
    BLO,  // 操作与PL/0的CAL方法一致，开辟新的地址空间解释器完成,此处添加了新的作用区域！！
    BLE,
    JPCAND, // 代表短路逻辑中的&&跳转与jpc的区别为，此判断不会对数据栈的数据产生影响
};
/**
 * OPR 0 0	过程调用结束后,返回调用点并退栈
OPR 0 1	栈顶元素取反
OPR 0 2	次栈顶与栈顶相加，退两个栈元素，结果值进栈
OPR 0 3	次栈顶减去栈顶，退两个栈元素，结果值进栈
OPR 0 4	次栈顶乘以栈顶，退两个栈元素，结果值进栈
OPR 0 5	次栈顶除以栈顶，退两个栈元素，结果值进栈
OPR 0 6	栈顶元素的奇偶判断，结果值在栈顶 ->  次栈顶对栈顶取摸，退两个栈元素，结果值进栈。
OPR 0 7
OPR 0 8	次栈顶与栈顶是否相等，退两个栈元素，结果值进栈
OPR 0 9	次栈顶与栈顶是否不等，退两个栈元素，结果值进栈
OPR 0 10	次栈顶是否小于栈顶，退两个栈元素，结果值进栈
OPR 0 11	次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
OPR 0 12	次栈顶是否大于栈顶，退两个栈元素，结果值进栈
OPR 0 13	次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
OPR 0 14	栈顶值输出至屏幕
OPR 0 15	屏幕输出换行
OPR 0 16	从命令行读入一个输入置于栈顶
 */

class PcodeInstr {
public:
    PcodeInstr();
    PcodeInstr(PcodeOp op, string level, string address);
    string toString();
    PcodeOp getOp();
    string getAddress();
    string getLevel();
    void setAddress(string addr);

private:
    PcodeOp op;
    string level;
    string address;
};

