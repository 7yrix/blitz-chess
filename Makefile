build:
	./src/engine-mlisp/compiler.bin ./src/engine-mlisp/main.mlisp > ./out/main.s
	nasm -F dwarf -g -f elf32 -o ./out/main.o ./out/main.s 
	ld -m elf_i386 -o ./out/main ./out/main.o
	rm ./out/main.o
	mv ./bin/latest ./bin/$(shell date +'%Y-%m-%d_%H-%M-%S' -r ./bin/latest)
	cp ./out/main ./bin/latest

run:
	./out/main

test: 
	./lib/bashunit tests/test.bash