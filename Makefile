CC = gcc
CFLAGS = -Wall -g

O = out
OBJS = $(O)/logger.o $(O)/test.o

.PHONY: all
all: out $(O)/log.exe
	$(O)/log.exe

out:
	mkdir out

$(O)/log.exe: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(O)/log.exe

$(O)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
