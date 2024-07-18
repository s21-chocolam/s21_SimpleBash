EXECUTABLE=../cat/s21_cat

CC=gcc

CFLAGS=-std=c11 -Wall -Werror -Wextra

SOURCES=s21_cat.c

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -r $(EXECUTABLE)
	
rebuild: clean all