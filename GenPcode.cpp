#include "GenPcode.h"


#define ADD_INSTR instr.emplace_back(PcodeOp::OPR, "0", "2");
#define MUL_INSTR instr.emplace_back(PcodeOp::OPR, "0", "4");
#define NEW_INSTR(a) instr.emplace_back(PcodeOp::INT, "0", a);


GenPcode::GenPcode(vector<Quaternion> &quaterions111) {
    quaterions = quaterions111;
    addressStack.push_back(3);
    map<string, Addr> temp;
    varMap.push_back(temp);
}

vector<PcodeInstr> &GenPcode::getInstr() {
    return this->instr;
}

GenPcode::GenPcode() {

}

// 得到某个变量所在的第几个区，从0开始
int GenPcode::findVar(const string &name) {
    for (int i = varMap.size() - 1; i >= 0; --i) {
        if (varMap.at(i).find(name) != varMap.at(i).end()) {
            return i;
        }
    }
    return -1;
}


void GenPcode::genPcode() {
    for (auto quater : quaterions) {
        QuadOp op = quater.op;
        switch (op) {
            case QuadOp::constDef:
                genConstDef(quater);
                break;
            case QuadOp::varDef:
                genVarDef(quater);
                break;
            case QuadOp::myNot:
                genCalculate(quater, "1");
                break;
            case QuadOp::add:
                genCalculate(quater, "2");
                break;
            case QuadOp::sub:
                genCalculate(quater, "3");
                break;
            case QuadOp::mul:
                genCalculate(quater, "4");
                break;
            case QuadOp::div:
                genCalculate(quater, "5");
                break;
            case QuadOp::mod:
                genCalculate(quater, "6");
                break;
            case QuadOp::notEqual:
                genCalculate(quater, "9");
                break;
            case QuadOp::equal:
                genCalculate(quater, "8");
                break;
            case QuadOp::lss:
                genCalculate(quater, "10");
                break;
            case QuadOp::leq:
                genCalculate(quater, "13");
                break;
            case QuadOp::gre:
                genCalculate(quater, "12");
                break;
            case QuadOp::geq:
                genCalculate(quater, "11");
                break;
            case QuadOp::lor:
                genLor(quater);
                break;
            case QuadOp::land:
                genLand(quater);
                break;
            case QuadOp::condBegin:
                genCondBegin(quater);
                break;
            case QuadOp::condEnd:
                genCondEnd(quater);
                break;
            case QuadOp::assign:
                genAssign(quater);
                break;
            case QuadOp::getInt:
                genGetInt(quater);
                break;
            case QuadOp::arrRef:
                genArrRef(quater);
                break;
            case QuadOp::arrLoad:
                genArrLoad(quater);
                break;
            case QuadOp::varLoad:
                genVarLoad(quater);
                break;
            case QuadOp::callBegin:
                genCallBegin(quater);
                break;
            case QuadOp::callEnd:
                genCallEnd(quater);
                break;
            case QuadOp::callTableRow:
                genCallTableRow(quater);
                break;
            case QuadOp::funBegin:
                genFunBegin(quater);
                break;
            case QuadOp::funEnd:
                genFunEnd(quater);
                break;
            case QuadOp::funParaDef:
                genFunParaDef(quater);
                break;
            case QuadOp::main:
                genMain(quater);
                break;
            case QuadOp::ifBegin:
                genIfBegin(quater);
                break;
            case QuadOp::ifEnd:
                genIfEnd(quater);
                break;
            case QuadOp::elseBegin:
                genElseBegin(quater);
                break;
            case QuadOp::elseEnd:
                genElseEnd(quater);
                break;
            case QuadOp::whileBegin:
                genWhileBegin(quater);
                break;
            case QuadOp::whileEnd:
                genWhileEnd(quater);
                break;
            case QuadOp::myBreak:
                genMyBreak(quater);
                break;
            case QuadOp::myContinue:
                genMyContinue(quater);
                break;
            case QuadOp::printfStr:
                genPrintfStr(quater);
                break;
            case QuadOp::printfInt:
                genPrintfInt(quater);
                break;
            case QuadOp::myReturn:
                genMyReturn(quater);
                break;
            case QuadOp::number:
                break;
            case QuadOp::blockBegin:
                genBlockBegin(quater);
                break;
            case QuadOp::blockEnd:
                genBlockEnd();
                break;
            case QuadOp::jMain:
                instr.emplace_back(PcodeOp::JMP, "", "$main");
                break;
        }
    }
    std::ofstream pcode("pcode.txt");
    for (auto &temp : instr) {
        PcodeOp op = temp.getOp();
        if (op == PcodeOp::JMP || op == PcodeOp::JTP || op == PcodeOp::JPC || op == PcodeOp::CAL ||
            op == PcodeOp::JPCAND) {
            if (temp.getAddress().at(0) == '$') {
                string sb = nameToAddr.at(temp.getAddress());
                temp.setAddress(sb);
            }
        }
    }
    for (auto temp : instr) {
        pcode << temp.toString() << endl;
    }
}

