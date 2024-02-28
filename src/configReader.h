#ifndef FILEREADER_H
#define FILEREADER_H

#include <stdio.h>

typedef struct Entry {
  char *key;
  char *value;
} Entry;

typedef struct Config {
  int appearanceCount;
  int commandsCount;
  Entry *appearance;
  Entry *commands;
} Config;

enum ReadState { APPEARANCE, COMMANDS, NONE };

int readConfig(FILE *fp, Config *config);
void freeConfig(Config *config);

#endif
