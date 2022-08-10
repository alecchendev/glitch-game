all: build run

build:
	mkdir build && cd build && conan install .. && cmake .. && cmake --build . && cd ..

run: build
	build/bin/largo_game

clean:
	rm -rf build