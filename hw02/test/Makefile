.PHONY: all check clean

CC = gcc
CFLAGS = -std=gnu99 -Wall -g

OBJS = ut_main.o
EXEC = ut_main

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS)
	$(CC) $^ -o $(EXEC)

check: $(EXEC)
	@./$(EXEC); \
	if [ $$? -eq 0 ] ; then \
		echo "PASS."; \
	else \
		echo "FAIL."; \
	fi

clean:
	rm -f $(OBJS) ut_main
