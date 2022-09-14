#include "Token.h"
#include "getSym.h"
#include "parsing.h"

#define IF_AND_WRITE(b)         \
    if (token.symbol == b) {    \
        writeFile(token);           \
    }

#define GET_IF_AND_WRITE(b) getNext(); \
                            IF_AND_WRITE(b)

#define GET_IF_AND_WRITE_COVER(b, a) getNext(); \
if(token.symbol == b) {writeFile(token); a;}

#define GOOD_BYE if(goodBye){return;}

#define SET_GOOD_BYE else {goodBye = true; return;}

#define SAVE_NOW tempIndex = token.index; \
                tempLine = token.lineNum;

#define GO_BACK goBack(tempIndex, tempLine);

#define ERROR(message) writeError(token.lineNum, message);


//
//  符号表的建立主要分为三部分：常量定义，变量定义，函数定义
//  常量定义需要解决：
//      constExp 求值
//      设计多维数组递归分析时的坐标的获取
//  变量定义需要解决：
//      exp 求值，定义是什么就写什么
//      设计多维数组递归分析时的坐标的获取
//  函数定义需要解决：
//      参数列表的信息的统计
//
//

/*
 * 目前符号表仍然存在一部分的问题：
 * 首先符号表需要进行记录的是：
 * 1. 常量定义
 *      * 常量定义，目前已经进行了常量的名字的统计，但存在以下问题
 *              - 首先是常量数组维度的定义。常量数组维度的定义是constExp也就意味着是一些常量的加减乘除，常量可以是普通的数也可以是已经定义好的常量，出现了一个递归的问题
 *              - 其次就是常量的初始化问题，首先常量的初始化值必须为constExp，那么问题同一，然后多维数组初始化的方式。显然都需要在编译时进行确定。我需要在编译期间将constExp的值求出来。
 * 2. 变量定义
 *      * 变量定义，目前已经进行了变量的名字的统计，但存在以下问题
 *              - 首先是变量的数组维度的定义。由于是constExp，所以需要在编译期间将值算出来，问题同常量定义的第一个。
 *              - 其次是变量的初始化问题，由于其中是exp，所以需要单独的生成计算exp的指令来进行计算。但是由于初始化的问题存在，可能需要一个一个对每一个地址进行初始化！！！
 *
 * 3. 函数的定义
 * 函数的定义目前的问题为：简简单单 constExp的求值问题！！！！！！！！！！！！
 *
 *
 * 综上所分析来说，constExp在编译期间需要进行求值。而exp需要在编译期间进行求值。
 * 至于初始化的方式很简单：目前想法是在符号表的建立过程中将每一维度的值按照类似于"a,b,c"的空间向量的形式存储，也就是说对数组的全部变量都生成对应的（"a,b,c", 0）类似的键值对，然后在进行指令的生成时
 * 记录每一个变量的栈中起始和终止的地址，然后对其中的每一个地址进行一个一个的初始化，也就是对每一个地址执行一次store指令，默认值都为0
 * 关于递归分析初始化值的问题。。。
 * 主要有两种
 *
 *
 * constExp放在编译时运算，那么会面临一系列的问题：
 * 如何放在编译时运算？放在编译时运算，首先constExp就是一个常数、常量、常量数组的引用之间的加减乘除，常数之间进行计算，常量和常量数组需要从符号表中获取
 * 首先是如何不做不必要的四元式的生成？
 * 四元式的生成是肯定需要在每一级进行的，那么就不可以避免的进行生成多余的，若进行标注？
 *
 *
 */




Parsing::Parsing(Token &tokenIn) : token(tokenIn) {
    cover = CoverageChecker();
}

void Parsing::goBack(int index, int lineNum) {
    token.index = index;
    token.lineNum = lineNum;
}

void Parsing::getNext() {
    if (token.index < token.length) {
        getSym(token);
    } else {
        token.symbol = "";
    }
}

// begin all
void Parsing::parsingProgram() {
    getNext();
    if (token.symbol.empty()) {
        writeFile("<CompUnit>");
        return;
    }
    if (token.symbol == "CONSTTK") {
        writeFile(token);
        constDecl();
        GOOD_BYE
        cover.setDecl();
    } else if (token.symbol == "VOIDTK") {
        writeFile(token);
        isHasReturn = 0;
        if (isHasFun == 0) {
            quaterions.emplace_back(QuadOp::jMain);
        }
        isHasFun = 1;
        writeFile("<FuncType>");
        funcDef();
        GOOD_BYE
        cover.setVoidFunc();
        writeFile("<FuncDef>");
    } else if (token.symbol == "INTTK") {
        cover.setIntBType();
        writeFile(token);
        SAVE_NOW
        getNext();
        if (token.symbol == "MAINTK") {
            GO_BACK
            if (isHasFun == 0) {
                quaterions.emplace_back(QuadOp::jMain);
            }
            isHasFun = 2;
            isHasReturn = 1;
            token.word = "int";
            funcDef();
            GOOD_BYE
            cover.setMainFuncDef();
            writeFile("<MainFuncDef>");
        } else if (token.symbol == "IDENFR") {
            getNext();
            if (token.symbol == "LPARENT") {
                GO_BACK
                isHasReturn = 1;
                if (isHasFun == 0) {
                    quaterions.emplace_back(QuadOp::jMain);
                }
                isHasFun = 1;
                token.word = "int";
                writeFile("<FuncType>");
                funcDef();
                GOOD_BYE
                cover.setIntFunc();
                writeFile("<FuncDef>");
            } else {
                GO_BACK
                token.symbol = "INTTK";
                varDecl();
                cover.setDecl();
                GOOD_BYE
            }
        } else {
            goodBye = true;
            return;
        }
    } else {
        goodBye = true;
        return;
    }
    parsingProgram();
    GOOD_BYE
}

