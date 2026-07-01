#include "Cipher.h"

Cipher::Cipher(const std::string& dllPath) {
    create_caesar = nullptr;
    create_vigenere = nullptr;
    encrypt_fn = nullptr;
    decrypt_fn = nullptr;
    destroy_fn = nullptr;
    free_fn = nullptr;

    dll = LoadLibraryA(dllPath.c_str());

    if (dll != nullptr) {
        create_caesar = (create_caesar_t)GetProcAddress(dll, "cipher_create_caesar");
        create_vigenere = (create_vigenere_t)GetProcAddress(dll, "cipher_create_vigenere");
        encrypt_fn = (encrypt_t)GetProcAddress(dll, "cipher_encrypt");
        decrypt_fn = (decrypt_t)GetProcAddress(dll, "cipher_decrypt");
        destroy_fn = (destroy_t)GetProcAddress(dll, "cipher_destroy");
        free_fn = (free_t)GetProcAddress(dll, "cipher_free");
    }
}

Cipher::~Cipher() {
    if (dll != nullptr) {
        FreeLibrary(dll);
    }
}

bool Cipher::isLoaded() const {
    if (dll == nullptr) return false;
    if (create_caesar == nullptr) return false;
    if (create_vigenere == nullptr) return false;
    if (encrypt_fn == nullptr) return false;
    if (decrypt_fn == nullptr) return false;
    if (destroy_fn == nullptr) return false;
    if (free_fn == nullptr) return false;
    return true;
}

std::string Cipher::caesarEncrypt(const std::string& text, int key) {
    void* c = create_caesar(key);
    char* res = encrypt_fn(c, text.c_str());
    std::string result = res;
    free_fn(res);
    destroy_fn(c);
    return result;
}

std::string Cipher::caesarDecrypt(const std::string& text, int key) {
    void* c = create_caesar(key);
    char* res = decrypt_fn(c, text.c_str());
    std::string result = res;
    free_fn(res);
    destroy_fn(c);
    return result;
}

std::string Cipher::vigenereEncrypt(const std::string& text, const std::string& key) {
    void* c = create_vigenere(key.c_str());
    char* res = encrypt_fn(c, text.c_str());
    std::string result = res;
    free_fn(res);
    destroy_fn(c);
    return result;
}

std::string Cipher::vigenereDecrypt(const std::string& text, const std::string& key) {
    void* c = create_vigenere(key.c_str());
    char* res = decrypt_fn(c, text.c_str());
    std::string result = res;
    free_fn(res);
    destroy_fn(c);
    return result;
}
