#ifndef PASSWORD_H
#define PASSWORD_H

#define MAX_PASSWORD_LENGTH 32 

void encode_password(char raw_password[MAX_PASSWORD_LENGTH]);

void decode_password(char password_hash[MAX_PASSWORD_LENGTH]);

void generate_password(int password_length, char destination[]);

#endif
