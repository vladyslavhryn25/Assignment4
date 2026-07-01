#pragma once
#include <string>
#include <windows.h>

class Cipher {
    HMODULE dll;

    typedef void* (*create_caesar_t)(int);
    typedef void* (*create_vigenere_t)(const char*);
    typedef char* (*encrypt_t)(void*, const char*);
    typedef char* (*decrypt_t)(void*, const char*);
    typedef void (*destroy_t)(void*);
    typedef void (*free_t)(char*);

    create_caesar_t create_caesar;
    create_vigenere_t create_vigenere;
    encrypt_t encrypt_fn;
    decrypt_t decrypt_fn;
    destroy_t destroy_fn;
    free_t free_fn;

public:
    Cipher(const std::string& dllPath);
    ~Cipher();

    bool isLoaded() const;

    std::string caesarEncrypt(const std::string& text, int key);
    std::string caesarDecrypt(const std::string& text, int key);
    std::string vigenereEncrypt(const std::string& text, const std::string& key);
    std::string vigenereDecrypt(const std::string& text, const std::string& key);
};
