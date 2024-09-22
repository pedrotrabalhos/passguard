#include "credential.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int credential_generate_id(FILE *file) {
  int highest_id = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, sizeof(line), file) != NULL) {
    int current_id;

    if (sscanf(line, "%d,", &current_id) == 1) {
      if (current_id > highest_id) {
        highest_id = current_id;
      }
    }
  }

  // Get back to the beginning of the file
  rewind(file);
  return highest_id + 1;
}

int credential_create(char service[SERVICE_MAX_LENGTH],
                      char password[PASSWORD_MAX_LENGTH]) {
  FILE *file = fopen(FILENAME, "a+");

  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  Credential new_credential;

  new_credential.id = credential_generate_id(file);

  strncpy(new_credential.service, service, SERVICE_MAX_LENGTH - 1);
  new_credential.service[SERVICE_MAX_LENGTH - 1] = '\0';
  strncpy(new_credential.password, password, PASSWORD_MAX_LENGTH - 1);
  new_credential.password[PASSWORD_MAX_LENGTH - 1] = '\0';

  fprintf(file, "%d,%s,%s\n", new_credential.id, new_credential.service,
          new_credential.password);

  fclose(file);
  return 0;
}

int credential_destroy(int credential_id) {
  FILE *file = fopen(FILENAME, "r");

  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  FILE *temp_file = fopen("temp.csv", "w");

  if (temp_file == NULL) {
    fclose(file);
    return ERROR_OPENING_FILE;
  }

  char line[MAX_LINE_LENGTH];
  bool found = false;

  while (fgets(line, sizeof(line), file) != NULL) {
    int current_id;

    if (sscanf(line, "%d,", &current_id) == 1) {
      if (current_id == credential_id) {
        found = true;
        continue;
      }
    }
    fprintf(temp_file, "%s", line);
  }

  fclose(file);
  fclose(temp_file);

  if (!found) {
    remove("temp.csv");
    return 0;
  }

  if (remove(FILENAME) != 0 || rename("temp.csv", FILENAME) != 0) {
    return ERROR_OPENING_FILE;
  }

  return 0;
}

int credential_update_password(int credential_id,
                               char password[PASSWORD_MAX_LENGTH]) {
  FILE *file = fopen(FILENAME, "r");

  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  FILE *temp_file = fopen("temp.csv", "w");

  if (temp_file == NULL) {
    fclose(file);
    return ERROR_OPENING_FILE;
  }

  char line[MAX_LINE_LENGTH];
  bool found = false;

  while (fgets(line, sizeof(line), file) != NULL) {
    int current_id;
    char current_service[SERVICE_MAX_LENGTH];
    char current_password[PASSWORD_MAX_LENGTH];

    if (sscanf(line, "%d,%49[^,],%31[^\n]", &current_id, current_service,
               current_password) == 3) {
      if (current_id == credential_id) {
        fprintf(temp_file, "%d,%s,%s\n", current_id, current_service, password);
        found = true;
      } else {
        fprintf(temp_file, "%s", line);
      }
    }
  }

  fclose(file);
  fclose(temp_file);

  if (!found) {
    remove("temp.csv");
    return 0;
  }

  if (remove(FILENAME) != 0 || rename("temp.csv", FILENAME) != 0) {
    return ERROR_OPENING_FILE;
  }

  return 0;
}

int credential_update_service(int credential_id,
                              const char new_service[SERVICE_MAX_LENGTH]) {
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  FILE *temp_file = fopen("temp.csv", "w");
  if (temp_file == NULL) {
    fclose(file);
    return ERROR_OPENING_FILE;
  }

  char line[MAX_LINE_LENGTH];
  bool found = false;

  while (fgets(line, sizeof(line), file) != NULL) {
    int current_id;
    char current_service[SERVICE_MAX_LENGTH];
    char current_password[PASSWORD_MAX_LENGTH];

    if (sscanf(line, "%d,%49[^,],%31[^\n]", &current_id, current_service,
               current_password) == 3) {
      if (current_id == credential_id) {
        fprintf(temp_file, "%d,%s,%s\n", current_id, new_service,
                current_password);
        found = true;
      } else {
        fprintf(temp_file, "%s", line);
      }
    }
  }

  fclose(file);
  fclose(temp_file);

  if (!found) {
    remove("temp.csv");
    return ERROR_CREDENTIAL_NOT_FOUND;
  }

  if (remove(FILENAME) != 0 || rename("temp.csv", FILENAME) != 0) {
    return ERROR_OPENING_FILE;
  }

  return 0;
}

int credential_list(Credential *destination[MAX_CREDENTIALS]) {
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    return ERROR_OPENING_FILE;
  }

  int count = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, sizeof(line), file) != NULL && count < MAX_CREDENTIALS) {
    destination[count] = (Credential *)malloc(sizeof(Credential));

    if (destination[count] == NULL) {
      fclose(file);
      return ERROR_ALLOCATING_MEMORY;
    }

    Credential *current_cred = destination[count];

    if (sscanf(line, "%d,%49[^,],%31[^\n]", &current_cred->id,
               current_cred->service, current_cred->password) == 3) {
      count++;
    }
  }

  fclose(file);
  return count;
}

bool credential_repeated_service(char *service,
                                 Credential *credentials[MAX_CREDENTIALS]) {
  for (int i = 0; i < MAX_CREDENTIALS; i++) {
    if (credentials[i] == NULL) {
      break;
    }

    if (strcmp(credentials[i]->service, service) == 0) {
      return true;
    }
  }
}
