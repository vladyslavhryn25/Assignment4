#include "Text.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"
#include <iostream>

int main() {
    Text doc;
    doc.addLine(new TextLine("My student notes"));
    doc.addLine(new ChecklistLine("Make assignment 4", true));
    doc.addLine(new ChecklistLine("Be happy", false));
    doc.addLine(new ContactLine("Vladyslav", "Hryn", "vladyslav.hryn.25@kse.org.ua"));

    std::cout << "All lines" << std::endl;
    doc.printAll();

    std::cout << "\nTotal lines: " << doc.size() << std::endl;

    std::cout << "\nInsert in 1 position" << std::endl;
    doc.insertLine(1, new TextLine("Inserted line"));
    doc.printAll();

    std::cout << "\nRemove line 0" << std::endl;
    doc.removeLine(0);
    doc.printAll();

    std::cout << "\nSerealize all" << std::endl;
    std::vector<std::string> data = doc.serializeAll();
    for (size_t i = 0; i < data.size(); i++)
        std::cout << data[i] << std::endl;

    return 0;
}