#pragma once
#include "Text.h"
#include <string>

class CommandLineInterface {
    Text doc;

    void addTextLine();
    void addContact();
    void addChecklistItem();
    void printAll();
    void removeLine();
    void toggleChecklist();
    void saveEncrypted();
    void loadDecrypted();

    std::string askCipherKey(std::string& outType);

public:
    void run();
};
