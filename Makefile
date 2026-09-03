.PHONY: run

build: src/main.c
	cc -o build/main src/main.c -lraylib

run: build
	./build/main