void GenPcode::genConstDef(Quaternion quater) {
    TableRow *num1 = &quater.num1;
    Addr addr;
    addr.isRef = false;
    if (!num1->isArr) {
        // 普通常量的声明。
        instr.emplace_back(PcodeOp::INT, "0", "1");
        instr.emplace_back(PcodeOp::LIT, "0", num1->value);
        instr.emplace_back(PcodeOp::STO, "0", to_string(addressStack.back()));
        addr.address = to_string(addressStack.back()++);
    } else {
        int length = 1;
        for (const auto &i : num1->dimensions) {
            length *= stoi(i);
        }
        int beginAddress = addressStack.back();
        addr.address = to_string(beginAddress);
        addressStack.back() += length;
        instr.emplace_back(PcodeOp::INT, "0", to_string(length));
        for (auto iter = num1->arrValue.begin(); iter != num1->arrValue.end(); iter++) {
            vector<string> splitDim = split(iter->first, ",");
            vector<int> dim;
            std::transform(splitDim.begin(), splitDim.end(), back_inserter(dim), [](string i) { return stoi(i); });
            int corr = Parsing::getCount(num1->dimensions, dim);
            instr.emplace_back(PcodeOp::LIT, "0", iter->second);
            instr.emplace_back(PcodeOp::STO, "0", to_string(corr + beginAddress - 1));
        }
    }
    varMap.back().insert(make_pair(num1->name, addr));
}


void GenPcode::genVarDef(Quaternion quater) {
    TableRow *num1 = &quater.num1;
    Addr addr;
    addr.isRef = false;
    if (!num1->isArr) {
        instr.emplace_back(PcodeOp::INT, "0", "1");
        if (!num1->value.empty()) {
            // 一种可能是value是$t开头，
            // 一种可能是num1.isKnown is true or false, but I can't promise this will include all the situation;
            if (num1->isKonwn) {
                instr.emplace_back(PcodeOp::LIT, "0", num1->value);
            } else {
                int level = findVar(num1->value);
                instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level),
                                   varMap.at(level).at(num1->value).address);
            }
            instr.emplace_back(PcodeOp::STO, "0", to_string(addressStack.back()));
        }
        addr.address = to_string(addressStack.back()++);
    } else {
        // 否则是数组，则意味着 有数组初始化和无数组初始化两个部分。
        int length = 1;
        for (const auto &i : num1->dimensions) {
            length *= stoi(i);
        }
        int beginAddress = addressStack.back(); // 存储数组分配的初始地址
        addr.address = to_string(beginAddress);
        addressStack.back() += length;
        instr.emplace_back(PcodeOp::INT, "0", to_string(length)); // 创造数组栈空间
        // 接下来 是 数组的初始化，初始化值是存储在num1->arrValue中的
        for (auto iter = num1->arrValue.begin(); iter != num1->arrValue.end(); iter++) {
            vector<string> splitDim = split(iter->first, ",");
            vector<int> dim;
            std::transform(splitDim.begin(), splitDim.end(), back_inserter(dim),
                           [](const string &i) { return stoi(i); });
            int corr = Parsing::getCount(num1->dimensions, dim);
            // 初始化值可能是常数可能不是常数
            if (iter->second.at(0) == '$') {
                // 初始化值为变量
                int level = findVar(iter->second);
                instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level),
                                   varMap.at(level).at(iter->second).address);
            } else {
                // 初始化值为常数
                instr.emplace_back(PcodeOp::LIT, "0", iter->second);
            }
            instr.emplace_back(PcodeOp::STO, "0", to_string(corr + beginAddress - 1));
        }
    }
    varMap.back().insert(make_pair(num1->name, addr));
}