void Parsing::constDecl() {
    GET_IF_AND_WRITE_COVER("INTTK", cover.setIntBType())
    SET_GOOD_BYE
    int i = 0;
    while (token.symbol != "SEMICN") {
        tempRow.clear();
        tempRow.type = "const";
        tempRow.symbol = "int";
        constDef();
        GOOD_BYE
        quaterions.emplace_back(QuadOp::constDef, tempRow);
        SAVE_NOW
        GET_IF_AND_WRITE("COMMA")
        else if (token.symbol != "SEMICN") {
            GO_BACK
            ERROR("i")
            break;
        }
        i++;
    }
    IF_AND_WRITE("SEMICN")
    if (i == 1) {
        cover.setOneConstDef();
    } else if (i > 1) {
        cover.setManyConstDef();
    }
    cover.setConDecl();
    writeFile("<ConstDecl>");
}

void Parsing::constDef() {
    GET_IF_AND_WRITE_COVER("IDENFR", cover.setIdent())
    SET_GOOD_BYE
    tempRow.name = token.word;
    if (tableStack.back()->find(token.word) != tableStack.back() -> end()) {
        ERROR("b")
    }
    SAVE_NOW
    getNext();
    if (token.symbol == "ASSIGN") {
        writeFile(token);
        constInitVal(0);
        GOOD_BYE
        cover.setNormalConDef();
        cover.setNormalConstInit();
        writeFile("<ConstInitVal>");
        tempRow.value = quaterions.back().result.value;
        tempRow.arrValue.clear();
    } else if (token.symbol == "LBRACK") {
        tempRow.isArr = true;
        writeFile(token);
        while (token.symbol != "ASSIGN") {
            constExp();
            GOOD_BYE
            tempRow.dimensions.push_back(quaterions.back().result.value);
            SAVE_NOW
            GET_IF_AND_WRITE("RBRACK")
            else {
                GO_BACK
                ERROR("k")
            }
            GET_IF_AND_WRITE("LBRACK")
            else if (token.symbol == "ASSIGN") {
                writeFile(token);
            }
            SET_GOOD_BYE
        }
        arrCoors.clear();
        for (int i = 0; i < tempRow.dimensions.size(); i++) {
            arrCoors.push_back(0);
        }
        if (tempRow.dimensions.size() == 1) {
            cover.setArrOneDef();
            cover.setArrOneConstInit();
        } else if (tempRow.dimensions.size() == 2) {
            cover.setArrTwoDef();
            cover.setArrTwoConstInit();
        }
        constInitVal(0);
        GOOD_BYE
        writeFile("<ConstInitVal>");
    }
    SET_GOOD_BYE
    currTable->insert(make_pair(tempRow.name, tempRow));
    writeFile("<ConstDef>");
    // 结束时，需要保证不多读
}

void Parsing::constInitVal(int dimCount) {
    SAVE_NOW
    getNext();
    if (token.symbol != "LBRACE") {
        GO_BACK
        constExp();
        GOOD_BYE
        tempRow.arrValue.insert(make_pair(getDown(tempRow.dimensions, arrValueCount), quaterions.back().result.value));
        arrValueCount++;
    } else if (token.symbol == "LBRACE") {
        writeFile(token);
        for (int i = dimCount; i < tempRow.dimensions.size(); i++) {
            if (i >= arrCoors.size()) {
                arrCoors.push_back(0);
            } else {
                arrCoors.at(i) = 0;
            }
        }
        arrValueCount = getCount(tempRow.dimensions, arrCoors);
        arrValueCount = arrValueCount == 0 ? 1 : arrValueCount;
        SAVE_NOW
        getNext();
        if (token.symbol == "RBRACE") {
            writeFile(token);
            ++arrCoors.at(dimCount - 1);
        } else {
            GO_BACK
            while (token.symbol != "RBRACE") {
                constInitVal(dimCount + 1);
                GOOD_BYE
                writeFile("<ConstInitVal>");
                getNext();
                if (token.symbol == "COMMA") {
                    writeFile(token);
                } else if (token.symbol == "RBRACE") {
                    if (dimCount > 0) {
                        ++arrCoors.at(dimCount - 1);
                    }
                }
                SET_GOOD_BYE
            }
            writeFile(token);
        }
    }
}

void Parsing::varDecl() {
    int i = 0;
    while (token.symbol != "SEMICN") {
        tempRow.clear();
        tempRow.type = "var";
        tempRow.symbol = "int";
        varDef();
        GOOD_BYE
        quaterions.emplace_back(QuadOp::varDef, tempRow);
        SAVE_NOW
        GET_IF_AND_WRITE("COMMA")
        else if (token.symbol != "SEMICN") {
            GO_BACK
            ERROR("i")
            break;
        }
        i++;
    }
    if (i == 1) {
        cover.setOneVarDef();
    } else if (i > 1) {
        cover.setManyVarDef();
    }
    IF_AND_WRITE("SEMICN")
    cover.setVarDecl();

    writeFile("<VarDecl>");
}

// VarDef → IDENFR { '[' ConstExp ']' } | IDENFR { '[' ConstExp ']' } '=' InitVal
void Parsing::varDef() {
    GET_IF_AND_WRITE_COVER("IDENFR", cover.setIdent())
    SET_GOOD_BYE
    tempRow.name = token.word;
    if (tableStack.back()->find(token.word) != tableStack.back() -> end()) {
        ERROR("b")
    }
    SAVE_NOW
    getNext();
    if (token.symbol == "ASSIGN") {
        writeFile(token);
        initVal(0);
        GOOD_BYE
        writeFile("<InitVal>");
        cover.setNormalVarDefInit();
        tempRow.value = quaterions.back().result.isKonwn ? quaterions.back().result.value
                                                         : quaterions.back().result.name;
        tempRow.isKonwn = quaterions.back().result.isKonwn;
    } else if (token.symbol == "LBRACK") {
        tempRow.isArr = true;
        while (token.symbol == "LBRACK") {
            writeFile(token);
            constExp();
            GOOD_BYE
            tempRow.dimensions.push_back(quaterions.back().result.value);
            SAVE_NOW
            getNext();
            if (token.symbol == "RBRACK") {
                writeFile(token);
            } else {
                GO_BACK
                ERROR("k")
            }
            SAVE_NOW
            getNext();
        }
        GO_BACK
        getNext();
        if (token.symbol == "ASSIGN") {
            writeFile(token);
            initVal(0);
            GOOD_BYE
            if (tempRow.dimensions.size() == 1) {
                cover.setArrOneVarDefInit();
            } else if (tempRow.dimensions.size() == 2) {
                cover.setArrTwoVarDefInit();
            }
            writeFile("<InitVal>");
        } else {
            if (tempRow.dimensions.size() == 1) {
                cover.setArrOneVarDefNoInit();
            } else if (tempRow.dimensions.size() == 2) {
                cover.setArrTwoVarDefNoInit();
            }
            GO_BACK
        }
    } else {
        cover.setNormalVarDefNoInit();
        GO_BACK
    }
    currTable->insert(make_pair(tempRow.name, tempRow));
    writeFile("<VarDef>");
}

