#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Game{
    public:
    sf::RenderWindow window;
    std::string position;//Position in FEN
    sf::RectangleShape board[8][8]; // Array für die Schachbrettfelder
    sf::Texture pieceTextures[12]; // Array für die Texturen der Figuren
    sf::Sprite pieces[8][8]; // Array für die Figuren auf dem Schachbrett

    Game() : window(sf::VideoMode(800, 800), "SFML Chess"), position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
        loadTextures();
        initializeBoard();
    }

    void loadTextures(){
        for (int i=0;i<12;i++){
            pieceTextures[i].loadFromFile("assets/"+std::to_string(i)+".png");
        }
    }

    void initializeBoard() {
        bool isWhite = true;
        for(int i = 0; i < 8; ++i) {
            for(int j = 0; j < 8; ++j) {
                board[i][j] = sf::RectangleShape(sf::Vector2f(100, 100));
                board[i][j].setPosition(j * 100, i * 100);
                if(isWhite) {
                    board[i][j].setFillColor(sf::Color::White);
                } else {
                    board[i][j].setFillColor(sf::Color(34, 139, 34)); // Dunkelgrün
                }
                isWhite = !isWhite;
            }
            isWhite = !isWhite;
        }
    }

    void interpret(){
        int row = 7; // Starten Sie bei der letzten Reihe (Zeile 8)
        int col = 0; // Starten Sie in der ersten Spalte (A)
        
        for (char c : position) {
            std::cout << "Feld:" << std::to_string(row) << std::to_string(col) << std::endl;
            if (c == ' ') {
                // Informationen über die Möglichkeit der Rochade etc. erreicht
                break;
            } else if (c == '/') {
                // Neue Zeile erreicht, gehen Sie zur nächsten Zeile und setzen Sie die Spalte auf 0 zurück
                row--;
                col = 0;
            } else {
                switch (c){
                    case '1':
                        col+=1;
                        break;
                    case '2':
                        col+=2;
                        break;
                    case '3':
                        col+=1;
                        break;
                    case '4':
                        col+=4;
                        break;
                    case '5':
                        col+=5;
                        break;
                    case '6':
                        col+=6;
                        break;
                    case '7':
                        col+=7;
                        break;
                    case '8':
                        col+=8;
                        break;
                    default:
                        switch (c){
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
                        pieces[row][col].setScale(0.33f, 0.33f); // Skalieren Sie die Größe der Sprites
                        pieces[row][col].setPosition(col * 100, row * 100);
                        col++;
                        break;
                }
            }
        }
    }

    void drawBoard(){
        for(int i = 0; i < 8; ++i) {
            for(int j = 0; j < 8; ++j) {
                window.draw(board[i][j]);
            }
        }
    }

    void drawPieces(){
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
            window.draw(pieces[i][j]);
            }
        }
    }

    void run() {
        interpret();
        while (window.isOpen()) {
            window.clear();
            drawBoard();
            drawPieces();
            window.display();
        }
    }
};


int main() {
    Game main;
    main.run();
    return 0;
}