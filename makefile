CC = gcc
FLAGS = -W -Wall -Wextra -Wpedantic

all: malloc.so basic_func error_handling stress_test

malloc.so: malloc.c
	$(CC) $^ $(FLAGS) -o $@ -shared -fPIC

basic_func: test/basic_func.c
	$(CC) $^ $(FLAGS) -o $@

error_handling: test/error_handling.c
	$(CC) $^ $(FLAGS) -o $@

stress_test: test/stress_test.c
	$(CC) $^ $(FLAGS) -o $@

.PHONY: clean
clean:
	rm -f malloc.so basic_func error_handling stress_test
