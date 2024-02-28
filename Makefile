C=gcc
CFLAGS=-Wall -Wextra -O2 -std=c99 -pedantic
EXECUTABLE=./bin/simpleboard
EFLAGS=-lncurses


$(EXECUTABLE): ./src/main.c ./src/configReader.c| bin
	$(C) -o $(EXECUTABLE) ./src/main.c ./src/configReader.c $(CFLAGS) $(EFLAGS)

bin:
	mkdir -p $@

install: $(EXECUTABLE)
	cp ./bin/simpleboard /usr/local/bin/
	mkdir -p ~/.config/simpleboard/
	cp ./simpleboard.conf ~/.config/simpleboard/

uninstall:
	rm /usr/local/bin/simpleboard

test: ./src/configReader.c
	$(C) -o test ./src/configReader.c $(CFLAGS) $(EFLAGS)
