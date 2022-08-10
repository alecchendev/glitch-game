all: build run

build:
	mkdir build
	clang++ src/main.cpp -o build/largo-game

run: build
	build/largo-game

clean: build
	rm -rf build