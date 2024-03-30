build:
	./src/engine-mlisp/compiler.bin ./src/engine-mlisp/main.mlisp > ./out/main.s
	nasm -F dwarf -g -f elf32 -o ./out/main.o ./out/main.s 
	ld -m elf_i386 -o ./out/main ./out/main.o
	rm ./out/main.o
	cp ./out/main ./bin/$(shell date +'%Y-%m-%d_%H-%M-%S')-main

run:
	./out/main