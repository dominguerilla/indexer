TARGET=proj3
FLAGS=-g -Wall
LIBS=-lm
CC=gcc

default: $(TARGET)
all: default

OBJ=$(patsubst %.c, %.o, $(wildcard *.c))
HDR=$(wildcard *.h)

%.o: %.c $(HDR)
	$(CC) $(FLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJ)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -g -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
