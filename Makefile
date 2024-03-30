build:
	./src/engine-mlisp/compiler.bin ./src/engine-mlisp/main.mlisp > ./out/main.s
	mkdir -p ./out
	mkdir -p ./bin
	nasm -F dwarf -g -f elf32 -o ./out/main.o ./out/main.s 
	ld -m elf_i386 -o ./out/main ./out/main.o
	rm ./out/main.o
	mv ./bin/latest ./bin/$(shell date +'%Y-%m-%d_%H-%M-%S' -r ./bin/latest)
	cp ./out/main ./bin/latest

run:
	./out/main

test: 
	./lib/bashunit tests/test.bash

act:
	act -P ubuntu-latest=catthehacker/ubuntu:act-latest --env ACTIONS_RUNTIME_URL=http://host.docker.internal:8080/ --env ACTIONS_RUNTIME_TOKEN=foo --env ACTIONS_CACHE_URL=http://host.docker.internal:8080/ --artifact-server-path artifacts