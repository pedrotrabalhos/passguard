#include "clipboard.h"
#include "credential.h"
#include "ui.h"
#include <stdbool.h>
#include <string.h>

#define ACCESS_OPTION 0
#define CREATE_OPTION 1
#define UPDATE_OPTION 2
#define DELETE_OPTION 3
#define EXIT_OPTION 4

void read_service(Credential *existing_credentials[], int count,
                  char *destination);
void read_password(char *destination);

int main(void) {
  ui_init();

  while (true) {
    clear();
    const char *options[] = {"Acessar senhas", "Cadastrar nova senha",
                             "Atualizar senha", "Deletar senha", "Sair"};
    int option = ui_menu("Bem-vindo ao PassGuard", options, 5);

    if (option == EXIT_OPTION) {
      ui_end();
      break;
    }

    Credential *credentials[MAX_CREDENTIALS];
    int count = credential_list(credentials);

    switch (option) {
      char service[SERVICE_MAX_LENGTH];
      char password[PASSWORD_MAX_LENGTH];

    case ACCESS_OPTION: {
      ui_clear();

      if (count == ERROR_OPENING_FILE) {
        ui_display("Erro ao abrir arquivo\n");
        ui_wait_for_key();
        break;
      }

      if (count == 0) {
        ui_display("Nenhuma senha cadastrada\n");
        ui_wait_for_key();
        break;
      }

      char *options[count];

      for (int i = 0; i < count; i++) {
        options[i] = credentials[i]->service;
      }

      int selected = ui_menu("Suas senhas", (const char **)options, count);

      Credential *selected_credential = credentials[selected];

      while (true) {
        ui_clear();
        ui_display("Serviço: %s\n", selected_credential->service);
        ui_display("Senha: ");
        for (int i = 0; i < strlen(selected_credential->password); i++) {
          ui_display("*");
        }
        ui_display("\n\n");

        ui_display("Pressione S para copiar a senha ou ESC para voltar\n\n");

        int key = ui_getch();

        if (key == (int)'s') {
          clipboard_copy(selected_credential->password);
          ui_display("Senha copiada para a área de transferência.\n");
          ui_wait_for_key();
        } else if (key == 27) {
          break;
        }
      }

      break;
    }
    case CREATE_OPTION: {
      ui_clear();

      // Verify if the credentials limit has been reached
      if (count >= 5) {
        ui_display("Limite de senhas atingido\n");
        ui_wait_for_key();
        break;
      }

      ui_display("Cadastrar nova senha\n");

      read_service(credentials, count, service);
      read_password(password);

      credential_create(service, password);

      ui_display("\nSenha cadastrada com sucesso!\n");
      ui_refresh();
      break;
    }

    case UPDATE_OPTION: {
      ui_clear();

      // Verify if at least one credential exists
      if (count == 0) {
        ui_display("Você não possui nenhuma senha ainda.\n");
        ui_wait_for_key();
        break;
      }

      Credential *credentials[MAX_CREDENTIALS];
      int count = credential_list(credentials);

      if (count == ERROR_OPENING_FILE) {
        ui_display("Erro ao abrir arquivo\n");
        ui_wait_for_key();
        break;
      }

      char *options[count];

      for (int i = 0; i < count; i++) {
        options[i] = credentials[i]->service;
      }

      int selected = ui_menu("Atualizar senha", (const char **)options, count);

      Credential *selected_credential = credentials[selected];

      bool will_update_service =
          ui_confirm("Você deseja atualizar também o nome do serviço? (S/N)\n");

      if (will_update_service == true) {
        read_service(credentials, count, service);
        int response =
            credential_update_service(selected_credential->id, service);

        if (response < 0) {
          ui_display("Erro ao atualizar serviço\n");
          ui_wait_for_key();
          break;
        }
      }

      ui_display("Atualizando senha de %s\n", selected_credential->service);
      ui_input_sensitive("Insira sua nova senha\n", password,
                         PASSWORD_MAX_LENGTH);

      credential_update_password(selected_credential->id, password);

      ui_display("\nSenha atualizada com sucesso!\n");
      ui_wait_for_key();
      break;
    }

    case DELETE_OPTION: {
      ui_clear();

      // Verify if at least one credential exists
      if (count == 0) {
        ui_display("Você não possui nenhuma senha ainda.\n");
        ui_wait_for_key();
        break;
      }

      Credential *credentials[MAX_CREDENTIALS];
      int count = credential_list(credentials);

      if (count == ERROR_OPENING_FILE) {
        ui_display("Erro ao abrir arquivo\n");
        ui_wait_for_key();
        break;
      }

      char *options[count];

      for (int i = 0; i < count; i++) {
        options[i] = credentials[i]->service;
      }

      int selected = ui_menu("Deletar senha", (const char **)options, count);

      Credential *selected_credential = credentials[selected];

      bool confirmation =
          ui_confirm("Tem certeza que deseja deletar essa senha? (S/N)\n");

      if (confirmation == true) {
        credential_destroy(selected_credential->id);
        ui_display("Senha deletada com sucesso!\n");
        ui_wait_for_key();
      }

      break;
    }
    }
  }
  ui_end();
}

void read_service(Credential *existing_credentials[], int count,
                  char *destination) {
  char service[SERVICE_MAX_LENGTH];

  while (true) {
    ui_input("Qual serviço dessa senha?\n", service, SERVICE_MAX_LENGTH);

    if (strlen(service) == 0) {
      ui_display("O serviço não pode ser vazio\n");
      ui_wait_for_key();
      continue;
    }

    bool service_exists = false;
    for (int i = 0; i < count; i++) {
      if (strcmp(existing_credentials[i]->service, service) == 0) {
        ui_display("Esse serviço já existe\n");
        ui_wait_for_key();
        service_exists = true;
        break;
      }
    }

    if (!service_exists) {
      strcpy(destination, service);
      return; // Exit the function when a valid, non-existing service is entered
    }
  }
}
void read_password(char *destination) {
  char password[PASSWORD_MAX_LENGTH];

  while (true) {
    ui_input_sensitive("Insira sua senha\n", password, PASSWORD_MAX_LENGTH);

    if (strlen(password) == 0) {
      ui_display("A senha não pode ser vazia\n");
      ui_wait_for_key();
      continue;
    }

    break;
  }

  strcpy(destination, password);
}
