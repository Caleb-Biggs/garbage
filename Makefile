CC := gcc
CFLAGS := -Werror -Wall -Wpedantic -g #-Wextra
HEADERS := src/types.h src/memory.h#src/hashset.h src/shared.h src/bitfield.h src/arena.h src/garbage.h src/node.h

LIBS := -lm
EXEC := out
MAIN := src/main.o 
OBJS := src/types.o src/memory.o #src/hashset.o src/bitfield.o src/arena.o src/garbage.o src/node.o

TEST_LIBS := check
TEST_EXEC := tests/tests
TEST_MAIN := tests/tests.o
TEST_OBJS := tests/bitfield_tests.o tests/hashset_tests.o

$(EXEC): $(MAIN) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXEC) $(MAIN) $(OBJS) $(LIBS)

exec: $(EXEC)
	./$^

mem: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$^

clean:
	rm $(EXEC) $(TEST_EXEC) $(MAIN) $(TEST_MAIN) $(OBJS) $(TEST_OBJS)


### Tests ###


$(TEST_EXEC): $(TEST_MAIN) $(EXEC) $(TEST_OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< $(OBJS) $$(pkg-config --cflags --libs $(TEST_LIBS))

test: $(TEST_EXEC)
	valgrind -q --leak-check=full --track-origins=yes --error-exitcode=1 ./$<

test-bitfield: $(TEST_EXEC)
	valgrind -q --leak-check=full --track-origins=yes --error-exitcode=1 ./$< -b

test-hashset: $(TEST_EXEC)
	valgrind -q --leak-check=full --track-origins=yes --error-exitcode=1 ./$< -h