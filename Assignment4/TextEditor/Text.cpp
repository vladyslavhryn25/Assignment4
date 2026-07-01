#include "Text.h"
#include <iostream>

Text::Text() {
}

Text::~Text() {
    clear();
}

void Text::addLine(Line* line) {
    lines.push_back(line);
}

void Text::insertLine(size_t index, Line* line) {
    if (index > lines.size())
        index = lines.size();
    lines.insert(lines.begin() + index, line);
}

void Text::removeLine(size_t index) {
    if (index >= lines.size())
        return;
    delete lines[index];
    lines.erase(lines.begin() + index);
}

Line* Text::getLine(size_t index) const {
    if (index >= lines.size())
        return nullptr;
    return lines[index];
}

size_t Text::size() const {
    return lines.size();
}

void Text::printAll() const {
    for (size_t i = 0; i < lines.size(); i++)
        lines[i]->print();
}

std::vector<std::string> Text::serializeAll() const {
    std::vector<std::string> result;
    for (size_t i = 0; i < lines.size(); i++)
        result.push_back(lines[i]->serialize());
    return result;
}

void Text::clear() {
    for (size_t i = 0; i < lines.size(); i++)
        delete lines[i];
    lines.clear();
}
