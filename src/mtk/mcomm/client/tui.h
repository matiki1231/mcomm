#ifndef TUI_H
#define TUI_H

#include <ncurses.h>

extern WINDOW* win_input;
extern WINDOW* win_convers;
extern WINDOW* win_contacts;

void tui_init();

void tui_free();

#endif