void Parsing::initVal(int dimCount) {
    SAVE_NOW
    getNext();
    if (token.symbol != "LBRACE") {
        GO_BACK
        exp();
        GOOD_BYE
        tempRow.arrValue.insert(make_pair(getDown(tempRow.dimensions, arrValueCount),
                                          quaterions.back().result.isKonwn ? quaterions.back().result.value
                                                                           : quaterions.back().result.name));
        arrValueCount++;
    } else {
        writeFile(token);
        for (int i = dimCount; i < tempRow.dimensions.size(); i++) {
            if (i >= arrCoors.size()) {
                arrCoors.push_back(0);
            } else {
                arrCoors.at(i) = 0;
            }
        }
        arrValueCount = getCount(tempRow.dimensions, arrCoors);
        arrValueCount = arrValueCount == 0 ? 1 : arrValueCount;
        SAVE_NOW
        getNext();
        if (token.symbol == "RBRACE") {
            writeFile(token);
            if (dimCount > 0) {
                ++arrCoors.at(dimCount - 1);
            }
        } else {
            GO_BACK
            while (token.symbol != "RBRACE") {
                initVal(dimCount + 1);
                GOOD_BYE
                writeFile("<InitVal>");
                getNext();
                if (token.symbol == "COMMA") {
                    writeFile(token);
                } else if (token.symbol == "RBRACE") {
                    if (dimCount > 0) {
                        ++arrCoors.at(dimCount - 1);
                    }
                }
                SET_GOOD_BYE
            }
            writeFile(token);
        }
    }
}

// FuncDef → FuncType IDENFR '(' [FuncFParams] ')' Block
void Parsing::funcDef() {

    tempRow.clear();
    tempRow.type = "fun";
    tempRow.symbol = token.word;
    bool isMain = false;
    getNext();
    if (token.symbol == "IDENFR") {
        cover.setIdent();
        writeFile(token);
        tempRow.name = token.word;
        quaterions.emplace_back(QuadOp::funBegin, tempRow);
    } else {
        if (token.symbol == "MAINTK") {
            isMain = true;
            quaterions.emplace_back(QuadOp::main);
            writeFile(token);
        }
        SET_GOOD_BYE
    }
    if (tableStack.back()->find(token.word) != tableStack.back() -> end()) {
        ERROR("b")
    }
    GET_IF_AND_WRITE("LPARENT")
    SET_GOOD_BYE
    map <string, TableRow> tempTable = map<string, TableRow>();
    tableStack.push_back(&tempTable);
    currTable = tableStack.back();
    SAVE_NOW
    getNext();
    if (token.symbol == "RPARENT") {
        if (!isMain) {
            cover.setNoneParaFunc();
        }
        writeFile(token);
    } else {
        GO_BACK
        funcParams();
        cover.setHasParaFunc();
        GOOD_BYE
    }
    globalTable.insert(make_pair(tempRow.name, tempRow));
    block(true);
    GOOD_BYE
    if (isHasReturn == 1) {
        ERROR("g")
    }
    tableStack.pop_back();
    currTable = tableStack.back();
    quaterions.emplace_back(QuadOp::funEnd, TableRow("", "", "", isMain ? "1" : "0"));
    if (!isMain) {
        cover.setFuncDef();
    }

}

// FuncFParams → FuncFParam { ',' FuncFParam }
void Parsing::funcParams() {
    int i = 0;
    while (token.symbol != "RPARENT") {
        funTempRow.clear();
        funcParam();
        GOOD_BYE
        currTable->insert(make_pair(funTempRow.name, funTempRow));
        quaterions.emplace_back(QuadOp::funParaDef, funTempRow);
        if (token.symbol == "COMMA") {
            writeFile(token);
        } else if (token.symbol != "RPARENT") {
            GO_BACK
            ERROR("j")
            break;
        }
        i++;
    }
    writeFile("<FuncFParams>");
    IF_AND_WRITE("RPARENT")
    if (i == 1) {
        cover.setOneParaFunc();
    } else if (i > 1) {
        cover.setManyParaFunc();
    }

}

void Parsing::funcParam() {
    Param param;
    GET_IF_AND_WRITE_COVER("INTTK", cover.setIntBType())
    SET_GOOD_BYE
    funTempRow.type = "var";
    funTempRow.symbol = "int";
    GET_IF_AND_WRITE_COVER("IDENFR", cover.setIdent())
    SET_GOOD_BYE
    funTempRow.name = token.word;
    funTempRow.isKonwn = false;
    GET_IF_AND_WRITE("LBRACK")
    else {
        cover.setNormalParaFunc();
        param.array = false;
        tempRow.funParams.push_back(param);
        funTempRow.isArr = false;
        writeFile("<FuncFParam>");
        return;
    }
    funTempRow.isArr = true;
    SAVE_NOW
    GET_IF_AND_WRITE("RBRACK")
    else {
        GO_BACK
        ERROR("k")
    }
    param.array = true;
    param.paramDim.emplace_back("-1");
    funTempRow.dimensions.emplace_back("-1");
    getNext();
    int i = 0;
    while (token.symbol == "LBRACK") {
        writeFile(token);
//      exp(); 在此处exp是等价于constExp的
        constExp();
        GOOD_BYE
        param.paramDim.push_back(quaterions.back().result.value);
        funTempRow.dimensions.push_back(quaterions.back().result.value);
        SAVE_NOW
        getNext();
        IF_AND_WRITE("RBRACK")
        else {
            GO_BACK
            ERROR("k")
        }
        SAVE_NOW
        getNext();
        i++;
    }
    if (i == 0) {
        cover.setArrOneParaFunc();
    } else if (i == 1) {
        cover.setArrTwoParaFunc();
    }
    tempRow.funParams.push_back(param);
    writeFile("<FuncFParam>");
}

