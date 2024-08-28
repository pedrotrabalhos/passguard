#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#define SERVICE_MAX_LENGTH 32
#define PASSWORD_MAX_LENGTH 32
#define MAX_CREDENTIALS 10

typedef struct Credential
{
  int id;
  char service[SERVICE_MAX_LENGTH];
  char password[PASSWORD_MAX_LENGTH];
} Credential;

void create_credential(Credential *credential);

void update_credential(Credential *credential);

void delete_credential(int id);
/*
 * Fetch all the credentials and place them in the credentials array.
 */
int list_credentials(Credential credentials[MAX_CREDENTIALS]);

#endif