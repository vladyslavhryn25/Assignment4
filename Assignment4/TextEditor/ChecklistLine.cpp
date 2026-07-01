#include "ChecklistLine.h"
#include <iostream>

ChecklistLine::ChecklistLine(const std::string& i, bool c) {
    item = i;
    checked = c;
}

void ChecklistLine::print() const {
    if (checked)
        std::cout << "[ x ] " << item << std::endl;
    else
        std::cout << "[   ] " << item << std::endl;
}

std::string ChecklistLine::serialize() const {
    std::string flag;
    if (checked)
        flag = "1";
    else
        flag = "0";
    return "K|" + flag + "|" + item;
}

char ChecklistLine::type() const {
    return 'K';
}

void ChecklistLine::toggle() {
    checked = !checked;
}
