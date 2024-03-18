#include "../src/configReader.h"
#include "acutest.h"
#include <stdio.h>
#include <stdlib.h>

void defaultConfig(void) {
  FILE *fp = fopen("./tests/configs/default.conf", "r");
  INIT_CONIFG(config);
  readConfig(fp, &config);

  TEST_CHECK(config.command.count == 0);
  TEST_CHECK(!strcmp(config.preference.color, "blue"));
  TEST_CHECK(config.preference.height == 10);
  TEST_CHECK(config.preference.width == 20);
  TEST_CHECK(config.preference.dynamicHeight == 0);
  TEST_CHECK(config.preference.dynamicWidth == 0);
  TEST_CHECK(config.preference.dynamicWidthMin == 0);
  TEST_CHECK(!strcmp(config.preference.title, "simpleboard"));
  TEST_CHECK(!strcmp(config.preference.titleColor, "red"));
  TEST_CHECK(config.preference.border == 1);
  TEST_CHECK(!strcmp(config.preference.preCommand, ""));

  freeConfig(&config);
}

void emptyConfig(void) {
  FILE *fp = fopen("./tests/configs/empty.conf", "r");
  INIT_CONIFG(config);
  readConfig(fp, &config);

  TEST_CHECK(config.command.count == 0);
  TEST_CHECK(!strcmp(config.preference.color, "blue"));
  TEST_CHECK(config.preference.height == 10);
  TEST_CHECK(config.preference.width == 20);
  TEST_CHECK(config.preference.dynamicHeight == 0);
  TEST_CHECK(config.preference.dynamicWidth == 0);
  TEST_CHECK(config.preference.dynamicWidthMin == 0);
  TEST_CHECK(!strcmp(config.preference.title, "simpleboard"));
  TEST_CHECK(!strcmp(config.preference.titleColor, "red"));
  TEST_CHECK(config.preference.border == 1);
  TEST_CHECK(!strcmp(config.preference.preCommand, ""));

  freeConfig(&config);
}

void missingConfig(void) {
  FILE *fp = fopen("./tests/configs/missing.conf", "r");
  INIT_CONIFG(config);
  readConfig(fp, &config);

  TEST_CHECK(config.command.count == 0);
  TEST_CHECK(!strcmp(config.preference.color, "blue"));
  TEST_CHECK(config.preference.height == 10);
  TEST_CHECK(config.preference.width == 20);
  TEST_CHECK(config.preference.dynamicHeight == 0);
  TEST_CHECK(config.preference.dynamicWidth == 0);
  TEST_CHECK(config.preference.dynamicWidthMin == 0);
  TEST_CHECK(!strcmp(config.preference.title, "simpleboard"));
  TEST_CHECK(!strcmp(config.preference.titleColor, "red"));
  TEST_CHECK(config.preference.border == 1);
  TEST_CHECK(!strcmp(config.preference.preCommand, ""));

  freeConfig(&config);
}

void customConfig(void) {
  FILE *fp = fopen("./tests/configs/custom.conf", "r");
  INIT_CONIFG(config);
  readConfig(fp, &config);

  TEST_CHECK(config.command.count == 2);
  TEST_CHECK(!strcmp(config.preference.color, "green"));
  TEST_CHECK(config.preference.height == 11);
  TEST_CHECK(config.preference.width == 21);
  TEST_CHECK(config.preference.dynamicHeight == 1);
  TEST_CHECK(config.preference.dynamicWidth == 1);
  TEST_CHECK(config.preference.dynamicWidthMin == 1);
  TEST_CHECK(!strcmp(config.preference.title, "custom_title"));
  TEST_CHECK(!strcmp(config.preference.titleColor, "blue"));
  TEST_CHECK(config.preference.border == 1);
  TEST_CHECK(!strcmp(config.preference.preCommand, "echo test"));

  TEST_CHECK(!strcmp(config.command.commands[0].name, "foo"));
  TEST_CHECK(!strcmp(config.command.commands[0].command, "echo foo"));
  TEST_CHECK(!strcmp(config.command.commands[0].hotkey, "f"));
  TEST_CHECK(!strcmp(config.command.commands[0].misc, ""));
  TEST_CHECK(!strcmp(config.command.commands[1].name, "bar"));
  TEST_CHECK(!strcmp(config.command.commands[1].command, "echo bar"));
  TEST_CHECK(!strcmp(config.command.commands[1].hotkey, "b"));
  TEST_CHECK(!strcmp(config.command.commands[1].misc, "stay_alive"));

  freeConfig(&config);
}

void parsingConfig(void) {
  FILE *fp = fopen("./tests/configs/parsing.conf", "r");
  INIT_CONIFG(config);
  readConfig(fp, &config);

  TEST_CHECK(config.command.count == 3);
  TEST_CHECK(!strcmp(config.preference.color, "green"));
  TEST_CHECK(config.preference.height == 11);
  TEST_CHECK(config.preference.width == 21);
  TEST_CHECK(config.preference.dynamicHeight == 1);
  TEST_CHECK(config.preference.dynamicWidth == 1);
  TEST_CHECK(config.preference.dynamicWidthMin == 1);
  TEST_CHECK(!strcmp(config.preference.title, "custom_title"));
  TEST_CHECK(!strcmp(config.preference.titleColor, "blue"));
  TEST_CHECK(config.preference.border == 1);
  TEST_CHECK(!strcmp(config.preference.preCommand, "echo test"));

  TEST_CHECK(!strcmp(config.command.commands[0].name, "foo"));
  TEST_CHECK(!strcmp(config.command.commands[0].command, "echo \"foo,foo\""));
  TEST_CHECK(!strcmp(config.command.commands[0].hotkey, "f"));
  TEST_CHECK(!strcmp(config.command.commands[0].misc, ""));
  TEST_CHECK(!strcmp(config.command.commands[1].name, "bar"));
  TEST_CHECK(!strcmp(config.command.commands[1].command, "echo bar"));
  TEST_CHECK(!strcmp(config.command.commands[1].hotkey, "b"));
  TEST_CHECK(!strcmp(config.command.commands[1].misc, "stay_alive"));
  TEST_CHECK(!strcmp(config.command.commands[2].name, "foobar"));
  TEST_CHECK(
      !strcmp(config.command.commands[2].command, "echo \"bar,,,,,fooo####\""));
  TEST_CHECK(!strcmp(config.command.commands[2].hotkey, "p"));
  TEST_CHECK(!strcmp(config.command.commands[2].misc, "stay_alive"));

  freeConfig(&config);
}

// TODO
void errorConfig(void) {
  FILE *fp = fopen("./tests/configs/error.conf", "r");
  INIT_CONIFG(config);
  readConfig(fp, &config);

  freeConfig(&config);
}

TEST_LIST = {{"Default Config", defaultConfig}, {"Empty Config", emptyConfig},
             {"Missing Config", missingConfig}, {"Custom Config", customConfig},
             {"Parsing Config", parsingConfig}, {NULL, NULL}};
