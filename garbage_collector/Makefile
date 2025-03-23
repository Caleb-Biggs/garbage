CC := gcc
CFLAGS := -Werror -Wall -pedantic -g
LIBS := -lm
TARGET := out
OBJS := src/main.o src/graph.c src/arena.c
HEADERS := src/graph.h src/arena.h

$(TARGET): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

exec: $(TARGET)
	./$^

mem: $(TARGET)
	valgrind ./$^