#include "password.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define PASSWORD_KEY 0XAE

void encode_password(char raw_password[MAX_PASSWORD_LENGTH])
{
  int i, password_length = strlen(raw_password);

  for (i = 0; i < password_length; i++)
  {
    raw_password[i] = raw_password[i] - PASSWORD_KEY;
  }
}

void decode_password(char password_hash[MAX_PASSWORD_LENGTH])
{
  int i, password_length = strlen(password_hash);

  for (i = 0; i < password_length; i++)
  {
    password_hash[i] = password_hash[i] + PASSWORD_KEY;
  }
}

void generate_password(int password_length, char destination[])
{
  char *characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

  if (password_length >= MAX_PASSWORD_LENGTH)
  {
    fprintf(stderr, "Password length exceeds maximum allowed length.\n");
    return;
  }

  srand(time(NULL));

  for (int i = 0; i < password_length; i++)
  {
    int random = rand() % strlen(characters);
    destination[i] = characters[random];
  }

  destination[password_length] = '\0';
}