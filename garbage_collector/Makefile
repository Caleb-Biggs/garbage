CC := gcc
CFLAGS := -Werror -Wall -pedantic -g
LIBS := -lm
TARGET := out
OBJS := src/main.o src/hashset.o src/bitfield.o src/arena.o src/garbage.o src/node.o
HEADERS := src/hashset.h src/shared.h src/bitfield.h src/arena.h src/garbage.h src/node.h

$(TARGET): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

exec: $(TARGET)
	./$^

mem: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$^

clean:
	rm $(TARGET) $(OBJS)