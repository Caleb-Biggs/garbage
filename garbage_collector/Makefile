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


### Tests ###


tests/tests: tests/tests.c tests/bitfield_tests.c
	gcc -o $@ $< $$(pkg-config --cflags --libs check)

test: tests/tests
	valgrind -q --leak-check=full --track-origins=yes --error-exitcode=1 ./$<

test-bitfield: tests/tests
	valgrind -q --leak-check=full --track-origins=yes --error-exitcode=1 ./$< -b