void GenPcode::genCalculate(const Quaternion &quater, const string &number) {
    TableRow num1 = quater.num1;
    TableRow num2 = quater.num2;
    Addr addr;
    addr.isRef = false;
    if (num1.isKonwn) {
        instr.emplace_back(PcodeOp::LIT, "0", num1.value);
    } else {
        int level = findVar(num1.name);
        instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level), varMap.at(level).at(num1.name).address);
    }
    if (number != "1") {
        if (num2.isKonwn) {
            instr.emplace_back(PcodeOp::LIT, "0", num2.value);
        } else {
            int level = findVar(num2.name);
            instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level),
                               varMap.at(level).at(num2.name).address);
        }
    }
    instr.emplace_back(PcodeOp::OPR, "0", number);
    instr.emplace_back(PcodeOp::INT, "0", "1");
    instr.emplace_back(PcodeOp::STO, "0", to_string(addressStack.back()));
    addr.address = to_string((addressStack.back()++));
    varMap.back().insert(make_pair(quater.result.name, addr));
}


void GenPcode::genLor(Quaternion quater) {
    nameToAddr.insert(make_pair("$lor" + quater.num1.value, to_string(instr.size())));
    instr.emplace_back(PcodeOp::JTP, "0", quater.num1.name);
}

void GenPcode::genLand(Quaternion quater) {

    if (!quater.num2.isKonwn) {
        int level = findVar(quater.num2.name);
        instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level),
                           varMap.at(level).at(quater.num2.name).address);
    } else {
        instr.emplace_back(PcodeOp::LIT, "0", quater.num2.value);
    }
    instr.emplace_back(PcodeOp::JPCAND, "0", quater.num1.name);

}

void GenPcode::genCondBegin(Quaternion quater) {
// nothing
}

void GenPcode::genCondEnd(const Quaternion &quater) {
    // 栈顶为假则跳转
    nameToAddr.insert(make_pair("$cond" + quater.num1.name, to_string(instr.size())));
    instr.emplace_back(PcodeOp::JPC, "", "$" + quater.num1.name);
}

void GenPcode::genAssign(const Quaternion &quater) {
    TableRow num1 = quater.num1;
    TableRow num2 = quater.num2;
    int levelNum1 = findVar(num1.name);
    int levelNum2;
    if (num2.isKonwn) {
        instr.emplace_back(PcodeOp::LIT, "0", num2.value);
    } else {
        levelNum2 = findVar(num2.name);
        instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - levelNum2),
                           varMap.at(levelNum2).at(num2.name).address);
    }
    if (num1.type == "address") {
        instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - levelNum1),
                           varMap.at(levelNum1).at(num1.name).address);
        instr.emplace_back(PcodeOp::STOS, "", "");
    } else {
        instr.emplace_back(PcodeOp::STO, to_string(varMap.size() - 1 - levelNum1),
                           varMap.at(levelNum1).at(num1.name).address); // 放进去
    }

}

void GenPcode::genGetInt(const Quaternion &quater) {
    TableRow num1 = quater.num1;
    int level = findVar(num1.name);
    instr.emplace_back(PcodeOp::OPR, "0", "16");
    if (num1.type == "address") {
        instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level),
                           varMap.at(level).at(num1.name).address);
        instr.emplace_back(PcodeOp::STOS, "", "");
    } else {
        instr.emplace_back(PcodeOp::STO, to_string(varMap.size() - 1 - level),
                           varMap.at(level).at(num1.name).address); // 放进去
    }
}

