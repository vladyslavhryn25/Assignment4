#include "ContactLine.h"
#include <iostream>

ContactLine::ContactLine(const std::string& n, const std::string& s, const std::string& e) {
    name = n;
    surname = s;
    email = e;
}

void ContactLine::print() const {
    std::cout << "Contact - " << name << " " << surname
              << ", E-mail: " << email << std::endl;
}

std::string ContactLine::serialize() const {
    return "C|" + name + "|" + surname + "|" + email;
}

char ContactLine::type() const {
    return 'C';
}
