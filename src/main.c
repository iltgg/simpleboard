#include "configReader.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

const char *PROGRAM_NAME = "simpleboard";
static int title = 1;

FILE *getConfigFile() {
  const char *CONFIG_PATH = "/.config/simpleboard/simpleboard.conf";
  const char *HOME = getenv("HOME");
  char path[100];

  strcpy(path, HOME);
  strcpy(path + strlen(path), CONFIG_PATH);

  return fopen(path, "r");
}

WINDOW *createDisplay(Config *config) {
  int height = 10;
  int width = 20;
  int startx, starty;

  for (int i = 0; i < config->preferenceCount; i++) {
    if (!strcmp(config->preference[i].key, "width")) {
      width = atoi(config->preference[i].value);
    } else if (!strcmp(config->preference[i].key, "height")) {
      height = atoi(config->preference[i].value);
    }
  }

  getmaxyx(stdscr, starty, startx);
  starty = (starty - height) / 2;
  startx = (startx - width) / 2;
  return newwin(height, width, starty, startx);
}

void setPreference(Config *config, WINDOW *display) {
  box(display, 0, 0);
  init_pair(1, COLOR_RED, -1);
  init_pair(2, COLOR_BLUE, -1);

  for (int i = 0; i < config->preferenceCount; i++) {
    if (!strcmp(config->preference[i].key, "color")) {
      if (!strcmp(config->preference[i].value, "blue")) {
        init_pair(2, COLOR_BLUE, -1);
      } else if (!strcmp(config->preference[i].value, "red")) {
        init_pair(2, COLOR_RED, -1);
      } else if (!strcmp(config->preference[i].value, "green")) {
        init_pair(2, COLOR_GREEN, -1);
      } else if (!strcmp(config->preference[i].value, "magenta")) {
        init_pair(2, COLOR_MAGENTA, -1);
      }
    }
    if (!strcmp(config->preference[i].key, "titleColor")) {
      if (!strcmp(config->preference[i].value, "blue")) {
        init_pair(1, COLOR_BLUE, -1);
      } else if (!strcmp(config->preference[i].value, "red")) {
        init_pair(1, COLOR_RED, -1);
      } else if (!strcmp(config->preference[i].value, "green")) {
        init_pair(1, COLOR_GREEN, -1);
      } else if (!strcmp(config->preference[i].value, "magenta")) {
        init_pair(1, COLOR_MAGENTA, -1);
      }
    }
    if (!strcmp(config->preference[i].key, "title")) {
      title = atoi(config->preference[i].value);
    }
  }
}

void printOptions(Config *config, WINDOW *display) {
  if (title) {
    wattron(display, COLOR_PAIR(1));
    mvwprintw(display, 0, 1, "simpleboard");
  }
  wattron(display, COLOR_PAIR(2));
  for (int i = 0; i < config->commandCount; i++) {
    mvwprintw(display, i + 1, 1, "%s", config->command[i].name);
  }
  refresh();
  wrefresh(display);
}

char *getCommand(Config *config, char key) {
  for (int i = 0; i < config->commandCount; i++) {
    if (config->command[i].hotkey[0] == key) {
      return config->command[i].command;
    }
  }
  return NULL;
}

int main() {
  FILE *fp = getConfigFile();
  Config config;
  if (readConfig(fp, &config)) {
    return EXIT_FAILURE;
  }

  printf("\033]2;%s\a", PROGRAM_NAME);
  // printf("\033]30;%s\a", PROGRAM_NAME);

  initscr();
  use_default_colors();
  noecho();
  curs_set(0);
  start_color();

#if DEBUG
  for (int i = 0; i < config.commandCount; i++) {
    printw("%s %s %s\n", config.command[i].name, config.command[i].command,
           config.command[i].hotkey);
  }
  for (int i = 0; i < config.preferenceCount; i++) {
    printw("%s %s\n", config.preference[i].key, config.preference[i].value);
  }
  refresh();
  getch();
  clear();
#endif

  WINDOW *display = createDisplay(&config);
  setPreference(&config, display);
  printOptions(&config, display);

  int opt;
  char *command = NULL;

  while (!command) {
    opt = getch();
    if (opt == 27) {
      break;
    }
    if (opt == KEY_RESIZE) {
      delwin(display);
      erase();
      display = createDisplay(&config);
      setPreference(&config, display);
      printOptions(&config, display);
    }
    command = getCommand(&config, opt);
  }
  if (command) {
    system(command);
  }

  // swaylock red screens if called with system() and the terminal exits before
  // logging back in, workaround until this issue can be figured out
  delwin(display);
  erase();
  int row, col;
  const char *exitMsg = "Welcome back! Press any key to continue...";
  getmaxyx(stdscr, row, col);
  mvprintw(row / 2, (col - strlen(exitMsg)) / 2, "%s", exitMsg);
  getch();

  // delwin(display);
  endwin();
  freeConfig(&config);
  fclose(fp);

  return EXIT_SUCCESS;
}
