cflags = -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
				 -Wsign-conversion -Wformat=2 -Wcast-qual -Wpointer-arith \
         -Wstrict-prototypes -Wlogical-op -Wduplicated-cond \
				 -Wduplicated-branches -Wnull-dereference -Wjump-misses-init \
				 -fanalyzer
exec = tempo

.PHONY: build clean

build: $(exec)

$(exec): $(exec).o datetime.o timeseries.o utils.o
	gcc $(cflags) -o $@ $^

$(exec).o: $(exec).c
	gcc $(cflags) -c -o $@ $<

datetime.o: datetime.c datetime.h
	gcc $(cflags) -c -o $@ $<

timeseries.o: timeseries.c timeseries.h
	gcc $(cflags) -c -o $@ $<

utils.o: utils.c utils.h
	gcc $(cflags) -c -o $@ $<

clean:
	rm -f $(exec)
	rm -f *.o