void Parsing::block(bool isFun) {
    if (!isFun) {
        map <string, TableRow> tempTable = map<string, TableRow>();
        tableStack.push_back(&tempTable);
        currTable = tableStack.back();
        quaterions.emplace_back(QuadOp::blockBegin);
    }
    GET_IF_AND_WRITE("LBRACE")
    SET_GOOD_BYE
    blockItem();
    GOOD_BYE
    // print
    writeFile(token);
    if (!isFun) {
        tableStack.pop_back();
        currTable = tableStack.back();
        quaterions.emplace_back(QuadOp::blockEnd);
    }
    writeFile("<Block>");
}

void Parsing::blockItem() {
    SAVE_NOW
    getNext();
    if (token.symbol == "RBRACE") {
        cover.setNoneItemBlock();
        return;
    }
    while (token.symbol != "RBRACE") {
        if (token.symbol == "CONSTTK") {
            writeFile(token);
            constDecl();
            GOOD_BYE
            cover.setDeclItem();
        } else if (token.symbol == "INTTK") {
            cover.setIntBType();
            writeFile(token);
            varDecl();
            GOOD_BYE
            cover.setDeclItem();
        } else {
            // already see one
            GO_BACK
            stmt();
            GOOD_BYE
            cover.setStmtItem();
        }
        cover.setHasItemBlock();
        SAVE_NOW
        getNext();
    }

}

void Parsing::stmt() {
    SAVE_NOW
    getNext();
    if (token.symbol == "IDENFR") {
        Quaternion quad;
        if (findAssign()) {
            // LVal '=' Exp ';'
            cover.setIdent();
            writeFile(token);
            lval(true);
            GOOD_BYE
            quad.num1 = quaterions.back().result;
            GET_IF_AND_WRITE("ASSIGN")
            SET_GOOD_BYE
            SAVE_NOW
            getNext();
            if (token.symbol == "GETINTTK") {
                quad.op = QuadOp::getInt;
                writeFile(token);
                GET_IF_AND_WRITE("LPARENT")
                SET_GOOD_BYE
                SAVE_NOW
                GET_IF_AND_WRITE("RPARENT")
                else {
                    GO_BACK
                    ERROR("j")
                }
                cover.setGetintStmt();
            } else {
                GO_BACK
                quad.op = QuadOp::assign;
                exp();
                GOOD_BYE
                quad.num2 = quaterions.back().result;
                cover.setAssignStmt();
            }
            SAVE_NOW
            GET_IF_AND_WRITE("SEMICN")
            else {
                GO_BACK
                ERROR("i")
            }
            quaterions.push_back(quad);
        } else {
            GO_BACK
            exp();
            GOOD_BYE
            SAVE_NOW
            GET_IF_AND_WRITE("SEMICN")
            else {
                GO_BACK
                ERROR("i")
            }
            cover.setExpStmt();
        }
    } else if (token.symbol == "SEMICN") {
        writeFile(token);
        cover.setNoneStmt();
        // [Exp] ';'
        writeFile("<Stmt>");
        return;
    } else if (token.symbol == "LBRACE") {
        // Block
        GO_BACK
        block(false);
        GOOD_BYE
        cover.setBlockStmt();
    } else if (token.symbol == "IFTK") {
        writeFile(token);
        quaterions.emplace_back(QuadOp::ifBegin);
        ifStmt();
        cover.setHasIfStmt();
        GOOD_BYE
    } else if (token.symbol == "WHILETK") {
        writeFile(token);
        isInLoop++;
        whileStmt();
        cover.setHasWhileStmt();
        GOOD_BYE
        isInLoop--;
    } else if (token.symbol == "BREAKTK" || token.symbol == "CONTINUETK") {
        QuadOp op = token.symbol == "BREAKTK" ? QuadOp::myBreak : QuadOp::myContinue;
        token.symbol == "BREAKTK" ? cover.setBreakStmt() : cover.setContinueStmt();
        quaterions.emplace_back(op, TableRow(to_string(whileCount)));
        if (isInLoop == 0) {
            ERROR("m")
        }
        writeFile(token);
        SAVE_NOW
        GET_IF_AND_WRITE("SEMICN")
        else {
            GO_BACK
            ERROR("i")

        }
    } else if (token.symbol == "PRINTFTK") {
        // 'printf''('<FormatString>{,<Exp>}')'';'
        writeFile(token);
        GET_IF_AND_WRITE("LPARENT")
        SET_GOOD_BYE
        formatStr();
        GOOD_BYE
        string str = token.word.substr(1, token.word.size() - 2);
        regex pattern("%d");
        //smatch res;
        int count = 0;
        for (sregex_iterator it(str.begin(), str.end(), pattern), end_it; it != end_it; ++it) {
            count++;
        }
        int count1 = count;
        vector <string> strs;
        for (sregex_iterator it(str.begin(), str.end(), pattern), end_it; it != end_it; ++it) {
            strs.push_back(it->prefix().str());
            count1--;
            if (count1 == 0) {
                strs.push_back(it->suffix().str());
            }
        }
        SAVE_NOW
        getNext();
        int i = 0;
        while (token.symbol == "COMMA") {
            writeFile(token);
            quaterions.emplace_back(QuadOp::printfStr);
            quaterions.back().num1.value = strs.at(i);
            exp();
            GOOD_BYE
            quaterions.emplace_back(QuadOp::printfInt, quaterions.back().result);
            SAVE_NOW
            getNext();
            i++;
        }
        if (count == 0) {
            quaterions.emplace_back(QuadOp::printfStr);
            quaterions.back().num1.value = str;
        } else {
            quaterions.emplace_back(QuadOp::printfStr);
            quaterions.back().num1.value = (i > (strs.size() - 1)) ? "" : strs.at(i);
        }

        if (i != count) {
            ERROR("l")
        }
        IF_AND_WRITE("RPARENT")
        else {
            GO_BACK
            ERROR("j")
            // error
        }
        SAVE_NOW
        GET_IF_AND_WRITE("SEMICN")
        else {
            GO_BACK
            ERROR("i")
            // error
        }
        if (i == 0){
            cover.setPrintfNoExpStmt();
        } else if (i > 0) {
            cover.setPrintfHasExpStmt();
        }
        cover.setHasPrintfStmt();
    } else if (token.symbol == "RETURNTK") {
        writeFile(token);

        returnStmt();
        cover.setHasReturnStmt();
        GOOD_BYE
    } else {
        GO_BACK
        exp();
        GOOD_BYE
        SAVE_NOW
        GET_IF_AND_WRITE("SEMICN")
        else {
            GO_BACK
            ERROR("i")
        }
        cover.setExpStmt();
    }
    writeFile("<Stmt>");
}

