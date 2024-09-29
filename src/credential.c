#include "credential.h"
#include <stdlib.h>
#include <string.h>

/*
Changed the file extension and now using binary files to
store the credentials
*/

int credential_generate_id() {
  Credential *credentials[MAX_CREDENTIALS];
  int count = credential_list(credentials);

  if (count < 0) {
    // Error occurred in credential_list
    return count; // Return the error code
  }

  int max_id = 0;

  for (int i = 0; i < count; i++) {
    if (credentials[i]->id > max_id) {
      max_id = credentials[i]->id;
    }
    free(credentials[i]); // Free the allocated memory
  }

  return max_id + 1; // Return the next available ID
}

int credential_create(char service[SERVICE_MAX_LENGTH],
                      char password[PASSWORD_MAX_LENGTH]) {
  FILE *file = fopen(FILENAME, "ab+");
  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  Credential new_credential;
  new_credential.id = credential_generate_id(file);
  strncpy(new_credential.service, service, SERVICE_MAX_LENGTH);
  strncpy(new_credential.password, password, PASSWORD_MAX_LENGTH);
  new_credential.service[SERVICE_MAX_LENGTH - 1] = '\0';
  new_credential.password[PASSWORD_MAX_LENGTH - 1] = '\0';

  size_t written = fwrite(&new_credential, sizeof(Credential), 1, file);
  fclose(file);

  if (written != 1) {
    return ERROR_OPENING_FILE;
  }

  return new_credential.id;
}

int credential_destroy(int credential_id) {
  FILE *file = fopen(FILENAME, "rb");
  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  FILE *temp_file = fopen("temp.dat", "wb");
  if (temp_file == NULL) {
    fclose(file);
    return ERROR_OPENING_FILE;
  }

  Credential credential;
  int found = 0;

  while (fread(&credential, sizeof(Credential), 1, file) == 1) {
    if (credential.id != credential_id) {
      fwrite(&credential, sizeof(Credential), 1, temp_file);
    } else {
      found = 1;
    }
  }

  fclose(file);
  fclose(temp_file);

  if (!found) {
    remove("temp.dat");
    return ERROR_CREDENTIAL_NOT_FOUND;
  }

  remove(FILENAME);
  rename("temp.dat", FILENAME);

  return 0;
}

int credential_update_password(int credential_id,
                               char password[PASSWORD_MAX_LENGTH]) {
  FILE *file = fopen(FILENAME, "rb+"); // Open in read/write binary mode
  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  Credential credential;
  int found = 0;

  while (fread(&credential, sizeof(Credential), 1, file) == 1) {
    if (credential.id == credential_id) {
      strncpy(credential.password, password, PASSWORD_MAX_LENGTH);
      credential.password[PASSWORD_MAX_LENGTH - 1] = '\0';
      fseek(file, -sizeof(Credential), SEEK_CUR);
      fwrite(&credential, sizeof(Credential), 1, file);
      found = 1;
      break;
    }
  }

  fclose(file);

  if (!found) {
    return ERROR_CREDENTIAL_NOT_FOUND;
  }

  return 0;
}

int credential_update_service(int credential_id,
                              const char new_service[SERVICE_MAX_LENGTH]) {
  FILE *file = fopen(FILENAME, "rb+"); // Open in read/write binary mode
  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  Credential credential;
  int found = 0;

  while (fread(&credential, sizeof(Credential), 1, file) == 1) {
    if (credential.id == credential_id) {
      strncpy(credential.service, new_service, SERVICE_MAX_LENGTH);
      credential.service[SERVICE_MAX_LENGTH - 1] = '\0'; // Null-terminate
      fseek(file, -sizeof(Credential), SEEK_CUR);
      fwrite(&credential, sizeof(Credential), 1, file);
      found = 1;
      break;
    }
  }

  fclose(file);

  if (!found) {
    return ERROR_CREDENTIAL_NOT_FOUND;
  }

  return 0;
}

/*Helper function to order credentials in credentials_list */
void swap_credentials(Credential **a, Credential **b) {
  Credential *temp = *a;
  *a = *b;
  *b = temp;
}

int credential_list(Credential *destination[MAX_CREDENTIALS]) {
  FILE *file = fopen(FILENAME, "rb");
  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  int count = 0;
  Credential *credential;

  while (count < MAX_CREDENTIALS) {
    credential = (Credential *)malloc(sizeof(Credential));
    if (credential == NULL) {
      fclose(file);
      return ERROR_ALLOCATING_MEMORY;
    }

    if (fread(credential, sizeof(Credential), 1, file) != 1) {
      free(credential);
      break;
    }

    destination[count] = credential;
    count++;
  }

  fclose(file);

  for (int i = 0; i < count; i++) {
    for (int j = 0; j < count; j++) {
      // We'll do the cast to explicitely convert the char to int
      // sort alphabetically
      if ((int)destination[i]->service[i] < (int)destination[j]->service[j]) {
        swap_credentials(&destination[i], &destination[j]);
      }
    }
  }

  return count;
}

int credential_extract_plain() {
  FILE *file = fopen(FILENAME, "rb");
  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  FILE *plain_file = fopen("plain.txt", "w");
  if (plain_file == NULL) {
    fclose(file);
    return ERROR_OPENING_FILE;
  }

  Credential credential;
  while (fread(&credential, sizeof(Credential), 1, file) == 1) {
    fprintf(plain_file, "Service: %s\n", credential.service);
    fprintf(plain_file, "Password: %s\n\n", credential.password);
  }

  fclose(file);
  fclose(plain_file);

  return 0;
}
