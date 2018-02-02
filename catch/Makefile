.PHONY:clean all
CC=g++
CFLAGS=-Wall -g
BIN=catch
all:$(BIN)
%.o:%.c%.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)

