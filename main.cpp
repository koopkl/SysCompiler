#include <algorithm>
#include <sstream>
#include "Token.h"
#include "parsing.h"
#include "getSym.h"
#include "GenPcode.h"
#include "Interpreter.h"
#include "CoverageChecker.h"

int main() {
    std::ofstream coverage("cover.txt");
    std::ifstream in("testfile.txt");
    std::ofstream pcoderesult("pcoderesult.txt");
    std::stringstream buffer;
    buffer << in.rdbuf();
    Token token(buffer.str());
    Parsing parser = Parsing(token);
    bool error1 = false;
    try {
        parser.parsingProgram();
    } catch (exception& e) {
        cout << e.what() << endl;
        error1 = true;
    }
    if (parser.getGoodBye()) {
        cout << "parsing error!!!" << endl;
    } else if (!isHasError && !error1) {

        parser.getCover().showResult(coverage);
        GenPcode genPcode = GenPcode(parser.getQuaterions());
        try {
            genPcode.genPcode();
        } catch (exception &e) {
            cout << "generate Pcode Error!!!" << endl;
            cout << e.what() << endl;
        }
        try {
            Interpreter test = Interpreter(genPcode.getInstr());
            test.runPcode(pcoderesult);
        } catch (exception &e) {
            cout << "run Pcode Error!!!" << endl;
            cout << e.what() << endl;
        }

    }
    os.close();
    in.close();
    error.close();
    pcoderesult.close();
    return 0;
}


