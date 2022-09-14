//
// Created by lsp on 2021/7/21.
//

#include "TableRow.h"

TableRow::TableRow() {

}

TableRow::TableRow(string name) : name(name) {

}

TableRow::TableRow(string name, string type, string symbol) :
        name(name), type(type), symbol(symbol) {

}

void TableRow::clear() {
    isArr = false;
    type.clear();
    name.clear();
    symbol.clear();
    dimensions.clear();
    this->arrValue.clear();
    this->funParams.clear();
    this->value.clear();
}

TableRow::TableRow(string name, string type, string symbol, string value) :
    name(std::move(name)), type(std::move(type)), symbol(std::move(symbol)), value(std::move(value)) {

}
