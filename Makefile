CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.c)
LIBS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

golf.exe: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o golf.exe $(LIBS)

clean:
	del golf.exe