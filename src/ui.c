#include "ui.h"
#include <ncurses.h>
#include <stdbool.h>

/*
Now using stdscr instead of a specific window, remove the border
but we can add things to the screen more easily
*/

void ui_init() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
}

void ui_refresh() { refresh(); }

void ui_clear() { clear(); }

void ui_end() { endwin(); }

void ui_display(const char *format, ...) {
  // Buffer to hold the formatted string
  char buffer[1024]; // Adjust size as needed

  va_list args;
  va_start(args, format);

  // Format the string into the buffer
  vsnprintf(buffer, sizeof(buffer), format, args);

  va_end(args);

  // Display the formatted string
  printw("%s", buffer);
}

void ui_display_at(int x, int y, const char *message) {
  mvprintw(y, x, "%s", message);
}

int ui_menu(const char *title, const char *options[], int count) {
  int highlight = 0;

  while (true) {
    ui_display("Passguard - Gerenciador de senhas\n\n");
    ui_display(title);
    ui_display("\n");
    ui_display("\n");

    for (int i = 0; i < count; i++) {
      if (i == highlight) {
        attron(A_REVERSE);
      }
      ui_display(options[i]);
      attroff(A_REVERSE);
      ui_display("\n");
    }

    int ch = getch();

    switch (ch) {
    case KEY_UP:
      highlight = (highlight - 1 + count) % count;
      break;
    case KEY_DOWN:
      highlight = (highlight + 1) % count;
      break;
    case 10:
      return highlight;
    }

    ui_clear();
    ui_refresh();
  }
}

int ui_getch() { return getch(); }

bool ui_confirm(const char *message) {
  ui_display(message);

  while (true) {
    int ch = getch();
    if (ch == (int)'s' || ch == (int)'S') {
      return true;
    } else if (ch == (int)'n' || ch == (int)'N') {
      return false;
    }
  }
}

void ui_wait_for_key() {
  ui_display("Pressione qualquer tecla para continuar...\n");
  getch();
}

void ui_input(const char *message, char *buffer, int buffer_size) {
  ui_display(message);
  echo();
  getnstr(buffer, buffer_size);
  noecho();
}

void ui_input_sensitive(const char *message, char *buffer, int buffer_size) {
  ui_display(message);
  noecho(); // Make sure this is set to noecho

  int ch, i = 0;
  while ((ch = getch()) != '\n' && i < buffer_size - 1) {
    // Removing character if backspace is pressed
    if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
      // This `mv`functions are only being used here because we are using
      // the standard screen `stdscr` instead of a window
      if (i > 0) {
        i--;
        // No need to remove the character since we'll update it next time
        // user types a character
        mvaddch(getcury(stdscr), getcurx(stdscr) - 1, ' ');
        move(getcury(stdscr), getcurx(stdscr) - 1);
      }
    } else {
      buffer[i++] = ch;
      addch('*');
    }
    refresh();
  }
  buffer[i] = '\0';

  ui_display("\n");
}
