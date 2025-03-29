CC := gcc
CFLAGS := -Werror -Wall -pedantic -g
LIBS := -lm
TARGET := out
MAIN := src/main.o 
OBJS := src/hashset.o src/bitfield.o src/arena.o src/garbage.o src/node.o
HEADERS := src/hashset.h src/shared.h src/bitfield.h src/arena.h src/garbage.h src/node.h

$(TARGET): $(MAIN) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN) $(OBJS) $(LIBS)

exec: $(TARGET)
	./$^

mem: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$^

clean:
	rm $(TARGET) $(OBJS)


### Tests ###


tests/tests: tests/tests.c $(TARGET) tests/bitfield_tests.c 
	gcc -o $@ $< $(OBJS) $$(pkg-config --cflags --libs check)

test: tests/tests
	valgrind -q --leak-check=full --track-origins=yes --error-exitcode=1 ./$<

test-bitfield: tests/tests
	valgrind -q --leak-check=full --track-origins=yes --error-exitcode=1 ./$< -b