void Parsing::lval(bool isAssign) {
    // IDENFR {'[' Exp ']'}
    if (findAndGet(token.word).name.empty()) {
        ERROR("c")
    } else if (isAssign && findAndGet(token.word).type == "const") {
        ERROR("h")
    }
    string name = token.word;
    SAVE_NOW
    getNext();
    if (token.symbol == "LBRACK") {
        string coordinate;
        int countDim = 0;
        while (token.symbol == "LBRACK") {
            ++countDim;
            writeFile(token);
            exp();
            GOOD_BYE
            if (quaterions.back().op == QuadOp::number) {
                coordinate += quaterions.back().result.value + ",";
            } else {
                coordinate += quaterions.back().result.name + ",";
            }
            SAVE_NOW
            GET_IF_AND_WRITE("RBRACK")
            else {
                GO_BACK
                ERROR("k")
            }
            SAVE_NOW
            getNext();
        }
        if (countDim == 1) {
            cover.setArrOneLval();
        } else if (countDim == 2) {
            cover.setArrTwoLval();
        }
        GO_BACK
        coordinate = coordinate.substr(0, coordinate.size() - 1);
        TableRow IDENFR = findAndGet(name);
        if (IDENFR.dimensions.size() == countDim) {
            bool flag = false;
            for (auto ch : coordinate) {
                if (!isdigit(ch) && ch != ',') {
                    flag = true;
                    break;
                }
            }
            if (IDENFR.type == "const" && !flag) {
                quaterions.emplace_back(QuadOp::number, IDENFR, TableRow(),
                                        TableRow("", "const", "int", IDENFR.arrValue.at(coordinate)));
                quaterions.back().result.isKonwn = true;
            } else {
                quaterions.emplace_back(QuadOp::arrLoad, IDENFR, TableRow(),
                                        TableRow("$t" + to_string(tempCount++), "address", "", coordinate));
                quaterions.back().result.isKonwn = false;
            }
            quaterions.back().num2.value = to_string(isAssign);
        } else {
            // IDENFR.dimensions.size() != countDim 表明这是一个函数调用的数组引用，需要额外的处理才可以！！！
            quaterions.emplace_back(QuadOp::arrRef);
            quaterions.back().num2.name = "partRef";
            quaterions.back().num1 = IDENFR;
            quaterions.back().result = TableRow("$t" + to_string(tempCount++), "", "", coordinate);
        }
    } else {
        GO_BACK
        cover.setNormalLval();
        TableRow IDENFR = findAndGet(name);
        if (!IDENFR.isArr) {
            if (IDENFR.type == "const") {
                quaterions.emplace_back(QuadOp::number, TableRow(), TableRow(),
                                        TableRow("", "const", "int", IDENFR.value));
                quaterions.back().result.isKonwn = true;
            } else {
                quaterions.emplace_back(QuadOp::varLoad, TableRow(), TableRow(),
                                        TableRow(IDENFR.name, "var", "int", ""));
            }
        } else {
            // 是数组，表明是一个完全的数组引用
            string corr;
            for (auto i : IDENFR.dimensions) {
                corr += "0,";
            }
            corr = corr.substr(0, corr.length() - 1);
            quaterions.emplace_back(QuadOp::arrRef, IDENFR, TableRow("allRef"),
                                    TableRow("$t" + to_string(tempCount++), "", "", corr));
        }
    }
    writeFile("<LVal>");
}

// 'if' '( Cond ')' Stmt [ 'else' Stmt ]
void Parsing::ifStmt() {
    ifCount++;
    int tempIfCount = ifCount;
    GET_IF_AND_WRITE("LPARENT")
    SET_GOOD_BYE
    cond(QuadOp::ifBegin);
    GOOD_BYE
    SAVE_NOW
    GET_IF_AND_WRITE("RPARENT")
    else {
        GO_BACK
        ERROR("j")
    }
    stmt();
    GOOD_BYE
    SAVE_NOW
    getNext();
    if (token.symbol == "ELSETK") {
        writeFile(token);
        quaterions.emplace_back(QuadOp::ifEnd, TableRow(to_string(tempIfCount), "", "", "1"));
        quaterions.emplace_back(QuadOp::elseBegin);
        stmt();
        quaterions.emplace_back(QuadOp::elseEnd, TableRow(to_string(tempIfCount)));
        cover.setIfElseStmt();
    } else {
        // may be error
        // because go back do not goBack(token.symbol);
        cover.setIfStmt();
        quaterions.emplace_back(QuadOp::ifEnd, TableRow(to_string(tempIfCount), "", "", "0"));
        GO_BACK
    }
}

void Parsing::whileStmt() {
    whileCount++;
    int tempWhileCount = whileCount;
    quaterions.emplace_back(QuadOp::whileBegin, TableRow(to_string(tempWhileCount)));
    GET_IF_AND_WRITE("LPARENT")
    SET_GOOD_BYE
    cond(QuadOp::whileBegin);
    GOOD_BYE
    SAVE_NOW
    GET_IF_AND_WRITE("RPARENT")
    else {
        GO_BACK
        ERROR("j")
    }
    stmt();
    GOOD_BYE
    quaterions.emplace_back(QuadOp::whileEnd, TableRow(to_string(tempWhileCount)));
}

