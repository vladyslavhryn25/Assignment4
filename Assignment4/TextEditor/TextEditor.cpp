#include "Line.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<Line*> doc;
    doc.push_back(new TextLine("My student notes"));
    doc.push_back(new ChecklistLine("Prepare agenda", true));
    doc.push_back(new ChecklistLine("Send invitations", false));
    doc.push_back(new ContactLine("Vladyslav", "Hryn", "vladyslav.hryn.25@kse.org.ua"));

    std::cout << "Print" << std::endl;
    for (size_t i = 0; i < doc.size(); i++)
        doc[i]->print();

    std::cout << "\nSerialize" << std::endl;
    std::vector<std::string> saved;
    for (size_t i = 0; i < doc.size(); i++) {
        std::string s = doc[i]->serialize();
        saved.push_back(s);
        std::cout << s << std::endl;
    }

    std::cout << "\nDeserialize back" << std::endl;
    for (size_t i = 0; i < saved.size(); i++) {
        Line* l = Line::deserialize(saved[i]);
        l->print();
        delete l;
    }

    for (size_t i = 0; i < doc.size(); i++)
        delete doc[i];

    return 0;
}