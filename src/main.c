#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "credential.h"
#include "clipboard.h"
#include "password.h"

int main()
{
  // Allocates memory and initializes ncurses
  initscr();
  cbreak();
  raw();
  noecho();

  // Constants for the window dimensions
  const int HEIGHT = 14;
  const int WIDTH = 40;
  const int start_y = 0;
  const int start_x = 0;

  // Pointer to a main window object from ncurses, with the specified dimensions
  WINDOW *main_win = newwin(HEIGHT, WIDTH, start_y, start_x);
  refresh();

  box(main_win, 0, 0);
  mvwprintw(main_win, 2, 3, "Passguard - Gerenciador de chaves");
  wrefresh(main_win);

  char *choices[] = {"Adicionar chave", "Acessar chaves", "Sair"};

  keypad(main_win, true);

  int key_pressed;
  int selected = 0;

  int const CREATE_OPTION = 0;
  int const ACCESS_OPTION = 1;
  int const EXIT_OPTION = 2;

  int const OPTIONS_NUMBER = 3;

  bool exit_program = false;

  while (exit_program == false)
  {
    // Display the main menu
    while (true)
    {
      for (int i = 0; i < OPTIONS_NUMBER; i++)
      {
        if (i == selected)
        {
          wattron(main_win, A_REVERSE);
        }
        mvwprintw(main_win, i + 4, 2, choices[i]);
        wattroff(main_win, A_REVERSE);
      }

      key_pressed = wgetch(main_win);

      if (key_pressed == KEY_UP && selected > 0)
      {
        selected--;
      }
      else if (key_pressed == KEY_DOWN && selected < 2)
      {
        selected++;
      }
      if (key_pressed == KEY_ENTER || key_pressed == 10)
      {
        break;
      }
    }

    // Handle the selected option
    if (selected == CREATE_OPTION)
    {
      WINDOW *create_win = newwin(HEIGHT, WIDTH, start_y, start_x);
      box(create_win, 0, 0);
      mvwprintw(create_win, 2, 12, "Adicionar chave");
      mvwprintw(create_win, 4, 2, "Serviço (website/app):");
      mvwprintw(create_win, 5, 2, "Senha:");

      wattron(create_win, A_REVERSE);
      mvwprintw(create_win, 7, 2, "Deixe a senha vazia caso deseje");
      mvwprintw(create_win, 8, 2, "gerar uma senha aleatória");
      wattroff(create_win, A_REVERSE);

      char service[SERVICE_MAX_LENGTH];
      char password[MAX_PASSWORD_LENGTH];

      mvwgetnstr(create_win, 4, 24, service, SERVICE_MAX_LENGTH);
      mvwgetnstr(create_win, 5, 9, password, MAX_PASSWORD_LENGTH);

      if (strlen(password) == 0)
      {
        generate_password(24, password);
      }

      Credential credential;
      strcpy(credential.service, service);
      strcpy(credential.password, password);

      create_credential(&credential);

      mvwprintw(create_win, HEIGHT - 4, 2, "Senha registrada com sucesso");
      wrefresh(create_win);

      getch();
      delwin(create_win);
    }
    else if (selected == ACCESS_OPTION)
    {
      WINDOW *access_window = newwin(HEIGHT, WIDTH, start_y, start_x);
      Credential credentials[MAX_CREDENTIALS]; // Adjusted for local storage
      int count = list_credentials(credentials);

      box(access_window, 0, 0);
      mvwprintw(access_window, 2, 13, "Acessar chave");
      wrefresh(access_window);

      int selected = 0;
      int key_pressed;

      int const COPY_KEY = (int)'c';
      int const EDIT_KEY = (int)'e';
      int const DELETE_KEY = (int)'d';

      while (true)
      {
        for (int i = 0; i < count; i++)
        {
          if (i == selected)
          {
            wattron(access_window, A_REVERSE);
          }
          mvwprintw(access_window, i + 4, 2, credentials[i].service);
          wattroff(access_window, A_REVERSE);
        }
        wattron(access_window, A_REVERSE);
        mvwprintw(access_window, HEIGHT - 2, 2, "'c' copiar");
        mvwprintw(access_window, HEIGHT - 2, 14, "'e' editar");
        mvwprintw(access_window, HEIGHT - 2, 26, "'d' deletar");
        wattroff(access_window, A_REVERSE);

        wrefresh(access_window);

        key_pressed = wgetch(main_win);

        if (key_pressed == KEY_UP && selected > 0)
        {
          selected--;
        }
        else if (key_pressed == KEY_DOWN && selected < count - 1)
        {
          selected++;
        }
        else if (key_pressed == COPY_KEY || key_pressed == EDIT_KEY || key_pressed == DELETE_KEY)
        {
          break;
        }
      }

      int action_message_height = HEIGHT - 4;

      if (key_pressed == COPY_KEY)
      {
        copy_to_clipboard(credentials[selected].password);
        mvwprintw(access_window, action_message_height, 2, "Senha copiada para o clipboard.");
        wrefresh(access_window);
      }
      else if (key_pressed == EDIT_KEY)
      {
        WINDOW *edit_win = newwin(HEIGHT, WIDTH, start_y, start_x);
        box(edit_win, 0, 0);

        mvwprintw(edit_win, 2, 13, "Editar chave");
        mvwprintw(edit_win, 4, 2, "Serviço (website/app): %s", credentials[selected].service);
        mvwprintw(edit_win, 5, 2, "Senha: ");

        char password[MAX_PASSWORD_LENGTH];
        mvwgetnstr(edit_win, 5, 9, password, MAX_PASSWORD_LENGTH - 1);

        strcpy(credentials[selected].password, password);

        update_credential(&credentials[selected]);

        mvwprintw(edit_win, action_message_height, 2, "Senha atualizada com sucesso");
        wrefresh(edit_win);

        getch();
        delwin(edit_win);
      }
      else if (key_pressed == DELETE_KEY)
      {
        delete_credential(credentials[selected].id); // Use the correct id for deletion
        mvwprintw(access_window, action_message_height, 2, "Chave deletada com sucesso.");
        wrefresh(access_window);
      }

      getch();
      delwin(access_window);
    }
    else if (selected == EXIT_OPTION)
    {
      exit_program = true;
    }

    // Clear the main window to redisplay the menu
    wclear(main_win);
    box(main_win, 0, 0);
    mvwprintw(main_win, 2, 3, "Passguard - Gerenciador de chaves");
    wrefresh(main_win);
  }

  // Deallocates memory and ends ncurses
  endwin();
  return 0;
}
