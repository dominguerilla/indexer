CC=gcc
LIBS=-lm
FLAGS=-Wall -g

OBJ=tokenizer.c
HDR=tokenizer.h

test: $(OBJ) $(HDR)
	$(CC) driver.c $(OBJ) $(FLAGS) $(LIBS) -o tokenizer

clean:
	rm -f tokenizer
