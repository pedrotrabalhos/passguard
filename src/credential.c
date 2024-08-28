#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "credential.h"
#include "password.h"
#include <curses.h>

#define FILENAME "credentials.csv"
#define MAX_CREDENTIALS 100 // Make sure this is defined somewhere

/*
 * Helper function to save all credentials to a CSV file.
 */
void save_credentials_to_file(Credential *credentials, int count)
{
    FILE *file = fopen(FILENAME, "w");
    if (!file)
    {
        perror("Unable to open file for writing");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        encode_password(credentials[i].password);
        fprintf(file, "%d,%s,%s\n", credentials[i].id, credentials[i].service, credentials[i].password);
    }

    fclose(file);
}

/*
 * Helper function to load all credentials from a CSV file.
 */
int load_credentials_from_file(Credential credentials[MAX_CREDENTIALS])
{
    FILE *file = fopen(FILENAME, "r");
    if (!file)
    {
        perror("Unable to open file for reading");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%d,%31[^,],%31[^\n]", &credentials[count].id, credentials[count].service, credentials[count].password) == 3)
    {
        decode_password(credentials[count].password);
        count++;
        if (count >= MAX_CREDENTIALS)
            break;
    }

    fclose(file);
    return count;
}

void create_credential(Credential *credential)
{
    Credential credentials[MAX_CREDENTIALS];
    int count = load_credentials_from_file(credentials); // Pass the array, not the pointer to it

    credential->id = (count > 0) ? credentials[count - 1].id + 1 : 1;
    credentials[count] = *credential;
    count++;

    save_credentials_to_file(credentials, count);
}

void update_credential(Credential *credential)
{
    Credential credentials[MAX_CREDENTIALS];
    int count = load_credentials_from_file(credentials);

    for (int i = 0; i < count; i++)
    {
        if (credentials[i].id == credential->id)
        {
            strcpy(credentials[i].service, credential->service);
            strcpy(credentials[i].password, credential->password);
            save_credentials_to_file(credentials, count);
            return;
        }
    }
}

void delete_credential(int id)
{
    Credential credentials[MAX_CREDENTIALS];
    int count = load_credentials_from_file(credentials);

    int index = -1;
    for (int i = 0; i < count; i++)
    {
        if (credentials[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        return;
    }

    for (int i = index; i < count - 1; i++)
    {
        credentials[i] = credentials[i + 1];
    }

    save_credentials_to_file(credentials, count - 1);
}

int list_credentials(Credential credentials[MAX_CREDENTIALS])
{
    if (credentials == NULL)
    {
        return -1;
    }
    int count = load_credentials_from_file(credentials);
    return count;
}
