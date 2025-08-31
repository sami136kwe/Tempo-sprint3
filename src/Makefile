CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
				 -Wsign-conversion -Wformat=2 -Wcast-qual -Wpointer-arith \
         -Wstrict-prototypes -Wlogical-op -Wduplicated-cond \
				 -Wduplicated-branches -Wnull-dereference -Wjump-misses-init \
				 -fanalyzer
exec = tempo

.PHONY: build clean

build: $(exec)

$(exec): $(exec).o datetime.o timeseries.o
	gcc $(CFLAGS) -o $@ $^

$(exec).o: $(exec).c
	gcc $(CFLAGS) -c -o $@ $<

datetime.o: datetime.c datetime.h
	gcc $(CFLAGS) -c -o $@ $<

timeseries.o: timeseries.c timeseries.h
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(exec)
	rm -f *.o
