EXEC=svg2pdf
SOURCES=main.c pdf.c menu.c args.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall

.PHONY: default clean

default: $(EXEC)

args.o: args.c args.h
menu.o: menu.c menu.h
pdf.o: pdf.c pdf.h
main.o: main.c pdf.h menu.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ -lm

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
