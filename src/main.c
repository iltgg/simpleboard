#include "configReader.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void setup(Config *config) {
  if (strlen(config->preference.preCommand) > 0) {
    system(config->preference.preCommand);
  }
}

FILE *getUserConfig(char *file) {
  const char *DEFAULT_FILE = "simpleboard.conf";
  const char *CONFIG_PATH = "/.config/simpleboard/";
  const char *HOME = getenv("HOME");
  char path[200];

  strcpy(path, HOME);
  strcpy(path + strlen(path), CONFIG_PATH);

  if (file == NULL) {
    strcpy(path + strlen(path), DEFAULT_FILE);
  } else {
    strcpy(path + strlen(path), file);
  }

  return fopen(path, "r");
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

  int starty, startx;
  getbegyx(display, starty, startx);
  for (int i = 0; i < config->command.count; i++) {
    config->command.commands[i].y = starty + i + 1;
    config->command.commands[i].x = startx + 1;
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

CommandEntry *getCommandMouse(Config *config, int y, int x) {
  for (int i = 0; i < config->command.count; i++) {
    if (config->command.commands[i].y == y &&
        x >= config->command.commands[i].x &&
        x < config->command.commands[i].x +
                config->command.commands[i].commandLength) {
      return config->command.commands + i;
    }
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  char *configFile = NULL;
  int argType = 0;
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--config") || !strcmp(argv[i], "-c")) {
      i++;
      configFile = argv[i];
    }
    if (!strcmp(argv[i], "--file") || !strcmp(argv[i], "-f")) {
      i++;
      argType = 1;
      configFile = argv[i];
    }
    if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
      puts("Usage:\n  simpleboard [option(s)]\n\nOptions:\n  -h, --help\n    "
           "print help screen\n  -c, --config [dir]\n    specify a path to a "
           "config file to use\n  -f, --file [file_name]\n    specify a file "
           "in $HOME/.config/simpleboard/");
      return EXIT_SUCCESS;
    }
  }

  FILE *fp;
  if (configFile == NULL) {
    fp = getUserConfig(NULL);
  } else if (argType == 1) {
    fp = getUserConfig(configFile);
  } else {
    fp = fopen(configFile, "r");
  }

  INIT_CONIFG(config);
  if (readConfig(fp, &config)) {
    puts("readConfig: invalid file pointer, does the config file exist?");
    return EXIT_FAILURE;
  }

  printf("\033]2;%s\a", config.preference.name);

  initscr();
  use_default_colors();
  noecho();
  curs_set(0);
  start_color();
  setup(&config);

  WINDOW *display = createDisplay(&config);
  setPreference(&config, display);
  printOptions(&config, display);

  MEVENT mevent;
  keypad(display, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL);
  CommandEntry *pressCommand = NULL;

  // remove mouse delay, click detection will not work
  mouseinterval(0);
  // removes key delay, will cause issues in processing escape codes
  ESCDELAY = 0;

  int opt;
  CommandEntry *command = NULL;

INPUT:
  while (!command) {
    // getch is broken for mouse input for some reason so use wgetch
    opt = wgetch(display);
    if (opt == 27) {
      break;
    }
    switch (opt) {
    case KEY_RESIZE:
      delwin(display);
      clear();
      display = createDisplay(&config);
      setPreference(&config, display);
      printOptions(&config, display);
      break;

    case KEY_MOUSE:
      if (getmouse(&mevent) == OK) {
        // check if mouse press and release on same command
        if (mevent.bstate & BUTTON1_PRESSED) {
          pressCommand = getCommandMouse(&config, mevent.y, mevent.x);
        } else if (mevent.bstate & BUTTON1_RELEASED) {
          CommandEntry *releaseCommand =
              getCommandMouse(&config, mevent.y, mevent.x);
          if (pressCommand == releaseCommand) {
            command = releaseCommand;
          }
        }
      }
      break;

    default:
      command = getCommand(&config, opt);
    }
  }
  if (command) {
    // some commands will output causing ncurses to read chars
    // send all output to null
    char noOutput[MAX_LINE + 20] = "bash -c \"";
    strcat(noOutput, command->command);
    strcat(noOutput, "\" >/dev/null 2>&1");

    pid_t pid = fork();
    if (pid < 0) {
      exit(EXIT_FAILURE);
    } else if (pid == 0) { // if in fork create new session and run
      if (setsid() < 0) {
        exit(EXIT_FAILURE);
      }
      system(noOutput);
      exit(EXIT_SUCCESS);
    }
    if (config.preference.exit != 1) {
      command = NULL;
      goto INPUT;
    }
  }

  endwin();
  freeConfig(&config);
  fclose(fp);

  return EXIT_SUCCESS;
}
