build:
	mkdir -p ./out
	mkdir -p ./bin
	cat ./lib/malloc.asm > ./out/main.s
	./src/engine/compiler.bin ./src/engine/main.mlisp >> ./out/main.s
	nasm -F dwarf -g -f elf32 -o ./out/main.o ./out/main.s 
	ld -m elf_i386 -o ./out/main ./out/main.o
	rm ./out/main.o
	
publish:
	if [ -f ./bin/latest ]; then \
		mv ./bin/latest ./bin/$(shell date +'%Y-%m-%d_%H-%M-%S' -r ./bin/latest); \
	fi
	cp ./out/main ./bin/latest

clean:
	rm -rf ./out
	rm -rf ./bin

run:
	./out/main

test: 
	./lib/bashunit tests/test.bash

act:
	act -P ubuntu-latest=catthehacker/ubuntu:act-latest --env ACTIONS_RUNTIME_URL=http://host.docker.internal:8080/ --env ACTIONS_RUNTIME_TOKEN=foo --env ACTIONS_CACHE_URL=http://host.docker.internal:8080/ --artifact-server-path artifacts