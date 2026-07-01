#include "CommandLineInterface.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"
#include "Cipher.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

static std::string joinLines(const std::vector<std::string>& lines) {
    std::string all;
    for (size_t i = 0; i < lines.size(); i++) {
        all += lines[i];
        if (i + 1 < lines.size())
            all += "\n";
    }
    return all;
}

static std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> result;
    std::string current;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '\n') {
            result.push_back(current);
            current = "";
        }
        else {
            current += text[i];
        }
    }
    result.push_back(current);
    return result;
}

static bool parseInt(const std::string& s, int& out) {
    if (s.empty())
        return false;
    size_t start = 0;
    if (s[0] == '-' || s[0] == '+')
        start = 1;
    if (start == s.size())
        return false;
    for (size_t i = start; i < s.size(); i++)
        if (!std::isdigit((unsigned char)s[i]))
            return false;
    out = std::stoi(s);
    return true;
}

static bool isValidLine(const std::string& s) {
    if (s.size() < 2)
        return false;
    char t = s[0];
    if (t != 'T' && t != 'C' && t != 'K')
        return false;
    if (s[1] != '|')
        return false;
    return true;
}

void CommandLineInterface::addTextLine() {
    std::cout << "Enter text: ";
    std::string t;
    std::getline(std::cin, t);
    if (t.empty()) {
        std::cout << "Error: text cannot be empty." << std::endl;
        return;
    }
    doc.addLine(new TextLine(t));
}

void CommandLineInterface::addContact() {
    std::string name, surname, email;
    std::cout << "Name: ";    std::getline(std::cin, name);
    std::cout << "Surname: "; std::getline(std::cin, surname);
    std::cout << "E-mail: ";  std::getline(std::cin, email);
    if (name.empty() || email.empty()) {
        std::cout << "Error: name and e-mail cannot be empty." << std::endl;
        return;
    }
    doc.addLine(new ContactLine(name, surname, email));
}

void CommandLineInterface::addChecklistItem() {
    std::cout << "Item text: ";
    std::string item;
    std::getline(std::cin, item);
    if (item.empty()) {
        std::cout << "Error: item text cannot be empty." << std::endl;
        return;
    }
    std::cout << "Checked? (1 = yes, 0 = no): ";
    std::string flag;
    std::getline(std::cin, flag);
    bool checked = (flag == "1");
    doc.addLine(new ChecklistLine(item, checked));
}

void CommandLineInterface::printAll() {
    std::cout << "\nOutput:" << std::endl;
    doc.printAll();
}

void CommandLineInterface::removeLine() {
    if (doc.size() == 0) {
        std::cout << "Error: document is empty." << std::endl;
        return;
    }
    std::cout << "Line number to remove (from 0): ";
    std::string num;
    std::getline(std::cin, num);
    int index;
    if (!parseInt(num, index) || index < 0 || (size_t)index >= doc.size()) {
        std::cout << "Error: invalid line number." << std::endl;
        return;
    }
    doc.removeLine((size_t)index);
}

void CommandLineInterface::toggleChecklist() {
    if (doc.size() == 0) {
        std::cout << "Error: document is empty." << std::endl;
        return;
    }
    std::cout << "Checklist line number to toggle (from 0): ";
    std::string num;
    std::getline(std::cin, num);
    int index;
    if (!parseInt(num, index) || index < 0 || (size_t)index >= doc.size()) {
        std::cout << "Error: invalid line number." << std::endl;
        return;
    }
    Line* line = doc.getLine((size_t)index);
    if (line->type() != 'K') {
        std::cout << "Error: this line is not a checklist item." << std::endl;
        return;
    }
    ChecklistLine* cl = (ChecklistLine*)line;
    cl->toggle();
    std::cout << "Status toggled." << std::endl;
}

std::string CommandLineInterface::askCipherKey(std::string& outType) {
    std::cout << "Cipher (1 = Caesar, 2 = Vigenere): ";
    std::string choice;
    std::getline(std::cin, choice);
    if (choice == "2")
        outType = "vigenere";
    else
        outType = "caesar";

    std::cout << "Enter key: ";
    std::string key;
    std::getline(std::cin, key);
    return key;
}

