#include "configReader.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1
#define CONFIG_PATH "simpleboard.conf"

const char *PROGRAM_NAME = "simpleboard";

void setAppearence(Config *config) {}

void printOptions(Config *config) {
  for (int i = 0; i < config->commandsCount; i++) {
    printw("%s\n", config->commands[i].key);
  }
  refresh();
}

int main() {
  FILE *fp = fopen(CONFIG_PATH, "r");
  Config config;
  readConfig(fp, &config);

  printf("\033]2;%s\a", PROGRAM_NAME);
  printf("\033]30;%s\a", PROGRAM_NAME);

  initscr();
  noecho();
  clear();

  printOptions(&config);

  char opt = getch();
  system("echo test");

  endwin();
  freeConfig(&config);
  fclose(fp);

  return EXIT_SUCCESS;
}
