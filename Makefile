# ---------------------------------- Engine ---------------------------------- #

engine-build:
	mkdir -p ./out/engine
	mkdir -p ./bin/engine
	cat ./lib/malloc.asm > ./out/engine/main.s
	./src/engine/compiler.bin ./src/engine/main.mlisp >> ./out/engine/main.s
	nasm -F dwarf -g -f elf32 -o ./out/main.o ./out/engine/main.s 
	ld -m elf_i386 -o ./out/engine/main ./out/engine/main.o
	rm ./out/engine/main.o
	
engine-publish:
	@if [ -f ./bin/engine/latest ]; then \
		mv ./bin/engine/latest ./bin/engine/$(shell date +'%Y-%m-%d_%H-%M-%S' -r ./bin/latest); \
	fi
	cp ./out/engine/main ./bin/engine/latest

engine-run:
	./out/engine/main $(fen)

engine-test: 
	./lib/bashunit tests/test.bash

# ------------------------------------ GUI ----------------------------------- #

gui-build:
	mkdir -p ./out/gui
	mkdir -p ./bin/gui
	g++ -o out/gui/game ./src/gui/Game.cpp ./src/gui/MoveGeneration.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	@if [ $$? -eq 0 ]; then \
		echo "Game compiled and written to out/game"; \
	else \
		echo "Compilation Failed!!!"; \
	fi

gui-publish:
	@if [ -f ./bin/gui/latest ]; then \
		mv ./bin/gui/latest ./bin/gui/$(shell date +'%Y-%m-%d_%H-%M-%S' -r ./bin/latest); \
	fi
	cp ./out/gui/main ./bin/gui/latest

gui-run:
	./out/gui/game

gui-test:
	g++ -o out/gui/test ./tests/gui/test.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./out/gui/test

# ---------------------------------- Common ---------------------------------- #

act:
	act -P ubuntu-latest=catthehacker/ubuntu:act-latest --env ACTIONS_RUNTIME_URL=http://host.docker.internal:8080/ --env ACTIONS_RUNTIME_TOKEN=foo --env ACTIONS_CACHE_URL=http://host.docker.internal:8080/ --artifact-server-path artifacts

clean:
	rm -rf ./out
	rm -rf ./bin