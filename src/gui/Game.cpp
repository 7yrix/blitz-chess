#include "utils/filesystem.cpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <string>

enum PieceType
{
    WHITE_KING,
    WHITE_QUEEN,
    WHITE_ROOK,
    WHITE_BISHOP,
    WHITE_KNIGHT,
    WHITE_PAWN,
    BLACK_KING,
    BLACK_QUEEN,
    BLACK_ROOK,
    BLACK_BISHOP,
    BLACK_KNIGHT,
    BLACK_PAWN,
    EMPTY
};

struct Piece{
    sf::Sprite piece;
    PieceType type;
};

class Game
{
public:
    sf::RenderWindow window;
    std::string position;           // Position in FEN
    sf::RectangleShape board[8][8]; // Array für die Schachbrettfelder
    sf::Texture pieceTextures[12];  // Array für die Texturen der Figuren
    // Initialize the pieces array with -1
    Piece pieces[8][8] = {{{.piece = sf::Sprite(), .type = PieceType::EMPTY}}}; // Array für die Schachfiguren
    // Create a transparent texture for empty fields
    sf::Texture emptyTexture;

    Game()
        : window(sf::VideoMode(800, 800), "Blitz Chess"), position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    {
        loadTextures();
        initializeBoard();
    }

    void loadTextures()
    {
        for (int i = 0; i < 12; i++)
        {
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

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                board[i][j] = sf::RectangleShape(sf::Vector2f(fieldSize, fieldSize));
                board[i][j].setPosition(boardX + j * fieldSize, boardY + i * fieldSize);
                // Check if this field should be white
                if ((i + j) % 2 == 0)
                {
                    board[i][j].setFillColor(sf::Color(238, 238, 210));
                }
                else
                {
                    board[i][j].setFillColor(sf::Color(118, 150, 86)); // Dunkelgrün
                }
            }
        }
    }

    void interpret()
    {
        int row = 0; // Starten Sie bei der letzten Reihe (Zeile 8)
        int col = 0; // Starten Sie in der ersten Spalte (A)

        for (char c : position)
        {
            if (c == ' ')
            {
                // Informationen über die Möglichkeit der Rochade etc. erreicht
                break;
            }
            else if (c == '/')
            {
                // Neue Zeile erreicht, gehen Sie zur nächsten Zeile und setzen Sie die Spalte auf 0 zurück
                row++;
                col = 0;
            }
            else
            {
                switch (c)
                {
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
                    switch (c)
                    {
                    case 'k':
                        pieces[row][col].piece.setTexture(pieceTextures[0]); // z. B. Textur für schwarzen König
                        pieces[row][col].type=PieceType::BLACK_KING;
                        break;
                    case 'K':
                        pieces[row][col].piece.setTexture(pieceTextures[6]); // z. B. Textur für weißen König
                        pieces[row][col].type=PieceType::WHITE_KING;
                        break;
                    case 'q':
                        pieces[row][col].piece.setTexture(pieceTextures[1]); // z. B. Textur für schwarze Dame
                        pieces[row][col].type=PieceType::BLACK_QUEEN;
                        break;
                    case 'Q':
                        pieces[row][col].piece.setTexture(pieceTextures[7]); // z. B. Textur für weiße Dame
                        pieces[row][col].type=PieceType::WHITE_QUEEN;
                        break;
                    case 'r':
                        pieces[row][col].piece.setTexture(pieceTextures[2]); // z. B. Textur für schwarzen Turm
                        pieces[row][col].type=PieceType::BLACK_ROOK;
                        break;
                    case 'R':
                        pieces[row][col].piece.setTexture(pieceTextures[8]); // z. B. Textur für weißen Turm
                        pieces[row][col].type=PieceType::WHITE_ROOK;
                        break;
                    case 'b':
                        pieces[row][col].piece.setTexture(pieceTextures[3]); // z. B. Textur für schwarzen Läufer
                        pieces[row][col].type=PieceType::BLACK_BISHOP;
                        break;
                    case 'B':
                        pieces[row][col].piece.setTexture(pieceTextures[9]); // z. B. Textur für weißen Läufer
                        pieces[row][col].type=PieceType::WHITE_BISHOP;
                        break;
                    case 'n':
                        pieces[row][col].piece.setTexture(pieceTextures[4]); // z. B. Textur für schwarzen Springer
                        pieces[row][col].type=PieceType::BLACK_KNIGHT;
                        break;
                    case 'N':
                        pieces[row][col].piece.setTexture(pieceTextures[10]); // z. B. Textur für weißen Springer
                        pieces[row][col].type=PieceType::WHITE_KNIGHT;
                        break;
                    case 'p':
                        pieces[row][col].piece.setTexture(pieceTextures[5]); // z. B. Textur für schwarzen Bauer
                        pieces[row][col].type=PieceType::BLACK_PAWN;
                        break;
                    case 'P':
                        pieces[row][col].piece.setTexture(pieceTextures[11]); // z. B. Textur für weißen Bauer
                        pieces[row][col].type=PieceType::WHITE_PAWN;
                        break;
                    default:
                        pieces[row][col].type = PieceType::EMPTY;
                        break;
                    }

                    int fieldSize = window.getSize().x < window.getSize().y ? window.getSize().x / 8 : window.getSize().y / 8;

                    int boardY = (window.getSize().y - fieldSize * 8) / 2;
                    int boardX = (window.getSize().x - fieldSize * 8) / 2;

                    float scale = (float)fieldSize / pieceTextures[0].getSize().x;

                    pieces[row][col].piece.setScale(scale, scale); // Skalieren Sie die Größe der Sprites
                    pieces[row][col].piece.setPosition(boardX + col * fieldSize, boardY + row * fieldSize);
                    col++;
                    break;
                }
            }
        }
    }

    void reinterpret(){
        std::string token = position.substr(position.find(' '),position.length()-1);
        int count=1;
        std::string tmp="";
        for (int i=0;i<8;i++){
            for (int j=0;j<8;j++){
                if (board[i][j].getGlobalBounds().intersects(pieces[i][j].piece.getGlobalBounds())){
                    switch (pieces[i][j].type){
                        case PieceType::EMPTY:
                            break;
                        case PieceType::WHITE_KING:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="K";
                           count=0;
                           break; 
                        case PieceType::WHITE_QUEEN:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="Q";
                           count=0;
                           break;
                        case PieceType::WHITE_ROOK:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="R";
                           count=0;
                           break;
                        case  PieceType::WHITE_BISHOP:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="B";
                           count=0;
                           break;
                        case PieceType::WHITE_KNIGHT:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="N";
                           count=0;
                           break;
                        case PieceType::WHITE_PAWN:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="P";
                           count=0;
                           break;
                        case PieceType::BLACK_KING:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="k";
                           count=0;
                           break; 
                        case PieceType::BLACK_QUEEN:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="q";
                           count=0;
                           break;
                        case PieceType::BLACK_ROOK:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="r";
                           count=0;
                           break;
                        case PieceType::BLACK_BISHOP:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="b";
                           count=0;
                           break;
                        case PieceType::BLACK_KNIGHT:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="n";
                           count=0;
                           break;
                        case PieceType::BLACK_PAWN:
                           count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                           tmp+="p";
                           count=0;
                           break;
                    }
                }
                count++;
                if (j==7){
                    count!=1 ? tmp+=std::to_string(count-1) : tmp+="";
                    i!=7 ? tmp+="/" : tmp+="";
                    count=1;
                }
            }
        }
        tmp+=token;
        std::cout << tmp << std::endl;
    }

    bool allowedMove(){
        return true;
    }
    
    void drawBoard()
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                window.draw(board[i][j]);
            }
        }
    }

    void drawPieces()
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                window.draw(pieces[i][j].piece);
            }
        }
    }

    sf::Vector2i getCoordinateIndex(int x, int y)
    {
        float fieldSize = window.getSize().x < window.getSize().y ? window.getSize().x / 8 : window.getSize().y / 8;
        int boardY = (window.getSize().y - fieldSize * 8) / 2;
        int boardX = (window.getSize().x - fieldSize * 8) / 2;

        int row = (y - boardY) / fieldSize;
        int col = (x - boardX) / fieldSize;

        return sf::Vector2(col, row);
    }

    void run()
    {
        interpret();
        sf::Event event;
        bool isMoving = false;
        Piece* movingPiece = nullptr;
        int pieceStartPositionX;
        int pieceStartPositionY;
        int pieceStartIndexX;
        int pieceStartIndexY;


        while (window.isOpen())
        {
            window.clear();
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Resized)
                {
                    sf::FloatRect view(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(view));
                    initializeBoard();
                    interpret();
                }

                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }

                // Listen for mouse drag
                if (event.type == sf::Event::MouseButtonPressed && !isMoving)
                {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    // Get piece size
                    int fieldSize = window.getSize().x < window.getSize().y ? window.getSize().x / 8 : window.getSize().y / 8;
                    
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            if (pieces[i][j].type != PieceType::EMPTY && pieces[i][j].piece.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                            {
                                isMoving = true;
                                movingPiece = &pieces[i][j];
                                pieceStartPositionX = movingPiece->piece.getPosition().x;
                                pieceStartPositionY = movingPiece->piece.getPosition().y;
                                pieceStartIndexX = i;
                                pieceStartIndexY = j;
                            }
                        }
                    }
                }


                if (isMoving){
                    initializeBoard();
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    movingPiece->piece.setPosition(mousePosition.x - movingPiece->piece.getGlobalBounds().width / 2, mousePosition.y - movingPiece->piece.getGlobalBounds().height / 2);
                    
                    sf::Vector2i vector = getCoordinateIndex(mousePosition.x, mousePosition.y);

                    sf::RectangleShape* highlightedField = &board[vector.y][vector.x];

                    highlightedField->setFillColor(sf::Color(186,202,68));
                }

                if (event.type == sf::Event::MouseButtonReleased && isMoving)
                {
                    isMoving = false;
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                    // Get the target field given the current mouse position
                    int targetRow = (mousePosition.y - (window.getSize().y - window.getSize().x) / 2) / (window.getSize().x / 8);
                    int targetCol = (mousePosition.x - (window.getSize().x - window.getSize().y) / 2) / (window.getSize().y / 8);

                    printf("Piece Type: %d\n", pieces[targetRow][targetCol].type);
                    // Check if the target field is valid
                    if (targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8)
                    {
                        // TODO: Check if the move is valid
                        // Move the piece to the target field
                        movingPiece->piece.setPosition(targetCol * window.getSize().x / 8, targetRow * window.getSize().y / 8);
                        //TODO: Empty does not work!
                        // if (pieces[targetRow][targetCol].type == PieceType::EMPTY && pieceStartIndexX != targetRow && pieceStartIndexY != targetCol) {
                            pieces[targetRow][targetCol] = *movingPiece;
                            pieces[targetRow][targetCol].piece.setPosition(targetCol * window.getSize().x / 8, targetRow * window.getSize().y / 8);
                            pieces[targetRow][targetCol].piece.setScale((float)window.getSize().x / 8 / pieceTextures[0].getSize().x, (float)window.getSize().y / 8 / pieceTextures[0].getSize().y);
                            movingPiece->type = PieceType::EMPTY;
                            printf("Piece Type: %d\n", pieces[targetRow][targetCol].type);
                            movingPiece->piece.setPosition(pieceStartPositionX, pieceStartPositionY);
                            printf("Piece Type bro: %d\n", movingPiece->type);
                            movingPiece->piece.setTextureRect(sf::IntRect());
                            reinterpret();
                        // }
                    }
                    else
                    {
                        // Move the piece back to its original position
                        movingPiece->piece.setPosition(movingPiece->piece.getPosition().x, movingPiece->piece.getPosition().y);
                    }
                    
                    
                    printf("Mouse position: %d, %d\n", mousePosition.x, mousePosition.y);
                    movingPiece->piece.setPosition(mousePosition.x - movingPiece->piece.getGlobalBounds().width / 2, mousePosition.y - movingPiece->piece.getGlobalBounds().height / 2);
                    movingPiece = nullptr;
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