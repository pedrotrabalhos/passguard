#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include <stdio.h>

#define FILENAME "credentials.csv"
#define MAX_CREDENTIALS 5

#define ERROR_OPENING_FILE -1
#define ERROR_ALLOCATING_MEMORY -2
#define ERROR_CREDENTIAL_NOT_FOUND -3

#define MAX_LINE_LENGTH 256
#define SERVICE_MAX_LENGTH 50
#define PASSWORD_MAX_LENGTH 32

typedef struct Credential {
  int id;
  char service[SERVICE_MAX_LENGTH];
  char password[PASSWORD_MAX_LENGTH];
} Credential;

int credential_generate_id(FILE *file);

int credential_create(char service[SERVICE_MAX_LENGTH],
                      char password[PASSWORD_MAX_LENGTH]);

int credential_destroy(int credential_id);

int credential_update_password(int credential_id,
                               char password[PASSWORD_MAX_LENGTH]);

int credential_update_service(int credential_id,
                              const char new_service[SERVICE_MAX_LENGTH]);

int credential_list(Credential *destination[MAX_CREDENTIALS]);

#endif // CREDENTIAL_H
