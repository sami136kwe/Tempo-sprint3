CFLAGS = -Wall -Wextra
exec = tempo

.PHONY: build clean

build: $(exec)

$(exec): $(exec).o timeseries.o
	gcc $(CFLAGS) -o $@ $^

$(exec).o: $(exec).c
	gcc $(CFLAGS) -c -o $@ $<

timeseries.o: timeseries.c timeseries.h
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(exec)
	rm -f *.o
