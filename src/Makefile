JANSSON_CFLAGS = $(shell pkg-config --cflags jansson)
JANSSON_LIBS = $(shell pkg-config --libs jansson)

cflags = -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
				 -Wsign-conversion -Wformat=2 -Wcast-qual -Wpointer-arith \
         -Wstrict-prototypes -Wlogical-op -Wduplicated-cond \
				 -Wduplicated-branches -Wnull-dereference -Wjump-misses-init \
				 -fanalyzer $(shell pkg-config --cflags tap) $(JANSSON_CFLAGS)

lflags = $(shell pkg-config --libs tap) $(JANSSON_LIBS)

exec = tempo
objects = datetime.o timeseries.o utils.o validation.o json_parser.o

.PHONY: build clean test

build: $(exec)

$(exec): $(exec).o $(objects)
	gcc -o $@ $^ $(lflags)
	mkdir -p bin
	cp $(exec) bin/

$(exec).o: $(exec).c
	gcc $(cflags) -c -o $@ $

datetime.o: datetime.c datetime.h
	gcc $(cflags) -c -o $@ $

timeseries.o: timeseries.c timeseries.h
	gcc $(cflags) -c -o $@ $

utils.o: utils.c utils.h
	gcc $(cflags) -c -o $@ $

validation.o: validation.c validation.h
	gcc $(cflags) -c -o $@ $

json_parser.o: json_parser.c json_parser.h
	gcc $(cflags) -c -o $@ $

test_datetime.o: test_datetime.c
	gcc $(cflags) -c -o $@ $

test_datetime: test_datetime.o datetime.o validation.o utils.o
	gcc -o $@ $^ $(lflags)

clean:
	rm -f $(exec)
	rm -f *.o
	rm -rf bin

test: test_datetime
	./test_datetime
