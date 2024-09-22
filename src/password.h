#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdlib.h>
#include <string.h>

void password_encrypt(char *raw_password, char *destination, const char *key);

void password_decrypt(char *encrypted, char *destination, const char *key);

void password_generate(char *destination);

#endif // PASSWORD_H