void GenPcode::genArrRef(const Quaternion &quater) {
    // 在此处生成数组引用的中间变量时，需要标记出isRef, 将引用数组的地址也即在引用数组层中的给存储到栈中。
    // 在函数调用时 callTableRow为参数赋值时，直接将此中间变量的addr的各种值进行拷贝，同时不需要生成Pcode代码
    // 这样在进行数组访问时，生成Pcode时直接进行地址的替换即可，替换过程不需要保存在Pcode中，全在本地即可。

    //
    //  数组引用的时候，存在问题。
    //  首先数组引用的生成是在某一层的函数调用之前产生的，然后在后面对于一个函数来说，想要使用传递过来的参数只能
    //  只能够通过栈来进行取值，也就是在函数运行的Pcode代码生成过程中，是没有进行函数的调用的。
    //  以前是将原数组的相对地址记录到栈中，现在需要改为将原数组的绝对地址存储到栈中！！！
    //  因为相对地址需要拿到level，才可以进行相对地址的取值。
    //  绝对地址求法：两种情况，一种情况是当前内部定义的，也即层次差为0时，可以直接使用当前的基址寄存器的值加上地址偏移，看来需要增加新指令la。表明是取当前基地址寄存器的值到栈顶。
    //  也即 la   ； LIT 0 address； OPR 0 2；此时栈顶值为绝对地址。
    //  第二种情况是层次差不为0的时候，假设层次差为n，取第n-1层的DL的值 加上地址偏移
    //  也即 LOD n-1 1； LIT 0 address； OPR 0 2； 此时栈顶的值即为绝对地址
    TableRow num1 = quater.num1;
    TableRow result = quater.result;
    Addr addr;
    int level = findVar(num1.name);
    Addr tempAddr = varMap.at(level).at(num1.name);
    addr.isRef = true;
    NEW_INSTR("1")
    if (num1.dimensions.size() == 1 && result.value == "0" || num1.dimensions.size() == 2 && result.value == "0,0") {
        // 此时表明是一个完全的数组引用
        if (varMap.at(level).at(num1.name).isRef) {
            instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level), tempAddr.address);
            addr.dims = tempAddr.dims;
        } else {
            if ((varMap.size() - 1 - level) == 0) {
                instr.emplace_back(PcodeOp::LA, "", "");
            } else {
//                int sum = 0;
//                for (int i = 0; i < level; i++) {
//                    sum += addressStack[i];
//                }
//                instr.emplace_back(PcodeOp::LIT, "0", to_string(sum));
                instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level - 1), "0");
            }
            instr.emplace_back(PcodeOp::LIT, "0", varMap.at(level).at(num1.name).address);
            ADD_INSTR
            addr.dims = num1.dimensions;
        }
        instr.emplace_back(PcodeOp::STO, "0", to_string(addressStack.back()));

    } else if (split(result.value, ",").size() == 1) {
        if (isdigit(result.value[0])) {
            // 开头是数字表明是一个已知，否则不是已知
            instr.emplace_back(PcodeOp::LIT, "0", result.value);
        } else {
            int tempLevel = findVar(result.value);
            instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - tempLevel),
                               varMap.at(tempLevel).at(result.value).address);
        }
        if (varMap.at(level).at(num1.name).isRef) {
            // 也即 部分引用一个完全引用的二维数组
            // 部分引用完全引用的数组，首先，完全被引用的数组的地址在栈中，level在varMap中，其次部分引用的方式可能是一个变量或常量，需要进行计算。
            instr.emplace_back(PcodeOp::LIT, "0", tempAddr.dims[1]);
            MUL_INSTR
            instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level), tempAddr.address);
            addr.dims = {tempAddr.dims[1]};
        } else {
            instr.emplace_back(PcodeOp::LIT, "0", num1.dimensions[1]);
            MUL_INSTR
            instr.emplace_back(PcodeOp::LIT, "0", varMap.at(level).at(num1.name).address);
            ADD_INSTR
            if (varMap.size() - 1 - level == 0) {
                instr.emplace_back(PcodeOp::LA, "", "");
            } else {
//                int sum = 0;
//                for (int i = 0; i < level; i++) {
//                    sum += addressStack[i];
//                }
//                instr.emplace_back(PcodeOp::LIT, "0", to_string(sum));
                instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level - 1), "0");

            }
            addr.dims = {num1.dimensions[1]};
        }
        ADD_INSTR
        instr.emplace_back(PcodeOp::STO, "0", to_string(addressStack.back()));
    }
    addr.address = to_string(addressStack.back()++);
    varMap.back().insert(make_pair(quater.result.name, addr));
}