void Parsing::returnStmt() {
    Quaternion quaternion;
    quaternion.op = QuadOp::myReturn;
    SAVE_NOW
    quaternion.num1.symbol = "void";
    getNext();
    if (token.symbol == "SEMICN") {
        writeFile(token);
        cover.setReturnNoExpStmt();
        if (isHasReturn >= 1) {
            ERROR("f")
        }
    } else {
        GO_BACK
        exp();
        GOOD_BYE
        if (isHasReturn == 0) {
            ERROR("f")
        } else {
            isHasReturn++;
        }
        quaternion.num1 = quaterions.back().result;
        quaternion.num1.symbol = "int";
        SAVE_NOW
        GET_IF_AND_WRITE("SEMICN")
        else {
            GO_BACK
            ERROR("i")
        }
        cover.setReturnHasExpStmt();
    }
    quaterions.push_back(quaternion);

}

void Parsing::cond(QuadOp op) {
    whereInExp = false;
    quaterions.emplace_back(QuadOp::condBegin);
    lorExp(op);
    GOOD_BYE
    whereInExp = true;
    if (op == QuadOp::whileBegin) {
        quaterions.emplace_back(QuadOp::condEnd, TableRow("whileEnd" + to_string(whileCount)));
    } else {
        quaterions.emplace_back(QuadOp::condEnd, TableRow("ifEnd" + to_string(ifCount)));
    }
    cover.setCondExp();
    writeFile("<Cond>");
}

void Parsing::formatStr() {
    getNext();
    if (token.symbol == "STRCON") {
        writeFile(token);
        for (int i = 1; i < token.word.size() - 1; i++) {
            char c = token.word.at(i);
            if (!isRightFormatChar(c)) {
                if (!(c == '%' && (i + 1 < (token.word.size() - 1)) && token.word.at(i + 1) == 'd')) {
                    ERROR("a")
                    break;
                } else {
                    i = i + 2;
                    continue;
                }
            }
        }
    }
    SET_GOOD_BYE

}

void Parsing::exp() {
    addExp();
    GOOD_BYE
    writeFile("<Exp>");
    cover.setExp();
}

void Parsing::constExp() {
    addExp(); // IDENFR must be const
    GOOD_BYE
    writeFile("<ConstExp>");
    cover.setConstExp();
}

// LOrExp → LAndExp | LOrExp '||' LAndExp
void Parsing::lorExp(QuadOp op) {
    lorCount++;
    TableRow num1;
    lAndExp(op);
    writeFile("<LOrExp>");
    GOOD_BYE
    string name =
            op == QuadOp::whileBegin ? "$condwhileEnd" + to_string(whileCount) : "$condifEnd" + to_string(ifCount);
    quaterions.emplace_back(QuadOp::lor, TableRow(name, "", "", to_string(lorCount)));
    SAVE_NOW
    getNext();
    int i = 0;
    while (token.symbol == "OR") {
        i++;
        cover.setLOr();
        lorCount++;
        writeFile(token);
        lAndExp(op);
        GOOD_BYE
        writeFile("<LOrExp>");
        quaterions.emplace_back(QuadOp::lor, TableRow(name, "", "", to_string(lorCount)));
        SAVE_NOW
        getNext();
    }
    if (i == 0) {
        cover.setOneLOrExp();
    }
    GO_BACK
}

// LAndExp → EqExp | LAndExp '&&' EqExp
void Parsing::lAndExp(QuadOp op) {
    TableRow num1;
    eqExp();
    writeFile("<LAndExp>");
    GOOD_BYE
    quaterions.emplace_back(QuadOp::land, TableRow("$lor" + to_string(lorCount)), quaterions.back().result);
    SAVE_NOW
    getNext();
    int i = 0;
    while (token.symbol == "AND") {
        i++;
        cover.setLAnd();
        writeFile(token);
        eqExp();
        GOOD_BYE
        writeFile("<LAndExp>");
        quaterions.emplace_back(QuadOp::land, TableRow("$lor" + to_string(lorCount)), quaterions.back().result);
        SAVE_NOW
        getNext();
    }
    if (i == 0) {
        cover.setOneLAndExp();
    }
    GO_BACK
}

// EqExp → RelExp | EqExp ('==' | '!=') RelExp
void Parsing::eqExp() {
    TableRow num1;
    relExp();
    writeFile("<EqExp>");
    GOOD_BYE
    num1 = quaterions.back().result;
    SAVE_NOW
    getNext();
    int i =0;
    while (token.symbol == "EQL" || token.symbol == "NEQ") {
        i++;
        QuadOp op = token.symbol == "EQL" ? QuadOp::equal : QuadOp::notEqual;
        token.symbol == "EQL" ? cover.setEql() : cover.setNeq();
        writeFile(token);
        relExp();
        GOOD_BYE
        writeFile("<EqExp>");
        quaterions.emplace_back(op, num1, quaterions.back().result,
                                TableRow("$t" + to_string(tempCount++)));
        num1 = quaterions.back().result;
        SAVE_NOW
        getNext();
    }
    if (i == 0) {
        cover.setOneEqlExp();
    }
    GO_BACK
}

// RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
void Parsing::relExp() {
    TableRow num1;
    addExp();
    writeFile("<RelExp>");
    GOOD_BYE
    num1 = quaterions.back().result;
    SAVE_NOW
    getNext();
    int i = 0;
    while (token.symbol == "LSS" || token.symbol == "LEQ" || token.symbol == "GRE" || token.symbol == "GEQ") {
        i++;
        QuadOp op;
        if (token.symbol == "LSS") {
            op = QuadOp::lss;
            cover.setLss();
        } else if (token.symbol == "LEQ") {
            op = QuadOp::leq;
            cover.setLeq();
        } else if (token.symbol == "GRE") {
            op = QuadOp::gre;
            cover.setGre();
        } else if (token.symbol == "GEQ") {
            op = QuadOp::geq;
            cover.setGeq();
        }
        writeFile(token);
        addExp();
        GOOD_BYE
        writeFile("<RelExp>");
        quaterions.emplace_back(op, num1, quaterions.back().result,
                                TableRow("$t" + to_string(tempCount++)));
        num1 = quaterions.back().result;
        SAVE_NOW
        getNext();
    }
    if (i == 0) {
        cover.setOneRelExp();
    }
    GO_BACK

}

