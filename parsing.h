#pragma once
#include "Token.h"
#include "CoverageChecker.h"
#include "TableRow.h"
#include "Quaternion.h"

class Parsing {
public:
    Parsing(Token &token);
    void parsingProgram();
    vector<Quaternion>& getQuaterions();
    CoverageChecker& getCover();
    bool getGoodBye();
    static string getDown(vector<string> downs, int k);
    static int getCount(vector<string> downs, vector<int> coefficients);
    static string vectorToString(const vector<int>& a);

private:
    int tempIndex;          // record index for GO_BACK
    int tempLine;           // record line for GO_BACK
    int ifCount = 0;        // to gen different if label
    int whileCount = 0;     // to gen different while label
    int lorCount = 0;       // to gen different lorLabel
    int isHasReturn = 0;    // to mark that is it have Return stmt
    int tempCount = 0;      // to gen different tempVar
    int arrValueCount = 0;  // mutli array initValue subscript value
    bool whereInExp = true; // true -> exp ; false -> lOrExp;
    int isInLoop = 0;       // to mark that is it in loop
    int isHasFun = 0;  // to mark Decl End;
    bool isInMain = false;  // to mark now Fun is Main
    bool goodBye = false;
    CoverageChecker cover;
    vector<Quaternion> quaterions; //
    vector<int> arrCoors;
    Token token;
    TableRow tempRow = TableRow();
    TableRow funTempRow = TableRow();
    map<string, TableRow> globalTable = map<string, TableRow>();
    map <string, TableRow>* currTable = &globalTable;
    vector<map<string, TableRow>*> tableStack {&globalTable};

    void goBack(int index, int lineNum);
    void getNext();

    void funcDef();// function define
    void funcParams();
    void funcParam();
    void block(bool isFun);
    void blockItem();
    void constDecl();   // const declaration
    void constDef();
    void constInitVal(int dimCount);

    void varDef();
    void varDecl();     // variable declaration

    void initVal(int dimCount);
    void stmt();
    void lval(bool isAssign);
    void ifStmt();
    void whileStmt();
    void returnStmt();
    void cond(QuadOp op);
    void formatStr();
    void exp();
    void addExp();
    void constExp();
    void mulExp();
    void relExp();
    void eqExp();
    void lAndExp(QuadOp op);
    void lorExp(QuadOp op);
    void unaryExp();
   // void unaryOp();
    void funcRParams(const string& funName, int line, Quaternion quad);
    // bool is
    bool findAssign();

    static bool isRightFormatChar(char c);

    TableRow findAndGet(const string& name);

};


