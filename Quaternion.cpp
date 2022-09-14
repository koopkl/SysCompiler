//
// Created by lsp on 2021/7/27.
//

#include "Quaternion.h"

Quaternion::Quaternion() {

}

Quaternion::Quaternion(QuadOp op, TableRow num1, TableRow num2, TableRow res) :
        op(op), num1(num1), num2(num2), result(res) {

}

Quaternion::Quaternion(QuadOp op, TableRow num1, TableRow num2) : op(op), num1(num1), num2(num2) {

}

Quaternion::Quaternion(QuadOp op, TableRow num1) : op(op), num1(num1) {

}

Quaternion::Quaternion(QuadOp op) : op(op) {

}
