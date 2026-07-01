#include "TextLine.h"
#include <iostream>

TextLine::TextLine(const std::string& t) {
    text = t;
}

void TextLine::print() const {
    std::cout << "Text: " << text << std::endl;
}

std::string TextLine::serialize() const {
    return "T|" + text;
}

char TextLine::type() const {
    return 'T';
}

std::string TextLine::getText() const {
    return text;
}

void TextLine::setText(const std::string& t) {
    text = t;
}
