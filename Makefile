CC := gcc
CFLAGS := -Werror -Wall -Wpedantic -O2 -g #-Wextra
HEADERS := src/types.h src/arena.h src/meta_arena.h src/arena_manager.h src/garbage.h

LIBS := -lm
EXEC := out
MAIN := src/main.o 
OBJS := src/types.o src/arena.o src/meta_arena.o src/arena_manager.o src/garbage.o

TEST_LIBS := check
TEST_EXEC := tests/tests
TEST_MAIN := tests/tests.o
TEST_OBJS := tests/arena_tests.o #tests/bitfield_tests.o tests/hashset_tests.o

$(EXEC): $(MAIN) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXEC) $(MAIN) $(OBJS) $(LIBS)

exec: $(EXEC)
	./$^

mem: $(EXEC)
	valgrind --max-stackframe=8000048 --leak-check=full --show-leak-kinds=all ./$^
# 	cpulimit -l 80 -- valgrind --leak-check=full --show-leak-kinds=all ./$^

profile: $(EXEC)
	valgrind --max-stackframe=8000048 --tool=callgrind --callgrind-out-file=callgrind.out ./$^
	kcachegrind callgrind.out


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