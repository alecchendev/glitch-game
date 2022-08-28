all: build run

build:
	mkdir build && cd build && conan install .. && cd .. && cmake -S . -B build && cmake --build build

run: build
	build/bin/glitch_game

rebuild:
	cmake -S . -B build && cmake --build build

dev: rebuild run

clean:
	rm -rf build