#ifndef FILEREADER_H
#define FILEREADER_H

#include <stdio.h>

typedef struct BaseEntry {
  char *key;
  char *value;
} BaseEntry;

typedef struct CommandEntry {
  char *name;
  char *command;
  char *hotkey;
} CommandEntry;

typedef struct Config {
  int preferenceCount;
  int commandCount;
  BaseEntry *preference;
  CommandEntry *command;
} Config;

enum ReadState { PREFERENCE, COMMAND, NONE };

int readConfig(FILE *fp, Config *config);
void freeConfig(Config *config);

#endif
