#include "configReader.h"
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

static void addBaseEntry(char *line, int *count, BaseEntry **entries) {
  if (*count == 0) {
    *entries = malloc(sizeof(BaseEntry));
  } else {
    *entries = realloc(*entries, sizeof(BaseEntry) * (*count + 1));
  }

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

  (*entries + *count)->key = malloc(sizeof(char) * (strlen(key) + 1));
  (*entries + *count)->value = malloc(sizeof(char) * (strlen(value) + 1));
  strcpy((*entries + *count)->key, key);
  strcpy((*entries + *count)->value, value);
  (*count)++;
}

static void addCommandEntry(char *line, int *count, CommandEntry **entry) {
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
  while (*line == ' ') {
    line++;
  }
  removeTrailing(name);
  i = 0;
  while (*line != ',') {
    command[i] = *line;
    line++;
    i++;
  }
  command[i] = '\0';
  line++;
  while (*line == ' ') {
    line++;
  }
  removeTrailing(command);
  i = 0;
  while (*line != '\0' && *line != ',') {
    hotkey[i] = *line;
    line++;
    i++;
  }
  hotkey[i] = '\0';
  removeTrailing(hotkey);
  if (*line == ',') {
    line++;
    while (*line == ' ') {
      line++;
    }
    i = 0;
    while (*line != '\0') {
      misc[i] = *line;
      line++;
      i++;
    }
    misc[i] = '\0';
    removeTrailing(misc);
  }

  (*entry + *count)->name = malloc(sizeof(char) * (strlen(name) + 1));
  (*entry + *count)->command = malloc(sizeof(char) * (strlen(command) + 1));
  (*entry + *count)->hotkey = malloc(sizeof(char) * (strlen(hotkey) + 1));
  (*entry + *count)->misc = malloc(sizeof(char) * (strlen(misc) + 1));
  strcpy((*entry + *count)->name, name);
  strcpy((*entry + *count)->command, command);
  strcpy((*entry + *count)->hotkey, hotkey);
  strcpy((*entry + *count)->misc, misc);
  (*count)++;
}

int readConfig(FILE *fp, Config *config) {
  char line[MAX_LINE];
  enum ReadState curState = NONE;

  if (fp == NULL) {
    puts("readConfig: invalid file pointer, does the config file exist?");
    return 1;
  }

  config->preferenceCount = 0;
  config->commandCount = 0;

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
      addBaseEntry(line, &(config->preferenceCount), &(config->preference));
      break;
    case COMMAND:
      addCommandEntry(line, &(config->commandCount), &(config->command));
      break;
    case NONE:
    default:
      break;
    }
  }

  return 0;
}

void freeConfig(Config *config) {
  for (int i = 0; i < config->preferenceCount; i++) {
    free(config->preference[i].key);
    free(config->preference[i].value);
  }
  for (int i = 0; i < config->commandCount; i++) {
    free(config->command[i].name);
    free(config->command[i].command);
    free(config->command[i].hotkey);
    free(config->command[i].misc);
  }
  free(config->preference);
  free(config->command);
}
