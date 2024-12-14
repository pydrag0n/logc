CC = gcc
CFLAGS = -Wall -Wextra -g
DEPFLAGS = -MT $@ -MMD -MP -MF $(O)/$*.d

O = out
OBJS = $(O)/logger.o
OBJSTEST = $(O)/test.o
DEPFILES = $(OBJS:%.o=%.d)
DEPFILES += $(OBJSTEST:%.o=%.d)

.PHONY: all
all: $(O) $(O)/log.a

check: $(O) $(O)/test.exe
	$(O)/test.exe

$(O)/test.exe: $(OBJSTEST)
	$(CC) $(CFLAGS) $(OBJSTEST) $(O)/log.a -o $(O)/test.exe

$(O)/log.a: $(OBJS)
	ar crs $(O)/log.a $(OBJS)

$(O):
	mkdir $@

$(O)/%.o : %.c $(O)/%.d
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@

$(DEPFILES):

include $(wildcard $(DEPFILES))
