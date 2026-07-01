#pragma once
#include "Line.h"
#include <string>

class ContactLine : public Line {
    std::string name;
    std::string surname;
    std::string email;
public:
    ContactLine(const std::string& n, const std::string& s, const std::string& e);

    void print() const override;
    std::string serialize() const override;
    char type() const override;
};
