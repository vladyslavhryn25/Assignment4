#pragma once
#include "Line.h"
#include <vector>

class Text {
    std::vector<Line*> lines;
public:
    Text();
    ~Text();

    void addLine(Line* line);
    void insertLine(size_t index, Line* line);
    void removeLine(size_t index);
    Line* getLine(size_t index) const;
    size_t size() const;

    void printAll() const;
    std::vector<std::string> serializeAll() const;
    void clear();
};
