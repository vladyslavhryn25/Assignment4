#include "cipher.h"
#include "cipher_api.h"
#include <cctype>
#include <cstring>

CaesarCipher::CaesarCipher(int key) : key_(key) {}

std::string CaesarCipher::encrypt(const std::string& text) {
    std::string result = text;
    for (char& c : result) {
        if (std::isalpha((unsigned char)c)) {
            char base = std::islower((unsigned char)c) ? 'a' : 'A';
            c = base + (c - base + key_ % 26 + 26) % 26;
        }
    }
    return result;
}

std::string CaesarCipher::decrypt(const std::string& text) {
    CaesarCipher reverse(-key_);
    return reverse.encrypt(text);
}

VigenereCipher::VigenereCipher(const std::string& key) {
    for (char c : key) {
        if (std::isalpha((unsigned char)c)) {
            key_ += std::toupper((unsigned char)c);
        }
    }
}

std::string VigenereCipher::encrypt(const std::string& text) {
    std::string result = text;
    size_t ki = 0;
    for (char& c : result) {
        if (std::isalpha((unsigned char)c)) {
            char base = std::islower((unsigned char)c) ? 'a' : 'A';
            int shift = key_[ki % key_.size()] - 'A';
            c = base + (c - base + shift) % 26;
            ++ki;
        }
    }
    return result;
}

std::string VigenereCipher::decrypt(const std::string& text) {
    std::string result = text;
    size_t ki = 0;
    for (char& c : result) {
        if (std::isalpha((unsigned char)c)) {
            char base = std::islower((unsigned char)c) ? 'a' : 'A';
            int shift = key_[ki % key_.size()] - 'A';
            c = base + (c - base - shift + 26) % 26;
            ++ki;
        }
    }
    return result;
}

extern "C" {

cipher_t cipher_create_caesar(int key) {
    return new CaesarCipher(key);
}

cipher_t cipher_create_vigenere(const char* key) {
    return new VigenereCipher(std::string(key));
}

char* cipher_encrypt(cipher_t cipher, const char* text) {
    Cipher* c = (Cipher*)cipher;
    std::string result = c->encrypt(std::string(text));
    char* out = new char[result.size() + 1];
    std::memcpy(out, result.c_str(), result.size() + 1);
    return out;
}

char* cipher_decrypt(cipher_t cipher, const char* text) {
    Cipher* c = (Cipher*)cipher;
    std::string result = c->decrypt(std::string(text));
    char* out = new char[result.size() + 1];
    std::memcpy(out, result.c_str(), result.size() + 1);
    return out;
}

void cipher_destroy(cipher_t cipher) {
    Cipher* c = (Cipher*)cipher;
    delete c;
}

void cipher_free(char* str) {
    delete[] str;
}

}
