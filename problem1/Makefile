CC = gcc
CFLAGS = -Wall -g -DTESTING
LDFLAGS = -lSDL2
TARGET = tests
VALGRIND_FLAGS = --leak-check=full --track-origins=yes --show-leak-kinds=all

all: $(TARGET)

$(TARGET): tests.c
	$(CC) $(CFLAGS) -o $(TARGET) tests.c $(LDFLAGS)

run: $(TARGET)
	@echo "=== Running Tests ==="
	@./$(TARGET)

test: run
	@echo "=== Running Memory Tests with Valgrind ==="
	@valgrind $(VALGRIND_FLAGS) ./$(TARGET)

clean:
	rm -f $(TARGET)
