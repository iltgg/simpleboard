#include "configReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 100
const char HEADER = ':';
const char COMMENT = '#';
const char *APPEARANCE_STRING = "[appearance]";
const char *COMMANDS_STRING = "[commands]";

int setHeader(char *line, enum ReadState *state) {
  char cmpline[MAX_LINE];
  strcpy(cmpline, line + 1);
  cmpline[strlen(cmpline) - 1] = '\0';

  if (!strcmp(cmpline, APPEARANCE_STRING)) {
    *state = APPEARANCE;
    return 0;
  }
  if (!strcmp(cmpline, COMMANDS_STRING)) {
    *state = COMMANDS;
    return 0;
  }

  *state = NONE;
  return 1;
}

void setConfigPointer(int **count, Entry ***entry, Config *config,
                      enum ReadState state) {
  if (state == APPEARANCE) {
    *count = &(config->appearanceCount);
    *entry = &(config->appearance);
  } else if (state == COMMANDS) {
    *count = &(config->commandsCount);
    *entry = &(config->commands);
  } else {
    *count = NULL;
    *entry = NULL;
  }
}

void removeTrailing(char *string) {
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

void addEntry(char *line, int *count, Entry **entries) {
  if (*count == 0) {
    *entries = malloc(sizeof(Entry));
  } else {
    *entries = realloc(*entries, sizeof(Entry) * (*count + 1));
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

int readConfig(FILE *fp, Config *config) {
  char line[MAX_LINE];
  int *curCount = NULL;
  Entry **curEntry = NULL;
  enum ReadState state = NONE;

  if (fp == NULL) {
    puts("readConfig: invalid file pointer");
    return 1;
  }

  config->appearanceCount = 0;
  config->commandsCount = 0;

  while (fgets(line, 100, fp) != NULL) {
    if (line[0] == '\n' || line[0] == '\r') {
      continue;
    }
    if (line[0] == COMMENT) {
      continue;
    }
    if (line[0] == HEADER) {
      setHeader(line, &state);
      setConfigPointer(&curCount, &curEntry, config, state);
      continue;
    }
    if (curCount == NULL || curEntry == NULL) {
      puts("readConfig: no header given");
      continue;
    }
    addEntry(line, curCount, curEntry);
  }

  return 0;
}

void freeConfig(Config *config) {
  for (int i = 0; i < config->appearanceCount; i++) {
    free(config->appearance[i].key);
    free(config->appearance[i].value);
  }
  for (int i = 0; i < config->commandsCount; i++) {
    free(config->commands[i].key);
    free(config->commands[i].value);
  }
  free(config->appearance);
  free(config->commands);
}

// int main() {
//   FILE *fptr;
//   Config config;
//
//   fptr = fopen("gusboard.conf", "r");
//
//   readConfig(fptr, &config);
//   printf("%d\n", config.appearanceCount);
//   printf("%d\n", config.commandsCount);
//   for (int i = 0; i < config.appearanceCount; i++) {
//     printf("%s: %s\n", config.appearance[i].key, config.appearance[i].value);
//   }
//   for (int i = 0; i < config.commandsCount; i++) {
//     printf("%s: %s\n", config.commands[i].key, config.commands[i].value);
//   }
//
//   fclose(fptr);
//   freeConfig(&config);
//
//   return EXIT_SUCCESS;
// }
