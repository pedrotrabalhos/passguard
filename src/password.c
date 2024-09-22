#include "password.h"
#include <stdlib.h>
#include <string.h>

void password_encrypt(char *raw_password, char *destination, const char *key) {
  size_t password_len = strlen(raw_password);
  size_t key_len = strlen(key);

  for (size_t i = 0; i < password_len; i++) {
    destination[i] = raw_password[i] ^ key[i % key_len];
  }
}

void password_decrypt(char *encrypted, char *destination, const char *key) {
  password_encrypt(encrypted, destination, key);
}

void password_generate(char *destination) {
  const char charset[] =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  size_t charset_len = strlen(charset);

  destination = malloc(16 * sizeof(char));

  for (size_t i = 0; i < 16; i++) {
    destination[i] = charset[rand() % charset_len];
  }
}
