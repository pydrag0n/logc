CC = gcc
CFLAGS = -Wall -g
DEPFLAGS = -MT $@ -MMD -MP -MF $(O)/$*.d

O = out
OBJS = $(O)/logger.o $(O)/test.o
DEPFILES = $(OBJS:%.o=%.d)

.PHONY: all
all: $(O) $(O)/log.exe
	$(O)/log.exe

$(O)/log.exe: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(O)/log.exe

$(O):
	mkdir $@

$(O)/%.o : %.c $(O)/%.d
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@

$(DEPFILES):

include $(wildcard $(DEPFILES))
