CC = gcc
CFLAGS = -ansi -Wall -g -arch x86_64

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
HEADERS = $(SOURCES:.c=.h)
EXECUTABLES = test_linked_list

LIBS = # [Link para as bibliotecas]

INC = -I. # [Coloque as demais pastas para arquivos-cabeçalho aqui]
DEF = # [Quaisquer definições]

.PHONY: all
all: $(SOURCES) $(EXECUTABLES)

# Create executable file
$(EXECUTABLES): $(OBJECTS)
	$(CC) $(CFLAGS) -O2 -o $@ $(OBJECTS) $(LIBS)
	
# Create object files
.c.o:
	$(CC) $(CFLAGS) $(INC) $(DEF) -c $< -o $@

.PHONY: print
print:
	@echo Sources: $(SOURCES)
	@echo Objects: $(OBJECTS)
	@echo Headers: $(HEADERS)
	@echo Executable: $(EXECUTABLES)

.PHONY: clean
clean:
	-rm -f $(EXECUTABLES) *.o