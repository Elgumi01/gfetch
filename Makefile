CC = gcc

CFLAGS = -Wall -Wextra -O2 -Iinclude -MMD -MP -Wpedantic
DEBUGFLAGS = -g -fsanitize=address,undefined

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

TARGET = gfetch
PREFIX = /usr/bin

.PHONY: all debug clean install uninstall

all: $(TARGET)

debug: CFLAGS += $(DEBUGFLAGS)
debug: clean all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)

-include $(DEP)

clean:
	rm -f src/*.o src/*.d $(TARGET)

install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)
