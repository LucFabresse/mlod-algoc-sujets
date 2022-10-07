CFLAGS=-D_POSIX_C_SOURCE -Wall -W -std=c11
CC=clang

PROGRAMS=main

all: $(PROGRAMS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%: %.o
	${CC} -o $@ ${LDFLAGS} $^

main: main.o

run: main
	./main

clean:
	@rm -f *.o $(PROGRAMS)

re: clean all

.PHONY: all clean re run
