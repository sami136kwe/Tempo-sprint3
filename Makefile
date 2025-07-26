.PHONY: build clean

exec = tempo

build:
	$(MAKE) build -C src
	mkdir -p bin
	cp src/$(exec) bin/$(exec)

clean:
	$(MAKE) clean -C src
	rm -rf bin
