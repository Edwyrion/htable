CC = gcc

SRC = src
LIB = lib
BIN = build

CFLAGS = -I./$(LIB)
CFLAGS += -std=c17 -g -O3
CFLAGS += -Wpedantic -Wextra -Werror -Wall -Wunused

SRCS = htable.c
OBJS = $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(addprefix $(SRC)/, $(SRCS)))

all: setup clean $(OBJS)

build: setup clean program

setup:
	mkdir -p $(BIN)

clean:
	rm -rf $(BIN)/*.o $(BIN)/*.exe

$(BIN)/%.o: $(SRC)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

program: $(OBJS)
	$(CC) -o $(BIN)/$@ $^ $(CFLAGS)

.PHONY: all directories