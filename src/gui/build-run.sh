if  g++ -o test Game.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio; then
    # it actually compiled, yippee! now run the game
    ./test
else
    echo "Compilation has failed. Did you install lsfml?"
    echo "if that does not fix it run 'g++ -o test main.cpp MainMenu.cpp Game.cpp -lsfml-graphics -lsfml-window -lsfml-system -v' for debugging."
fi
