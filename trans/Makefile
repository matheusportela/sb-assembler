CC = gcc
CFLAGS = -ansi -Wall -g

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
HEADERS = $(SOURCES:.c=.h)
EXECUTABLES = ../bin/translator

INC = -I.

.PHONY: all
all: $(SOURCES) $(EXECUTABLES)

# Create executable file
$(EXECUTABLES): $(OBJECTS)
	$(CC) -O2 -o $@ $^
	
# Create object files
.c.o:
	$(CC) $(CFLAGS) $(INC) -c $<

.PHONY: print
print:
	@echo Sources: $(SOURCES)
	@echo Objects: $(OBJECTS)
	@echo Headers: $(HEADERS)
	@echo Executable: $(EXECUTABLES)

.PHONY: clean
clean:
	-rm -f $(EXECUTABLES) *.o