void GenPcode::genArrLoad(const Quaternion &quater) {
    TableRow num1 = quater.num1;
    int level = findVar(num1.name);
    Addr tempAddr = varMap.at(level).at(num1.name);
    Addr addr;
    addr.isRef = false;
    NEW_INSTR("1")
    vector<string> splitDim = split(quater.result.value, ",");
    vector<string> down = tempAddr.isRef ? tempAddr.dims : num1.dimensions;
    down.push_back("1");
    instr.emplace_back(PcodeOp::LIT, "0", "0");
    for (int i = splitDim.size() - 1; i >= 0; i--) {
        if (isdigit(splitDim[i][0])) {
            instr.emplace_back(PcodeOp::LIT, "0", splitDim[i]);
        } else {
            int tempLevel = findVar(splitDim[i]);
            instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - tempLevel),
                               varMap.at(tempLevel).at(splitDim[i]).address);
        }
        instr.emplace_back(PcodeOp::LIT, "0", down[i + 1]);
        MUL_INSTR
        ADD_INSTR
    }
    if (tempAddr.isRef) {
        instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level), varMap.at(level).at(num1.name).address);
    } else {
        // 不是数组引用
        // 被取值的数组不是一个引用数组，那么我需要求出该数组首部的绝对地址，绝对地址的求法不应该是getAbAddr，
        // 应该为
        if ((varMap.size() - 1 - level) == 0) {
            instr.emplace_back(PcodeOp::LA, "", "");
        } else {
            instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level - 1), "0");
        }
        instr.emplace_back(PcodeOp::OPR, "0", "2");
        instr.emplace_back(PcodeOp::LIT, "0", varMap.at(level).at(num1.name).address);
    }
    instr.emplace_back(PcodeOp::OPR, "0", "2");
    if (quater.num2.value == "0") {
        // 代表此时不是赋值语句的左端的lval
        instr.emplace_back(PcodeOp::LODS, "", "");
    }

    instr.emplace_back(PcodeOp::STO, "0", to_string(addressStack.back()));
    addr.address = to_string(addressStack.back()++);
    varMap.back().insert(make_pair(quater.result.name, addr));
}

void GenPcode::genVarLoad(Quaternion quater) {
    // nothing
}

void GenPcode::genCallBegin(Quaternion quater) {
    // CAL 作用是开辟新的作用域，但是不会进行跳转，跳转是在calEnd的时候 通过jump实现
    NEW_INSTR("3")
    instr.emplace_back(PcodeOp::CAL, "0", "0");
//    instr.emplace_back(PcodeOp::LA, "", "");
//    instr.emplace_back(PcodeOp::STO, "0", to_string(addressStack.back() + 1)); // 手动给DL赋值
    //instr.emplace_back(PcodeOp::LIT, );
    map<string, Addr> sb;
    varMap.push_back(sb);
    addressStack.push_back(3);
}

void GenPcode::genCallEnd(const Quaternion &quater) {
    // CAL参数传参时，由于中间会产生中间变量导致扩大栈空间，所以返回时栈空间增大的不仅仅是quater.num1.dimensions.size + 3
    // 需要在addressStack后面加一个，然后来进行恢复。
    int address = 0;
    for (int i = 0; i < addressStack.size() - 1; i++) {
        address += addressStack[i];
    }
    instr.emplace_back(PcodeOp::LIT, "0", "0");
    instr.emplace_back(PcodeOp::STO, "0", "0");
    instr.emplace_back(PcodeOp::LIT, "0", to_string(instr.size() + 3));
    instr.emplace_back(PcodeOp::STO, "0", "2");
    instr.emplace_back(PcodeOp::JMP, "", nameToAddr.at(quater.num1.name));
    varMap.pop_back();
    addressStack.pop_back();
    if (quater.num1.symbol == "int") {
        NEW_INSTR("1")
        instr.emplace_back(PcodeOp::STO, "0", to_string(addressStack.back()));
        Addr addr;
        addr.isRef = false;
        addr.address = to_string(addressStack.back()++);
        varMap.back().insert(make_pair(quater.result.name, addr));
    }

}

