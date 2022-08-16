all: build run

build:
	mkdir build && cd build && conan install .. && cmake .. && cmake --build . && cd ..

run: build
	build/bin/largo_game

rebuild:
	cd build && cmake .. && cmake --build . && cd ..

dev: rebuild run

clean:
	rm -rf build