void CommandLineInterface::saveEncrypted() {
    std::cout << "Output file path: ";
    std::string path;
    std::getline(std::cin, path);
    if (path.empty()) {
        std::cout << "Error: file path cannot be empty." << std::endl;
        return;
    }

    std::string type;
    std::string key = askCipherKey(type);

    std::string plain = joinLines(doc.serializeAll());

    Cipher cipher("CipherLib.dll");
    if (!cipher.isLoaded()) {
        std::cout << "Error: cannot load CipherLib.dll" << std::endl;
        return;
    }

    std::string encrypted;
    if (type == "caesar") {
        int k;
        if (!parseInt(key, k)) {
            std::cout << "Warning: key is not a number, using 0." << std::endl;
            k = 0;
        }
        encrypted = cipher.caesarEncrypt(plain, k);
    }
    else {
        encrypted = cipher.vigenereEncrypt(plain, key);
    }

    std::ofstream out(path);
    if (!out) {
        std::cout << "Error: cannot open file for writing" << std::endl;
        return;
    }
    out << encrypted;
    out.close();
    std::cout << "Saved encrypted text to " << path << std::endl;
}

void CommandLineInterface::loadDecrypted() {
    std::cout << "Input file path: ";
    std::string path;
    std::getline(std::cin, path);

    std::ifstream in(path);
    if (!in) {
        std::cout << "Error: cannot open file for reading" << std::endl;
        return;
    }
    std::string encrypted, line;
    bool first = true;
    while (std::getline(in, line)) {
        if (!first) encrypted += "\n";
        encrypted += line;
        first = false;
    }
    in.close();

    std::string type;
    std::string key = askCipherKey(type);

    Cipher cipher("CipherLib.dll");
    if (!cipher.isLoaded()) {
        std::cout << "Error: cannot load CipherLib.dll" << std::endl;
        return;
    }

    std::string decrypted;
    if (type == "caesar") {
        int k;
        if (!parseInt(key, k)) {
            std::cout << "Warning: key is not a number, using 0." << std::endl;
            k = 0;
        }
        decrypted = cipher.caesarDecrypt(encrypted, k);
    }
    else {
        decrypted = cipher.vigenereDecrypt(encrypted, key);
    }

    if (decrypted.empty()) {
        doc.clear();
        std::cout << "File is empty. Loaded empty document." << std::endl;
        return;
    }

    std::vector<std::string> lines = splitLines(decrypted);

    for (size_t i = 0; i < lines.size(); i++) {
        if (!isValidLine(lines[i])) {
            std::cout << "Error: file is corrupted or wrong key was used." << std::endl;
            std::cout << "Document was not changed." << std::endl;
            return;
        }
    }

    doc.clear();
    for (size_t i = 0; i < lines.size(); i++)
        doc.addLine(Line::deserialize(lines[i]));

    std::cout << "Loaded and decrypted:" << std::endl;
    printAll();
}

void CommandLineInterface::run() {
    while (true) {
        std::cout << "\nMy text editor" << std::endl;
        std::cout << "1. Add text line" << std::endl;
        std::cout << "2. Add contact" << std::endl;
        std::cout << "3. Add checklist item" << std::endl;
        std::cout << "4. Print all" << std::endl;
        std::cout << "5. Remove line" << std::endl;
        std::cout << "6. Toggle checklist item" << std::endl;
        std::cout << "7. Save encrypted to file" << std::endl;
        std::cout << "8. Load and decrypt from file" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Choose: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") addTextLine();
        else if (choice == "2") addContact();
        else if (choice == "3") addChecklistItem();
        else if (choice == "4") printAll();
        else if (choice == "5") removeLine();
        else if (choice == "6") toggleChecklist();
        else if (choice == "7") saveEncrypted();
        else if (choice == "8") loadDecrypted();
        else if (choice == "0") break;
        else std::cout << "Unknown option" << std::endl;
    }
}