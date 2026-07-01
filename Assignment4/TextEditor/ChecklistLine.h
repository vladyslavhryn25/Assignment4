#pragma once
#include "Line.h"
#include <string>

class ChecklistLine : public Line {
    std::string item;
    bool checked;
public:
    ChecklistLine(const std::string& i, bool c);

    void print() const override;
    std::string serialize() const override;
    char type() const override;

    void toggle();
};
