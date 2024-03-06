C=gcc
CFLAGS=-Wall -Wextra -O2 -std=c99 -pedantic
DFLAGS=-g
EXECUTABLE=./bin/simpleboard
EFLAGS=-lncurses


$(EXECUTABLE): ./src/main.c ./src/configReader.c | bin
	$(C) -o $(EXECUTABLE) ./src/main.c ./src/configReader.c $(CFLAGS) $(EFLAGS)

bin:
	mkdir -p $@

install: $(EXECUTABLE)
	cp ./bin/simpleboard /usr/local/bin/
	mkdir -p ~/.config/simpleboard/
	cp ./simpleboard.conf ~/.config/simpleboard/

update: $(EXECUTABLE)
	cp ./bin/simpleboard /usr/local/bin/

uninstall:
	rm /usr/local/bin/simpleboard

test: ./tests/readConfig_test.c ./src/configReader.c | bin
	$(C) -o ./bin/readConfig_test ./tests/readConfig_test.c ./src/configReader.c $(CFLAGS) $(EFLAGS)

debug: ./src/main.c ./src/configReader.c | bin
	$(C) -o $(EXECUTABLE)_debug ./src/main.c ./src/configReader.c $(DFLAGS) $(CFLAGS) $(EFLAGS)
