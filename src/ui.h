#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

void ui_init();

void ui_refresh();

void ui_clear();

void ui_end();

void ui_display(const char *format, ...);

void ui_display_at(int x, int y, const char *message);

int ui_menu(const char *title, const char *options[], int count);

int ui_getch();

bool ui_confirm(const char *message);

void ui_wait_for_key();

void ui_input(const char *message, char *buffer, int buffer_size);

void ui_input_sensitive(const char *message, char *buffer, int buffer_size);

#endif // UI_H
