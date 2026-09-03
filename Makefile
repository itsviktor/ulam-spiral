.PHONY: run

build: src/main.c
	mkdir build
	cc -o build/main src/main.c -lraylib

run: build
	./build/main