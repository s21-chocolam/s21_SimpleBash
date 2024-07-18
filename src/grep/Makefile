EXECUTABLE=../grep/s21_grep

CC=gcc

CFLAGS=-std=c11 -Wall -Werror -Wextra

SOURCES=s21_grep.c

all: s21_grep

s21_cat:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -r $(EXECUTABLE)
	
rebuild: clean all
