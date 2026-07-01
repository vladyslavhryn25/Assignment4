#pragma once
#include <string>

class Line {
public:
    virtual ~Line() {}

    virtual void print() const = 0;
    virtual std::string serialize() const = 0;
    virtual char type() const = 0;

    static Line* deserialize(const std::string& data);
};
