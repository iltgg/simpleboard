C=gcc
CFLAGS=-Wall -Wextra -O2 -std=c99 -pedantic
DFLAGS=-g
EXECUTABLE=./bin/simpleboard
EFLAGS=-lncurses

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

$(EXECUTABLE): ./src/main.c ./src/configReader.c | bin
	$(C) -o $(EXECUTABLE) ./src/main.c ./src/configReader.c $(CFLAGS) $(EFLAGS)

bin:
	mkdir -p $@

install: $(EXECUTABLE)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 777 ./bin/simpleboard $(DESTDIR)$(PREFIX)/bin/

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/simpleboard

test: ./tests/readConfig_test.c ./src/configReader.c | bin
	$(C) -o ./bin/readConfig_test ./tests/readConfig_test.c ./src/configReader.c $(CFLAGS) $(EFLAGS) && ./bin/readConfig_test

debug: ./src/main.c ./src/configReader.c | bin
	$(C) -o $(EXECUTABLE)_debug ./src/main.c ./src/configReader.c $(DFLAGS) $(CFLAGS) $(EFLAGS)
