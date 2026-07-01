#include "CommandLineInterface.h"
#include "TextLine.h"
#include "ContactLine.h"
#include "ChecklistLine.h"
#include "Cipher.h"
#include <iostream>
#include <fstream>
#include <string>

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
        } else {
            current += text[i];
        }
    }
    result.push_back(current);
    return result;
}

void CommandLineInterface::addTextLine() {
    std::cout << "Enter text: ";
    std::string t;
    std::getline(std::cin, t);
    doc.addLine(new TextLine(t));
}

void CommandLineInterface::addContact() {
    std::string name, surname, email;
    std::cout << "Name: ";    std::getline(std::cin, name);
    std::cout << "Surname: "; std::getline(std::cin, surname);
    std::cout << "E-mail: ";  std::getline(std::cin, email);
    doc.addLine(new ContactLine(name, surname, email));
}

void CommandLineInterface::addChecklistItem() {
    std::cout << "Item text: ";
    std::string item;
    std::getline(std::cin, item);
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
    std::cout << "Line number to remove (from 0): ";
    std::string num;
    std::getline(std::cin, num);
    size_t index = (size_t)std::stoi(num);
    doc.removeLine(index);
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

    std::string type;
    std::string key = askCipherKey(type);

    std::string plain = joinLines(doc.serializeAll());

    Cipher cipher("CipherLib.dll");
    if (!cipher.isLoaded()) {
        std::cout << "Error: cannot load CipherLib.dll" << std::endl;
        return;
    }

    std::string encrypted;
    if (type == "caesar")
        encrypted = cipher.caesarEncrypt(plain, std::stoi(key));
    else
        encrypted = cipher.vigenereEncrypt(plain, key);

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
    if (type == "caesar")
        decrypted = cipher.caesarDecrypt(encrypted, std::stoi(key));
    else
        decrypted = cipher.vigenereDecrypt(encrypted, key);

    doc.clear();
    std::vector<std::string> lines = splitLines(decrypted);
    for (size_t i = 0; i < lines.size(); i++)
        doc.addLine(Line::deserialize(lines[i]));

    std::cout << "Loaded and decrypted:" << std::endl;
    printAll();
}

void CommandLineInterface::run() {
    while (true) {
        std::cout << "\My text editor" << std::endl;
        std::cout << "1. Add text line" << std::endl;
        std::cout << "2. Add contact" << std::endl;
        std::cout << "3. Add checklist item" << std::endl;
        std::cout << "4. Print all" << std::endl;
        std::cout << "5. Remove line" << std::endl;
        std::cout << "6. Save encrypted to file" << std::endl;
        std::cout << "7. Load and decrypt from file" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Choose: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") addTextLine();
        else if (choice == "2") addContact();
        else if (choice == "3") addChecklistItem();
        else if (choice == "4") printAll();
        else if (choice == "5") removeLine();
        else if (choice == "6") saveEncrypted();
        else if (choice == "7") loadDecrypted();
        else if (choice == "0") break;
        else std::cout << "Unknown option" << std::endl;
    }
}
