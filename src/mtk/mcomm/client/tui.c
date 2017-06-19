#include "tui.h"

#include <pthread.h>

#define CONT_W 20
#define INPUT_H 5

pthread_mutex_t tui_mutex;

WINDOW* win_input;
WINDOW* win_convers;
WINDOW* win_contacts;

static inline void _draw_borders() {
  mvvline(1, 0, 0, LINES - 2);
  mvvline(1, COLS - CONT_W - 2, 0, LINES - 2);
  mvvline(1, COLS - 1, 0, LINES - 2);
  mvhline(0, 1, 0, COLS - 2);
  mvhline(LINES - INPUT_H - 2, 1, 0, COLS - CONT_W - 3);
  mvhline(LINES - 1, 1, 0, COLS - 2);
  refresh();
}

static inline void _init_windows() {
  win_input = newwin(INPUT_H, COLS - CONT_W - 3, LINES - INPUT_H - 1, 1);
  win_convers = newwin(LINES - INPUT_H - 3, COLS - CONT_W - 3, 1, 1);
  win_contacts = newwin(LINES - 2, CONT_W, 1, COLS - CONT_W - 1);
  scrollok(win_convers, TRUE);
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
//  noecho();
  _draw_borders();
  _init_windows();
  nodelay(win_input, TRUE);
  pthread_mutex_init(&tui_mutex, NULL);
}

void tui_free() {
  pthread_mutex_destroy(&tui_mutex);
  _free_windows();
  endwin();
}

void tui_lock() {
  pthread_mutex_lock(&tui_mutex);
}

void tui_unlock() {
  pthread_mutex_unlock(&tui_mutex);
}
