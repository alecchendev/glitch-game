all: build run

install:
	echo "Not implemented"

run: build
	build/main

build:
	cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=/Users/alecchen/Developer/vcpkg/scripts/buildsystems/vcpkg.cmake
	cmake --build build

clean:
	rm -rf build