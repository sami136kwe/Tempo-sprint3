.PHONY: build clean test test-bats test-libtap

exec = tempo

build:
	$(MAKE) build -C src
	mkdir -p bin
	cp src/$(exec) bin/$(exec)

test-bats: build
	$(MAKE) test -C bats

test-libtap:
	$(MAKE) test -C src

test: test-libtap test-bats

clean:
	$(MAKE) clean -C src
	rm -rf bin
