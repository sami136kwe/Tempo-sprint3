cflags = -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
				 -Wsign-conversion -Wformat=2 -Wcast-qual -Wpointer-arith \
         -Wstrict-prototypes -Wlogical-op -Wduplicated-cond \
				 -Wduplicated-branches -Wnull-dereference -Wjump-misses-init \
				 -fanalyzer $(shell pkg-config --cflags tap)
lflags = $(shell pkg-config --libs tap)
exec = tempo
objects = datetime.o timeseries.o utils.o validation.o

.PHONY: build clean test

build: $(exec)

$(exec): $(exec).o $(objects)
	gcc $(lflags) -o $@ $^

$(exec).o: $(exec).c
	gcc $(cflags) -c -o $@ $<

$(objects): %.o: %.c %.h
	gcc $(cflags) -c -o $@ $<

test_datetime.o: test_datetime.c
	gcc $(cflags) -c -o $@ $<

test_datetime: test_datetime.o datetime.o validation.o utils.o
	gcc -o $@ $^ $(lflags)

clean:
	rm -f $(exec)
	rm -f *.o

test: test_datetime
	./test_datetime
