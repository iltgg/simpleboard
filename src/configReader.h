#ifndef FILEREADER_H
#define FILEREADER_H

#include <stdio.h>

#define MAX_LINE 100
#define INIT_CONIFG(C)                                                         \
  Config C = {                                                                 \
      .command.count = 0,                                                      \
      .preference.color = "blue",                                              \
      .preference.height = 10,                                                 \
      .preference.width = 20,                                                  \
      .preference.dynamicHeight = 0,                                           \
      .preference.dynamicWidth = 0,                                            \
      .preference.dynamicWidthMin = 0,                                         \
      .preference.title = "simpleboard",                                       \
      .preference.titleColor = "red",                                          \
      .preference.border = 1,                                                  \
      .preference.preCommand = "",                                             \
      .preference.exit = 1,                                                    \
      .preference.name = "simpleboard",                                        \
  }

typedef struct CommandEntry {
  char *name;
  char *command;
  char *hotkey;
  char *misc;
  int commandLength;
  int x;
  int y;
} CommandEntry;

typedef struct PreferenceConfig {
  char color[MAX_LINE];
  int height;
  int width;
  int dynamicHeight;
  int dynamicWidth;
  int dynamicWidthMin;
  char title[MAX_LINE];
  char titleColor[MAX_LINE];
  int border;
  char preCommand[MAX_LINE];
  int exit;
  char name[MAX_LINE];
} PreferenceConfig;

typedef struct CommandConfig {
  int count;
  CommandEntry *commands;
} CommandConfig;

typedef struct Config {
  CommandConfig command;
  PreferenceConfig preference;
} Config;

enum ReadState { PREFERENCE, COMMAND, NONE };

int readConfig(FILE *fp, Config *config);
void freeConfig(Config *config);

#endif
