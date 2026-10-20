CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.c)

golf: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o golf

clean:
	rm -f golf