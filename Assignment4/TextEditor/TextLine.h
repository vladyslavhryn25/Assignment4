#pragma once
#include "Line.h"
#include <string>

class TextLine : public Line {
    std::string text;
public:
    TextLine(const std::string& t);

    void print() const override;
    std::string serialize() const override;
    char type() const override;

    std::string getText() const;
    void setText(const std::string& t);
};
