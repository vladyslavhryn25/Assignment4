#include "Line.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"

Line* Line::deserialize(const std::string& data) {
    if (data.size() < 2) {
        return new TextLine(data);
    }

    char tag = data[0];
    std::string rest = data.substr(2);

    if (tag == 'T') {
        return new TextLine(rest);
    }
    else if (tag == 'C') {
        int p1 = rest.find('|');
        int p2 = rest.find('|', p1 + 1);

        std::string name = rest.substr(0, p1);
        std::string surname = rest.substr(p1 + 1, p2 - p1 - 1);
        std::string email = rest.substr(p2 + 1);

        return new ContactLine(name, surname, email);
    }
    else if (tag == 'K') {
        int p1 = rest.find('|');

        std::string flag = rest.substr(0, p1);
        std::string item = rest.substr(p1 + 1);

        bool checked = (flag == "1");
        return new ChecklistLine(item, checked);
    }

    return new TextLine(data);
}
