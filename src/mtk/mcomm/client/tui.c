#include "tui.h"

#define CONV_W 20
#define INPUT_H 5

WINDOW* win_input;
WINDOW* win_convers;
WINDOW* win_contacts;

static inline void _draw_borders() {
  mvvline(1, 0, 0, LINES - 2);
  mvvline(1, )
}

static inline void _init_windows() {
  win_input = newwin(5, COLS - 23, LINES - 6, 1);
  win_convers = newwin(LINES - 8, COLS - 23, 1, 1);
  win_contacts = newwin(LINES - 2, 20, 1, COLS - 21);
  box(win_input, 0, 0);
  box(win_convers, 0, 0);
  box(win_contacts, 0, 0);
  wprintw(win_input, "a\nb\nc\nd\ne");
  wrefresh(win_input);
  wrefresh(win_convers);
  wrefresh(win_contacts);
}

static inline void _free_windows() {
  delwin(win_input);
  delwin(win_convers);
  delwin(win_contacts);
}

void tui_init() {
  initscr();
  cbreak();
  noecho();
  _draw_borders();
  _init_windows();
}

void tui_free() {
  _free_windows();
  endwin();
}
