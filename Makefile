CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = greenscan

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
