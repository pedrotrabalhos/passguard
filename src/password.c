#include "password.h"
#include <stdlib.h>

void password_generate(char *destination) {
  const char charset[] =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  const size_t charset_len = sizeof(charset) - 1;

  for (size_t i = 0; i < 16; i++) {
    destination[i] = charset[rand() % charset_len];
  }
  destination[16] = '\0';
}