void GenPcode::genCallTableRow(const Quaternion &quater) {
    NEW_INSTR("1")
    if (quater.num1.isKonwn) {
        instr.emplace_back(PcodeOp::LIT, "0", quater.num1.value);
    } else {
        int level = findVar(quater.num1.name);
        instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level),
                           varMap.at(level).at(quater.num1.name).address);
    }
    // 此处存储的地址应该是按照参数的顺序来进行，而不是直接赋值为栈顶，所以应该增加信息，在语法分析中标记出此参数是第n个
    // 然后地址变为 addressStack的前n-1项的和 + 2 + n
    // 而由于中间变量的问题，应该不会覆盖该地址上的原来的数值，理由如下：
    // 中间变量的产生是由于进行了 比如最简单的运算产生的，比如一个8*i 然后 产生一个结果放到了栈中，而上述已经将值取出，所以不必担心会被覆盖
    // 再者假如是函数调用的返回值，返回值也会新的开辟一个空间来进行存储，那就意味着可能该位置刚刚好是中间变量的值，也刚刚好我们需要存储的值，但前面已经将其取出到栈顶所以此处应该还是正确的
    int address = 0;
    for (int i = 0; i < deep; i++) {
        address += addressStack[addressStack.size() - 2 - i];
    }
    // 每一个callBegin的开始都会保存当前基址寄存器的地址，但由于还没有调用所以这个是不确定的值
    instr.emplace_back(PcodeOp::STO, "0", to_string(2 + stoi(quater.num2.value)));
    addressStack.back()++;
}

void GenPcode::genFunBegin(const Quaternion &quater) {
    nameToAddr.insert(make_pair(quater.num1.name, to_string(instr.size())));
    addressStack.push_back(3);
    map<string, Addr> tempMap;
    varMap.push_back(tempMap);
}

void GenPcode::genFunParaDef(const Quaternion &quater) {
    Addr addr;
    addr.isRef = quater.num1.isArr;
    addr.address = to_string(addressStack.back()++);
    addr.dims = quater.num1.dimensions; // 如果是一维那么此dimensions为{"1"}
    varMap.back().insert(make_pair(quater.num1.name, addr));
}

void GenPcode::genFunEnd(Quaternion quater) {
    if (quater.num1.value == "0") {
        instr.emplace_back(PcodeOp::OPR, "0", "0");
    } else {
        instr.emplace_back(PcodeOp::JMP, "", "0");
    }
    varMap.pop_back();
    addressStack.pop_back();

}

void GenPcode::genMain(Quaternion quater) {
    nameToAddr.insert(make_pair("$main", to_string(instr.size())));
    NEW_INSTR("3")
    instr.emplace_back(PcodeOp::CAL, "0", "0");
//    instr.emplace_back(PcodeOp::LA, "", "");
//    instr.emplace_back(PcodeOp::STO, "0", to_string(addressStack.back() + 1)); // 手动给DL赋值
    instr.emplace_back(PcodeOp::JMP, "", to_string(instr.size() + 1));

    addressStack.push_back(3);
    map<string, Addr> tempMap;
    varMap.push_back(tempMap);

}

void GenPcode::genIfBegin(Quaternion quater) {
// nothing
}

void GenPcode::genIfEnd(const Quaternion &quater) {
//
    if (quater.num1.value == "1") {
        // 表明有else，需要产生一个跳转
        instr.emplace_back(PcodeOp::JMP, "", "$elseEnd" + quater.num1.name);
    }
    nameToAddr.insert(make_pair("$ifEnd" + quater.num1.name, to_string(instr.size())));
}

void GenPcode::genElseBegin(Quaternion quater) {
    // nothing
}

