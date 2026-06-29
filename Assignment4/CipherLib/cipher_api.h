#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	typedef void* cipher_t;

#define EXPORT __declspec(dllexport)

	EXPORT cipher_t cipher_create_caesar(int key);
	EXPORT cipher_t cipher_create_vigenere(const char* key);

	EXPORT char* cipher_encrypt(cipher_t cipher, const char* text);
	EXPORT char* cipher_decrypt(cipher_t cipher, const char* text);

	EXPORT void cipher_destroy(cipher_t cipher);
	EXPORT void cipher_free(char* str);

#ifdef __cplusplus
}
#endif