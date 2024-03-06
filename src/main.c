#include "configReader.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0
#define DEBUG_CONF 0

static const char *DEFAULT_NAME = "simpleboard";

void setup(Config *config) {
  if (strlen(config->preference.preCommand) > 0) {
    system(config->preference.preCommand);
  }
}

FILE *getConfigFile() {
#if DEBUG_CONF == 1
  return fopen("./simpleboard.conf", "r");
#else
  const char *CONFIG_PATH = "/.config/simpleboard/simpleboard.conf";
  const char *HOME = getenv("HOME");
  char path[100];

  strcpy(path, HOME);
  strcpy(path + strlen(path), CONFIG_PATH);

  return fopen(path, "r");
#endif
}

WINDOW *createDisplay(Config *config) {
  int height = config->preference.height;
  int width = config->preference.width;
  int dynamicHeight = config->preference.dynamicHeight;
  int dynamicWidth = config->preference.dynamicWidth;
  int dynamicWidthMin = config->preference.dynamicWidthMin;
  int startx, starty;

  if (dynamicHeight) {
    height = config->command.count + 2;
  }
  if (dynamicWidth) {
    int maxWidth = dynamicWidthMin;
    int len = strlen(config->preference.title);
    if (len > maxWidth) {
      maxWidth = len;
    }
    for (int i = 0; i < config->command.count; i++) {
      int len = strlen(config->command.commands[i].name);
      if (len > maxWidth) {
        maxWidth = len;
      }
    }
    width = maxWidth + 2;
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

  if (!strcmp(config->preference.color, "blue")) {
    init_pair(2, COLOR_BLUE, -1);
  } else if (!strcmp(config->preference.color, "red")) {
    init_pair(2, COLOR_RED, -1);
  } else if (!strcmp(config->preference.color, "green")) {
    init_pair(2, COLOR_GREEN, -1);
  } else if (!strcmp(config->preference.color, "magenta")) {
    init_pair(2, COLOR_MAGENTA, -1);
  }
  if (!strcmp(config->preference.titleColor, "blue")) {
    init_pair(1, COLOR_BLUE, -1);
  } else if (!strcmp(config->preference.titleColor, "red")) {
    init_pair(1, COLOR_RED, -1);
  } else if (!strcmp(config->preference.titleColor, "green")) {
    init_pair(1, COLOR_GREEN, -1);
  } else if (!strcmp(config->preference.titleColor, "magenta")) {
    init_pair(1, COLOR_MAGENTA, -1);
  }
  if (config->preference.border == 0) {
    wborder(display, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  }
}

void printOptions(Config *config, WINDOW *display) {
  wattron(display, COLOR_PAIR(1));
  mvwprintw(display, 0, 1, "%s", config->preference.title);
  wattron(display, COLOR_PAIR(2));
  for (int i = 0; i < config->command.count; i++) {
    mvwprintw(display, i + 1, 1, "%s", config->command.commands[i].name);
  }
  refresh();
  wrefresh(display);
}

CommandEntry *getCommand(Config *config, char key) {
  for (int i = 0; i < config->command.count; i++) {
    if (config->command.commands[i].hotkey[0] == key) {
      return config->command.commands + i;
    }
  }
  return NULL;
}

int main() {
  FILE *fp = getConfigFile();
  INIT_CONIFG(config);
  if (readConfig(fp, &config)) {
    puts("readConfig: invalid file pointer, does the config file exist?");
    return EXIT_FAILURE;
  }

  printf("\033]2;%s\a", DEFAULT_NAME);

  initscr();
  use_default_colors();
  noecho();
  curs_set(0);
  start_color();
  setup(&config);

#if DEBUG
  printw("Commands:\n");
  for (int i = 0; i < config.command.count; i++) {
    printw("%s = %s, %s, %s\n", config.command.commands[i].name,
           config.command.commands[i].command,
           config.command.commands[i].hotkey, config.command.commands[i].misc);
  }
  printw("Preferences:\n");
  printw("%s", config.preference.title);
  refresh();
  getch();
  clear();
#endif

  WINDOW *display = createDisplay(&config);
  setPreference(&config, display);
  printOptions(&config, display);

  int opt;
  CommandEntry *command = NULL;

  while (!command) {
    opt = getch();
    if (opt == 27) {
      break;
    }
    if (opt == KEY_RESIZE) {
      delwin(display);
      clear();
      display = createDisplay(&config);
      setPreference(&config, display);
      printOptions(&config, display);
    }
    command = getCommand(&config, opt);
  }
  if (command) {
    // some commands will output causing ncurses to read chars
    // send all output to null
    char noOutput[MAX_LINE + 20] = "bash -c \"";
    strcat(noOutput, command->command);
    strcat(noOutput, "\" >/dev/null 2>&1");

    system(noOutput);
    if (!strcmp(command->misc, "stay_alive")) {
      clear();
      int row, col;
      const char *exitMsg = "Welcome back! Press any key to continue...";
      getmaxyx(stdscr, row, col);
      mvprintw(row / 2, (col - strlen(exitMsg)) / 2, "%s", exitMsg);
      getch();
    }
  }

  endwin();
  freeConfig(&config);
  fclose(fp);

  return EXIT_SUCCESS;
}