void GenPcode::genElseEnd(const Quaternion &quater) {
    nameToAddr.insert(make_pair("$elseEnd" + quater.num1.name, to_string(instr.size())));
}

void GenPcode::genWhileBegin(const Quaternion &quater) {
    nameToAddr.insert(make_pair("$whileBegin" + quater.num1.name, to_string(instr.size())));
    Addr addr;
    addr.address = "-1";
    varMap.back().insert(make_pair("$whileEnd" + quater.num1.name, addr));
}

void GenPcode::genWhileEnd(const Quaternion &quater) {
    instr.emplace_back(PcodeOp::JMP, "", "$whileBegin" + quater.num1.name);
    nameToAddr.insert(make_pair("$whileEnd" + quater.num1.name, to_string(instr.size())));
}

void GenPcode::genMyBreak(const Quaternion &quater) {
    instr.emplace_back(PcodeOp::JMP, to_string(varMap.size() - 1 - findVar("$whileEnd" + quater.num1.name)),
                       "$whileEnd" + quater.num1.name);
}

void GenPcode::genMyContinue(const Quaternion &quater) {
    instr.emplace_back(PcodeOp::JMP, to_string(varMap.size() - 1 - findVar("$whileEnd" + quater.num1.name)),
                       "$whileBegin" + quater.num1.name);
}

void GenPcode::genPrintfStr(const Quaternion &quater) {
    string str = quater.num1.value;
    regex pattern("\\\\n");
    //smatch res;
    int count = 0;
    for (sregex_iterator it(str.begin(), str.end(), pattern), end_it; it != end_it; ++it) {
        count++;
    }
    vector<string> strs;
    int count1 = count;
    for (sregex_iterator it(str.begin(), str.end(), pattern), end_it; it != end_it; ++it) {
        strs.push_back(it->prefix().str());
        count1--;
        if (count1 == 0) {
            strs.push_back(it->suffix().str());
        }
    }
    int i = 0;
    if (count != 0) {
        for (i = 0; i < strs.size() - 1; i++) {
            if (!strs[i].empty()) {
                instr.emplace_back(PcodeOp::PRT, "0", strs[i]);
            }
            instr.emplace_back(PcodeOp::OPR, "0", "15");
        }
        if (!strs[i].empty()) {
            instr.emplace_back(PcodeOp::PRT, "0", strs[i]);
        }
    } else {
        if (!str.empty()) {
            instr.emplace_back(PcodeOp::PRT, "0", str);
        }
    }

}

void GenPcode::genPrintfInt(const Quaternion &quater) {
    if (quater.num1.isKonwn) {
        instr.emplace_back(PcodeOp::LIT, "0", quater.num1.value);
    } else {
        int level = findVar(quater.num1.name);
        instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level),
                           varMap.at(level).at(quater.num1.name).address);
    }
    instr.emplace_back(PcodeOp::OPR, "0", "14");
}

void GenPcode::genMyReturn(const Quaternion &quater) {
    if (quater.num1.symbol == "void") {
        instr.emplace_back(PcodeOp::OPR, "0", "0");
    } else {
        if (quater.num1.isKonwn) {
            instr.emplace_back(PcodeOp::LIT, "0", quater.num1.value);
        } else {
            int level = findVar(quater.num1.name);
            instr.emplace_back(PcodeOp::LOD, to_string(varMap.size() - 1 - level),
                               varMap.at(level).at(quater.num1.name).address);
        }
        instr.emplace_back(PcodeOp::OPR, "0", "0");
    }
}

void GenPcode::genBlockBegin(const Quaternion &quater) {
    instr.emplace_back(PcodeOp::BLO, "", "");
    map<string, Addr> sb;
    varMap.push_back(sb);
    addressStack.push_back(3);
}

void GenPcode::genBlockEnd() {
    instr.emplace_back(PcodeOp::BLE, "0", "0");
    addressStack.pop_back();
    varMap.pop_back();
}

int GenPcode::getAbAddr(int level) {
    if (level > addressStack.size() - 1) {
        return -1;
    }
    int sum = 0;
    for (int i = 0; i < level; i++) {
        sum += addressStack[i];
    }
    return sum;
}



