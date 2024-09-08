#include "configReader.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char HEADER = ':';
const char COMMENT = '#';
const char *PREFERENCE_STRING = "[appearance]";
const char *COMMAND_STRING = "[commands]";

static int setHeader(char *line, enum ReadState *state) {
  char cmpline[MAX_LINE];
  strcpy(cmpline, line + 1);
  cmpline[strlen(cmpline) - 1] = '\0';

  if (!strcmp(cmpline, PREFERENCE_STRING)) {
    *state = PREFERENCE;
    return 0;
  }
  if (!strcmp(cmpline, COMMAND_STRING)) {
    *state = COMMAND;
    return 0;
  }

  *state = NONE;
  return 1;
}

static void removeTrailing(char *string) {
  int mark = -1;
  for (int i = 0; i < (int)strlen(string); i++) {
    if (string[i] == ' ' || string[i] == '\n' || string[i] == '\r' ||
        string[i] == '\t') {
      mark = i;
    } else {
      mark = -1;
    }
  }
  if (mark > 0) {
    string[mark] = '\0';
  }
}

static void addPreference(char *line, PreferenceConfig *pref) {
  int i = 0;
  char key[MAX_LINE];
  char value[MAX_LINE];
  while (*line != '=') {
    key[i] = *line;
    line++;
    i++;
  }
  key[i] = '\0';
  line++;
  while (*line == ' ') {
    line++;
  }
  removeTrailing(key);
  i = 0;
  while (*line != '\0') {
    value[i] = *line;
    line++;
    i++;
  }
  value[i] = '\0';
  removeTrailing(value);

  if (!strcmp(key, "color")) {
    strcpy(pref->color, value);
  } else if (!strcmp(key, "height")) {
    pref->height = atoi(value);
  } else if (!strcmp(key, "width")) {
    pref->width = atoi(value);
  } else if (!strcmp(key, "dynamicHeight")) {
    pref->dynamicHeight = atoi(value);
  } else if (!strcmp(key, "dynamicWidth")) {
    pref->dynamicWidth = atoi(value);
  } else if (!strcmp(key, "dynamicWidthMin")) {
    pref->dynamicWidthMin = atoi(value);
  } else if (!strcmp(key, "title")) {
    strcpy(pref->title, value);
  } else if (!strcmp(key, "titleColor")) {
    strcpy(pref->titleColor, value);
  } else if (!strcmp(key, "border")) {
    pref->border = atoi(value);
  } else if (!strcmp(key, "preCommand")) {
    strcpy(pref->preCommand, value);
  } else if (!strcmp(key, "exit")) {
    pref->exit = atoi(value);
  } else if (!strcmp(key, "name")) {
    strcpy(pref->name, value);
  }
}

static int getNext(char **line, char *token) {
  int i = 0;
  bool escaped = false;

  while (**line == ' ') {
    (*line)++;
  }
  while (escaped == true || (**line != ',' && **line != '\0')) {
    if (**line == '\"') {
      escaped = !escaped;
    }
    token[i] = **line;
    (*line)++;
    i++;
  }
  token[i] = '\0';
  removeTrailing(token);

  if (**line == '\0') {
    return 1;
  }

  (*line)++;
  return 0;
}

static void addCommand(char *line, int *count, CommandEntry **entry) {
  if (*count == 0) {
    *entry = malloc(sizeof(CommandEntry));
  } else {
    *entry = realloc(*entry, sizeof(CommandEntry) * (*count + 1));
  }

  int i = 0;
  char name[MAX_LINE];
  char command[MAX_LINE];
  char hotkey[MAX_LINE];
  char misc[MAX_LINE] = ""; // or else behaviour is weird, pointers idk

  while (*line != '=') {
    name[i] = *line;
    line++;
    i++;
  }
  name[i] = '\0';
  line++;

  removeTrailing(name);
  getNext(&line, command);
  if (!getNext(&line, hotkey)) {
    getNext(&line, misc);
  }

  (*entry + *count)->name = malloc(sizeof(char) * (strlen(name) + 1));
  (*entry + *count)->command = malloc(sizeof(char) * (strlen(command) + 1));
  (*entry + *count)->hotkey = malloc(sizeof(char) * (strlen(hotkey) + 1));
  (*entry + *count)->misc = malloc(sizeof(char) * (strlen(misc) + 1));
  strcpy((*entry + *count)->name, name);
  strcpy((*entry + *count)->command, command);
  strcpy((*entry + *count)->hotkey, hotkey);
  strcpy((*entry + *count)->misc, misc);
  (*entry + *count)->commandLength = strlen(name);
  (*count)++;
}

int readConfig(FILE *fp, Config *config) {
  char line[MAX_LINE];
  enum ReadState curState = NONE;

  if (fp == NULL) {
    return 1;
  }

  config->command.count = 0;

  while (fgets(line, 100, fp) != NULL) {
    if (line[0] == '\n' || line[0] == '\r') {
      continue;
    }
    if (line[0] == COMMENT) {
      continue;
    }
    if (line[0] == HEADER) {
      setHeader(line, &curState);
      continue;
    }
    if (curState == NONE) {
      puts("readConfig: no header given");
      continue;
    }
    switch (curState) {
    case PREFERENCE:
      addPreference(line, &(config->preference));
      break;
    case COMMAND:
      addCommand(line, &(config->command.count), &(config->command.commands));
      break;
    case NONE:
    default:
      break;
    }
  }

  return 0;
}

void freeConfig(Config *config) {
  for (int i = 0; i < config->command.count; i++) {
    free(config->command.commands[i].name);
    free(config->command.commands[i].command);
    free(config->command.commands[i].hotkey);
    free(config->command.commands[i].misc);
  }
}
