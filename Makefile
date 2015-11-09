TARGET= index
FLAGS=-ansi -pedantic -g -Wall
CC=gcc

default: $(TARGET)
all: default


index.o: index.c index.h tokenizer.h
	$(CC) $(FLAGS) -c index.c index.h tokenizer.h

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(FLAGS) -c tokenizer.c tokenizer.h

$(TARGET): index.o tokenizer.o
	$(CC) $(FLAGS) -o index index.o tokenizer.o

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f *~
	-rm -f *.h.gch
