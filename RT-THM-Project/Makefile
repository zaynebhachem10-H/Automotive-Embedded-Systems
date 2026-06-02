
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lncurses

SRC = src/main.c src/worker.c src/ipc_utils.c src/logger.c
OBJ = $(SRC:.c=.o)
EXEC = rt-thm

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(EXEC) system.log