void Parsing::addExp() {
    TableRow num1;
    mulExp();
    writeFile("<AddExp>");
    GOOD_BYE
    num1 = quaterions.back().result;
    SAVE_NOW
    getNext();
    int i = 0;
    while (token.symbol == "PLUS" || token.symbol == "MINU") {
        i++;
        Quaternion quad;
        quad.num1 = num1;
        bool isAdd = token.symbol == "PLUS";
        token.symbol == "PLUS" ? cover.setPlus() : cover.setMinus();
        writeFile(token);
        mulExp();
        GOOD_BYE
        writeFile("<AddExp>");
        quad.num2 = quaterions.back().result;
        if (quad.num1.isKonwn && quad.num2.isKonwn) {
            quad.op = QuadOp::number; //
            quad.result = TableRow("$t" + to_string(tempCount++), "const", "int");
            quad.result.isKonwn = true;
            quad.result.value = to_string(isAdd ? (stoi(quad.num1.value) + stoi(quad.num2.value)) :
                                          (stoi(quad.num1.value) - stoi(quad.num2.value)));
        } else {
            quad.op = isAdd ? QuadOp::add : QuadOp::sub;
            quad.result = TableRow("$t" + to_string(tempCount++), "const", "int");
            quad.result.isKonwn = false;
        }
        quaterions.push_back(quad);
        num1 = quaterions.back().result;
        SAVE_NOW
        getNext();
    }
    if (i == 0) {
        cover.setOneAddExp();
    }
    GO_BACK
}

void Parsing::mulExp() {
    TableRow num1;
    unaryExp();
    writeFile("<MulExp>");
    GOOD_BYE
    num1 = quaterions.back().result;
    SAVE_NOW
    getNext();
    int i = 0;
    while (token.symbol == "MULT" || token.symbol == "DIV" || token.symbol == "MOD") {
        i++;
        string symbol = token.symbol;
        symbol == "MULT" ? cover.setMult() : symbol == "DIV" ? cover.setDiv() : cover.setMod();
        Quaternion quad;
        quad.num1 = num1;
        writeFile(token);
        unaryExp();
        GOOD_BYE
        writeFile("<MulExp>");
        quad.num2 = quaterions.back().result;
        if (quad.num1.isKonwn && quad.num2.isKonwn) {
            quad.op = QuadOp::number;
            quad.result = TableRow("$t" + to_string(tempCount++), "const", "int");
            string value;
            if (symbol == "MULT") {
                value = to_string(stoi(quad.num1.value) * stoi(quad.num2.value));
            } else if (symbol == "DIV") {
                value = to_string(stoi(quad.num1.value) / stoi(quad.num2.value));
            } else if (symbol == "MOD") {
                value = to_string(stoi(quad.num1.value) % stoi(quad.num2.value));
            }
            quad.result.isKonwn = true;
            quad.result.value = value;
        } else {
            quad.op = symbol == "MULT" ? QuadOp::mul : symbol == "DIV" ? QuadOp::div : QuadOp::mod;
            quad.result = TableRow("$t" + to_string(tempCount++), "const", "int");
            quad.result.isKonwn = false;
        }
        quaterions.push_back(quad);
        num1 = quaterions.back().result;
        SAVE_NOW
        getNext();
    }
    if (i == 0) {
        cover.setOneMultExp();
    }
    GO_BACK

}

