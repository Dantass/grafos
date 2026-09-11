CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = main
SRC = dag.c main.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
		rm -f $(TARGET) $(OBJ) main.exe

run: $(TARGET)
		./$(TARGET)

.PHONY: all clean run