LD_LIBRARY_PATH = /home/mirosha/lab5/lib.so
TARGET = lab5
CC = gcc
CFLAGS = -lpthread -lrt 
FLAGS = $(OPTIMIZATION) $(WARNINGS) $(CFLAGS)
OPTIMIZATION = 
WARNINGS = -Wshadow -Wall -Wextra -Wpedantic -Wbad-function-cast -Wcast-qual
DEPS = Makefile 
OBJ = main.o $(LD_LIBRARY_PATH) lib.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS) 

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)



clean:
	rm -f $(TARGET) *.o