void Parsing::unaryExp() {
    Quaternion quad;
    SAVE_NOW
    getNext();
    bool unaryOp = whereInExp ? (token.symbol == "PLUS" || token.symbol == "MINU") :
                   (token.symbol == "PLUS" || token.symbol == "MINU" || token.symbol == "NOT");
    if (unaryOp) {
        QuadOp op;
        string symbol = token.symbol;
        writeFile(token);
        writeFile("<UnaryOp>");
        unaryExp();
        GOOD_BYE
        if (symbol == "PLUS" || symbol == "MINU") {
            symbol == "PLUS" ? cover.setPlusUnaryExp() : cover.setMinuUnaryExp();
            op = symbol == "PLUS" ? QuadOp::add : QuadOp::sub;
            if (quaterions.back().result.isKonwn) {
                quaterions.emplace_back(QuadOp::number, TableRow(), TableRow(),
                                        TableRow("", "const", "int",
                                                 (op == QuadOp::add ? quaterions.back().result.value :
                                                 to_string((0 -stoi(quaterions.back().result.value))))));
                quaterions.back().result.isKonwn = true;
            } else {
                quaterions.emplace_back(op, TableRow("", "const", "int", "0"), quaterions.back().result,
                                        TableRow("$t" + to_string(tempCount++), "const", "int", ""));
                quaterions.back().num1.isKonwn = true;
            }
        }
        if (symbol == "NOT") {
            cover.setNotUnaryExp();
            op = QuadOp::myNot;
            quaterions.emplace_back(op, quaterions.back().result, TableRow(),
                                    TableRow("$t" + to_string(tempCount++), "const", "int", ""));
        }
        cover.setUnaryOpUnaryExp();
    } else if (token.symbol == "LPARENT") {
        writeFile(token);
        exp();
        GOOD_BYE
        SAVE_NOW
        GET_IF_AND_WRITE("RPARENT")
        else {
            GO_BACK
            ERROR("j")
        }
        cover.setParentPrimaryExp();
        cover.setPrimaryExpUnaryExp();
        writeFile("<PrimaryExp>");
    } else if (token.symbol == "DEC_CON" || token.symbol == "HEX_CON" || token.symbol == "OCT_CON" ||
               token.symbol == "INTCON") {
        cover.setIntConstExp();
        cover.setNumberPrimaryExp();
        cover.setPrimaryExpUnaryExp();
        writeFile(token);
        int base = token.symbol == "OCT_CON" ? 8 : token.symbol == "HEX_CON" ? 16 : 10;
        quaterions.emplace_back(QuadOp::number, TableRow(), TableRow(),
                                TableRow("", "const", "int", to_string(stoi(token.word, nullptr, base))));
        quaterions.back().result.isKonwn = true;
        writeFile("<Number>");
        writeFile("<PrimaryExp>");
    } else if (token.symbol == "IDENFR") {
        cover.setIdent();
        writeFile(token);
        SAVE_NOW
        string name = token.word;
        int funNameLine = token.lineNum;
        getNext();
        if (token.symbol == "LPARENT") {
            if (findAndGet(name).name.empty()) {
                ERROR("c")
            }
            writeFile(token);
            SAVE_NOW
            getNext();
            if (token.symbol != "RPARENT") {
                GO_BACK
                quaterions.emplace_back(QuadOp::callBegin, findAndGet(name));
                funcRParams(name, funNameLine, Quaternion(QuadOp::callTableRow));
                cover.setOneParaFunCallUnaryExp();
                GOOD_BYE
                quaterions.emplace_back(QuadOp::callEnd, findAndGet(name));
                quaterions.back().result = TableRow("$t" + to_string(tempCount++));
                IF_AND_WRITE("RPARENT")
            } else {
                if (!globalTable.at(name).funParams.empty()) {
                    writeError(funNameLine, "d");
                }
                quaterions.emplace_back(QuadOp::callBegin, findAndGet(name));
                quaterions.emplace_back(QuadOp::callEnd, findAndGet(name));
                quaterions.back().result = TableRow("$t" + to_string(tempCount++));
                writeFile(token);
                cover.setNoneParaFunCall();
            }
            cover.setFunCallUnaryExp();
        } else {
            GO_BACK
            token.word = name;
            lval(false);
            GOOD_BYE
            writeFile("<PrimaryExp>");
            cover.setLvalPrimaryExp();
            cover.setPrimaryExpUnaryExp();
        }
    } else {
        GO_BACK
        goodBye = true;
        return;
    }
    writeFile("<UnaryExp>");
}

void Parsing::funcRParams(const string &funName, int line, Quaternion quad) {
    int count = 0;
    while (token.symbol != "RPARENT") {
        exp();
        GOOD_BYE
        if (globalTable.at(funName).funParams.size() > count) {
            Param param = globalTable.at(funName).funParams.at(count);
            if ((param.array && quaterions.back().op != QuadOp::arrRef) ||
                (!param.array && quaterions.back().op == QuadOp::arrRef) ||
                (param.array && split(quaterions.back().result.value, ",").size() != param.paramDim.size())) {
                writeError(line, "e");
            }
        }
        count++;

//        else {
//            for (int i = 0; i < expArrDim.size(); i++) {
//                if (i != 0 && expArrDim[i] != param.paramDim[i]) {
//                    writeError(line, "e");
//                    break;
//                }
//            }
//        }
        if (quaterions.back().op == QuadOp::arrRef) {
            if (quaterions.back().num2.name == "allRef") {
                cover.setArrRefAllFunCall();
            } else {
                cover.setArrRefPartFunCall();
            }
        }
        quaterions.emplace_back(QuadOp::callTableRow, quaterions.back().result,
                                TableRow("", "", "", to_string(count)));
        SAVE_NOW
        GET_IF_AND_WRITE("COMMA")
        else if (token.symbol != "RPARENT") {
            GO_BACK
            ERROR("j")
            break;
        }
    }
    if (count != globalTable.at(funName).funParams.size()) {
        writeError(line, "d");
    }
    if (count == 1) {
        cover.setOneParaFunCall();
    } else if (count == 2) {
        cover.setManyParaFunCall();
    }
    writeFile("<FuncRParams>");
}

bool Parsing::findAssign() {
    int i = token.index;
    char ch = token.file.at(i);
    while (ch != ';') {
        if (ch == '=') {
            return true;
        } else {
            i++;
            ch = token.file.at(i);
        }
    }
    return false;
}

bool Parsing::isRightFormatChar(char c) {
    return c == 32 || c == 33 || (c >= 40 && c <= 126);
}

/**
 * 将元素在数组的线性位置转换为在数组索引中的下标
 * $param downs
 * $param k
 * $return
 */
string Parsing::getDown(vector <string> downs, int k) {
    k = k - 1;
    int res = 0;
    int modRes = 0;
    string resString = "";
    for (int i = downs.size(); i > 0; i--) {
        resString.insert(0, to_string(k % (stoi(downs.at(i - 1)))) + ",");
        k = k / stoi(downs.at(i - 1));
    }
    return resString.substr(0, resString.length() - 1);
}

/**
 * 由数组的维度和元素的坐标值得到元素在线性空间中的位置，位置从1开始计数
 * $param downs 输入数组每一维的长度
 * $param coefficients 输入元素每一维的坐标
 * $return 返回元素在线性空间中的位置
 */
int Parsing::getCount(vector <string> downs, vector<int> coefficients) {
    int result = 0;
    int number = 1;
    for (int i = downs.size() - 1; i >= 0; i--) {
        result += coefficients.at(i) * number;
        number *= stoi(downs.at(i));
    }
    return result + 1;
}

string Parsing::vectorToString(const vector<int> &a) {
    string sb;
    for (auto i : a) {
        sb += to_string(i) + ",";
    }
    return sb;
}

vector <Quaternion> &Parsing::getQuaterions() {
    return this->quaterions;
}

CoverageChecker& Parsing::getCover() {
    return cover;
}

TableRow Parsing::findAndGet(const string &name) {
    // 从栈式符号表中查找。
    for (auto table : tableStack) {
        if (table->find(name) != table->end()) {
            return table->at(name);
        }
    }
    return TableRow();
}

bool Parsing::getGoodBye() {
    return this->goodBye;
}




