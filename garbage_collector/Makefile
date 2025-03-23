CC := gcc
CFLAGS := -Werror -Wall -pedantic -g
LIBS := -lm
TARGET := out
OBJS := src/main.o src/graph.c src/arena.c src/garbage.c
HEADERS := src/graph.h src/arena.h src/garbage.h

$(TARGET): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

exec: $(TARGET)
	./$^

mem: $(TARGET)
	valgrind --leak-check=full ./$^