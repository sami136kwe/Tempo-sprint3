CFLAGS = -Wall -Wextra
exec = tempo

.PHONY: build clean

build: $(exec)

$(exec): $(exec).c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -f $(exec)
