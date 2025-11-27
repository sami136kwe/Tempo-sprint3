cflags = -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
				 -Wsign-conversion -Wformat=2 -Wcast-qual -Wpointer-arith \
         -Wstrict-prototypes -Wlogical-op -Wduplicated-cond \
				 -Wduplicated-branches -Wnull-dereference -Wjump-misses-init \
				 -fanalyzer
exec = tempo
objects = datetime.o timeseries.o utils.o validation.o

.PHONY: build clean

build: $(exec)

$(exec): $(exec).o $(objects)
	gcc $(cflags) -o $@ $^

$(exec).o: $(exec).c
	gcc $(cflags) -c -o $@ $<

$(objects): %.o: %.c %.h
	gcc $(cflags) -c -o $@ $<

clean:
	rm -f $(exec)
	rm -f *.o
