//
// Created by lsp on 2021/7/31.
//

#include "PcodeInstr.h"


PcodeInstr::PcodeInstr() {

}

PcodeInstr::PcodeInstr(PcodeOp op, string level, string address) :
op(op), level(std::move(level)), address(std::move(address)) {

}

string PcodeInstr::toString() {
    string result;
    switch (op) {
        case PcodeOp::LIT:
            result += "LIT";
            break;
        case PcodeOp::OPR:
            result += "OPR";
            break;
        case PcodeOp::LOD:
            result += "LOD";
            break;
        case PcodeOp::STO:
            result += "STO";
            break;
        case PcodeOp::CAL:
            result += "CAL";
            break;
        case PcodeOp::INT:
            result += "INT";
            break;
        case PcodeOp::JMP:
            result += "JMP";
            break;
        case PcodeOp::JPC:
            result += "JPC";
            break;
        case PcodeOp::JTP:
            result += "JTP";
            break;
        case PcodeOp::LODS:
            result += "LODS";
            break;
        case PcodeOp::LA:
            result += "LA";
            break;
        case PcodeOp::PRT:
            result += "PRT";
            break;
        case PcodeOp::BLO:
            result += "BLO";
            break;
        case PcodeOp::BLE:
            result += "BLE";
            break;
        case PcodeOp::JPCAND:
            result += "JPCAND";
            break;
        case PcodeOp::STOS:
            result += "STOS";
            break;
    }
    return result + " " + level + ", " + address;
}

PcodeOp PcodeInstr::getOp() {
    return op;
}

void PcodeInstr::setAddress(string addr) {
    this->address = addr;
}

string PcodeInstr::getAddress() {
    return this->address;
}

string PcodeInstr::getLevel() {
    return level;
}


