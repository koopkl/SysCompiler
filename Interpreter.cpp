

#include <fstream>
#include "Interpreter.h"

Interpreter::Interpreter() {

}

Interpreter::Interpreter(vector <PcodeInstr> &instrIn) {
    instr = instrIn;
    stack[T] = "0";
    stack[T + 1] = "0";
    stack[T + 2] = "0";
    T += 3;
}

int Interpreter::getSl(int B, int level) {
    while (level > 0) {
        B = stoi(stack[B]);
        level--;
    }
    return B;
}

void Interpreter::caluate() {

}

void Interpreter::runPcode(std::ofstream &pcoderesult) {
    do {
        PcodeInstr tempInstr = instr[P];
        int level, address;
        if (tempInstr.getOp() != PcodeOp::PRT) {
            level = tempInstr.getLevel().empty() ? 0 : stoi(tempInstr.getLevel());
            address = tempInstr.getAddress().empty() ? 0 : stoi(tempInstr.getAddress());
        }
        switch (tempInstr.getOp()) {
            case PcodeOp::LIT:
                calStack[calT++] = tempInstr.getAddress();
                break;
            case PcodeOp::LOD:
                calStack[calT++] = stack[getSl(B, level) + address];
                break;
            case PcodeOp::STO:
                calT--;
                stack[getSl(B, level) + address] = calStack[calT];
                break;
            case PcodeOp::CAL:
                stack[T - 3] = to_string(B);
                stack[T - 2] = to_string(B);
                B = T - 3;
                calT = 0;
                break;
            case PcodeOp::INT: {
                int length = stoi(tempInstr.getAddress());
                for (int i = 0; i < length; i++) {
                    stack[T + i] = "0";
                }
                T += length;
                break;
            }
            case PcodeOp::JMP:
                P = address;
                if (!tempInstr.getLevel().empty()) {
                    T = getSl(B, level - 1);
                    B = getSl(B, level);

                }
                continue;
            case PcodeOp::JTP:
                if (stoi(calStack[calT - 1]) == 1) {
                    P = address;
                    continue;
                }
                break;
            case PcodeOp::JPCAND:
                if (stoi(calStack[calT - 1]) == 0) {
                    P = address;
                    continue;
                }
                break;
            case PcodeOp::JPC:
                calT--;
                if (stoi(calStack[calT]) == 0) {
                    P = address;
                    continue;
                }
                break;
            case PcodeOp::LODS: {
                int i = stoi(calStack[calT - 1]);
                calStack[calT - 1] = stack[i];
            }
                break;
            case PcodeOp::STOS: {
                calT--;
                int i = stoi(calStack[calT]);
                calT--;
                stack[i] = calStack[calT];
            }
                break;
            case PcodeOp::LA:
                calStack[calT++] = to_string(B);
                break;
            case PcodeOp::PRT:
                pcoderesult << tempInstr.getAddress();
                break;
            case PcodeOp::BLO:
                stack[T] = to_string(B);
                stack[T + 1] = stack[B + 1];
                stack[T + 2] = stack[B + 2];
                B = T;
                T += 3;
                break;
            case PcodeOp::BLE: {
                int i = B;
                B = stoi(stack[B]);
                T = i;
            }
                break;
            case PcodeOp::OPR:
                switch (stoi(tempInstr.getAddress())) {
                    case 0: {
                        int tempB = B;
                        P = stoi(stack[tempB + 2]);
                        B = stoi(stack[tempB + 1]);
                        T = tempB;
                        continue;
                    }
                    case 1:
                        calStack[calT - 1] = to_string((!stoi(calStack[calT - 1])));
                        break;
                    case 2: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j + i));
                        calT--;
                    }
                        break;
                    case 3: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j - i));
                        calT--;
                    }
                        break;
                    case 4: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j * i));
                        calT--;
                    }

                        break;
                    case 5: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j / i));
                        calT--;
                    }
                        break;
                    case 6: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j % i));
                        calT--;
                    }
                        break;
                    case 7:
                        break;
                    case 8: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j == i));
                        calT--;
                    }
                        break;
                    case 9: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j != i));
                        calT--;
                    }
                        break;
                    case 10: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j < i));
                        calT--;
                    }
                        break;
                    case 11: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j >= i));
                        calT--;
                    }
                        break;
                    case 12: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j > i));
                        calT--;
                    }
                        break;
                    case 13: {
                        int i, j;
                        i = stoi(calStack[calT - 1]);
                        j = stoi(calStack[calT - 2]);
                        calStack[calT - 2] = (to_string(j <= i));
                        calT--;
                    }
                        break;
                    case 14:
                        calT--;
                        pcoderesult << calStack[calT];
                        break;
                    case 15:
                        pcoderesult << "\n";
                        break;
                    case 16: {
                        int i;
                        cin >> i;
                        calStack[calT] = to_string(i);
                        calT++;
                    }
                        break;
                    case 17:
                        break;
                }
                break;

        }
        P++;
    } while (P != 0);
}


