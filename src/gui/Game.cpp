#include "utils/filesystem.cpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <string>

class Game {
public:
    sf::RenderWindow window;
    std::string position; // Position in FEN
    sf::RectangleShape board[8][8]; // Array für die Schachbrettfelder
    sf::Texture pieceTextures[12]; // Array für die Texturen der Figuren
    sf::Sprite pieces[8][8]; // Array für die Figuren auf dem Schachbrett

    Game()
        : window(sf::VideoMode(800, 800), "Blitz Chess")
        , position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    {
        loadTextures();
        initializeBoard();
    }

    void loadTextures()
    {
        for (int i = 0; i < 12; i++) {
            std::filesystem::path executableDirectory = getAbsoluteExecutableDirectory();
            pieceTextures[i].loadFromFile(executableDirectory / "assets" / (std::to_string(i) + ".png"));
        }
    }

    void initializeBoard()
    {
        int windowWidth = window.getSize().x;
        int windowHeight = window.getSize().y;

        // Since we need 64 equally sized fields, we need to determine which size is smaller
        float fieldSize = windowWidth < windowHeight ? windowWidth / 8 : windowHeight / 8;

				// We want to center the board in the middle of the window
				int boardY = (windowHeight - fieldSize * 8) / 2;
				int boardX = (windowWidth - fieldSize * 8) / 2;

        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                board[i][j] = sf::RectangleShape(sf::Vector2f(fieldSize, fieldSize));
                board[i][j].setPosition(boardX + j * fieldSize, boardY + i * fieldSize);
                // Check if this field should be white
                if ((i + j) % 2 == 0) {
                    board[i][j].setFillColor(sf::Color(238, 238, 210));
                } else {
                    board[i][j].setFillColor(sf::Color(118, 150, 86)); // Dunkelgrün
                }
            }
        }
    }

    void interpret()
    {
        int row = 7; // Starten Sie bei der letzten Reihe (Zeile 8)
        int col = 0; // Starten Sie in der ersten Spalte (A)

        for (char c : position) {
            if (c == ' ') {
                // Informationen über die Möglichkeit der Rochade etc. erreicht
                break;
            } else if (c == '/') {
                // Neue Zeile erreicht, gehen Sie zur nächsten Zeile und setzen Sie die Spalte auf 0 zurück
                row--;
                col = 0;
            } else {
                switch (c) {
                case '1':
                    col += 1;
                    break;
                case '2':
                    col += 2;
                    break;
                case '3':
                    col += 1;
                    break;
                case '4':
                    col += 4;
                    break;
                case '5':
                    col += 5;
                    break;
                case '6':
                    col += 6;
                    break;
                case '7':
                    col += 7;
                    break;
                case '8':
                    col += 8;
                    break;
                default:
                    switch (c) {
                    case 'k':
                        pieces[row][col].setTexture(pieceTextures[6]); // z. B. Textur für schwarzen König
                        break;
                    case 'K':
                        pieces[row][col].setTexture(pieceTextures[0]); // z. B. Textur für weißen König
                        break;
                    case 'q':
                        pieces[row][col].setTexture(pieceTextures[7]); // z. B. Textur für schwarze Dame
                        break;
                    case 'Q':
                        pieces[row][col].setTexture(pieceTextures[1]); // z. B. Textur für weiße Dame
                        break;
                    case 'r':
                        pieces[row][col].setTexture(pieceTextures[8]); // z. B. Textur für schwarzen Turm
                        break;
                    case 'R':
                        pieces[row][col].setTexture(pieceTextures[2]); // z. B. Textur für weißen Turm
                        break;
                    case 'b':
                        pieces[row][col].setTexture(pieceTextures[9]); // z. B. Textur für schwarzen Läufer
                        break;
                    case 'B':
                        pieces[row][col].setTexture(pieceTextures[3]); // z. B. Textur für weißen Läufer
                        break;
                    case 'n':
                        pieces[row][col].setTexture(pieceTextures[10]); // z. B. Textur für schwarzen Springer
                        break;
                    case 'N':
                        pieces[row][col].setTexture(pieceTextures[4]); // z. B. Textur für weißen Springer
                        break;
                    case 'p':
                        pieces[row][col].setTexture(pieceTextures[11]); // z. B. Textur für schwarzen Bauer
                        break;
                    case 'P':
                        pieces[row][col].setTexture(pieceTextures[5]); // z. B. Textur für weißen Bauer
                        break;
                    }

										int fieldSize = window.getSize().x < window.getSize().y ? window.getSize().x / 8 : window.getSize().y / 8;

										int boardY = (window.getSize().y - fieldSize * 8) / 2;
										int boardX = (window.getSize().x - fieldSize * 8) / 2;

										float scale = (float)fieldSize / pieceTextures[0].getSize().x;

                    pieces[row][col].setScale(scale, scale); // Skalieren Sie die Größe der Sprites
                    pieces[row][col].setPosition(boardX + col * fieldSize, boardY + row * fieldSize);
                    col++;
                    break;
                }
            }
        }
    }

    void drawBoard()
    {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                window.draw(board[i][j]);
            }
        }
    }

    void drawPieces()
    {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                window.draw(pieces[i][j]);
            }
        }
    }

    void run()
    {
        interpret();
				sf::Event event;

				while (window.isOpen()) {
					window.clear();
					while (window.pollEvent(event)) {
						if (event.type == sf::Event::Resized) {
							sf::FloatRect view(0, 0, event.size.width, event.size.height);
							window.setView(sf::View(view));
							initializeBoard();
							interpret();
						}

						if (event.type == sf::Event::Closed) {
							window.close();
						}
					}
					drawBoard();
					drawPieces();
					window.display();
				}

    }
};

int main()
{
    Game main;
    main.run();
    return 0;
}