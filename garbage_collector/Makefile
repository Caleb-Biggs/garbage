CC := gcc
CFLAGS := -Werror -Wall -pedantic -g
LIBS := -lm
TARGET := out
OBJS := src/main.o src/hashset.c
HEADERS := src/hashset.h

$(TARGET): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

exec: $(TARGET)
	./$^

mem: $(TARGET)
	valgrind --leak-check=full ./$^