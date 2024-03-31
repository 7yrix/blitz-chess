#include "utils/filesystem.cpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
    int x;
    int y;
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
    bool isAllowed;
    int boardPos[8][8] = {{0}};

    Game()
        : window(sf::VideoMode(800, 800), "Blitz Chess"), position("rnbqkbnr/pppppppp/8/2R5/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
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
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=2;
                        break;
                    case 'K':
                        pieces[row][col].piece.setTexture(pieceTextures[6]); // z. B. Textur für weißen König
                        pieces[row][col].type=PieceType::WHITE_KING;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;   
                        boardPos[row][col]=1;                     
                        break;
                    case 'q':
                        pieces[row][col].piece.setTexture(pieceTextures[1]); // z. B. Textur für schwarze Dame
                        pieces[row][col].type=PieceType::BLACK_QUEEN;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=2;
                        break;
                    case 'Q':
                        pieces[row][col].piece.setTexture(pieceTextures[7]); // z. B. Textur für weiße Dame
                        pieces[row][col].type=PieceType::WHITE_QUEEN;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=1;
                        break;
                    case 'r':
                        pieces[row][col].piece.setTexture(pieceTextures[2]); // z. B. Textur für schwarzen Turm
                        pieces[row][col].type=PieceType::BLACK_ROOK;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=2;
                        break;
                    case 'R':
                        pieces[row][col].piece.setTexture(pieceTextures[8]); // z. B. Textur für weißen Turm
                        pieces[row][col].type=PieceType::WHITE_ROOK;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=1;
                        break;
                    case 'b':
                        pieces[row][col].piece.setTexture(pieceTextures[3]); // z. B. Textur für schwarzen Läufer
                        pieces[row][col].type=PieceType::BLACK_BISHOP;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=2;
                        break;
                    case 'B':
                        pieces[row][col].piece.setTexture(pieceTextures[9]); // z. B. Textur für weißen Läufer
                        pieces[row][col].type=PieceType::WHITE_BISHOP;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=1;
                        break;
                    case 'n':
                        pieces[row][col].piece.setTexture(pieceTextures[4]); // z. B. Textur für schwarzen Springer
                        pieces[row][col].type=PieceType::BLACK_KNIGHT;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=2;
                        break;
                    case 'N':
                        pieces[row][col].piece.setTexture(pieceTextures[10]); // z. B. Textur für weißen Springer
                        pieces[row][col].type=PieceType::WHITE_KNIGHT;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=1;
                        break;
                    case 'p':
                        pieces[row][col].piece.setTexture(pieceTextures[5]); // z. B. Textur für schwarzen Bauer
                        pieces[row][col].type=PieceType::BLACK_PAWN;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=2;
                        break;
                    case 'P':
                        pieces[row][col].piece.setTexture(pieceTextures[11]); // z. B. Textur für weißen Bauer
                        pieces[row][col].type=PieceType::WHITE_PAWN;
                        pieces[row][col].x=col;
                        pieces[row][col].y=row;
                        boardPos[row][col]=1;
                        break;
                    default:
                        pieces[row][col].type = PieceType::EMPTY;
                        boardPos[row][col]=0;
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

    bool isMoveInList(const std::vector<std::tuple<int, int>>& moves, int target_x, int target_y) {
        return std::find_if(moves.begin(), moves.end(), [&](const auto& tuple) {
            return std::get<0>(tuple) == target_x && std::get<1>(tuple) == target_y;
        }) != moves.end();
    }

    std::vector<std::tuple<int, int>> generatePossibleMoves(Piece& piece){
        std::vector<std::tuple<int, int>> moves;
        switch (piece.type){
            case PieceType::WHITE_KING:
                break;
            case PieceType::BLACK_KING:
                break;
            case PieceType::WHITE_ROOK:
                for (int i=piece.x+1;i<8;i++){
                    if (boardPos[piece.y][i]==0){
                        moves.push_back(std::make_tuple(piece.y, i)); 
                    }
                    else if(boardPos[piece.y][i]==2){
                        moves.push_back(std::make_tuple(piece.y, i));
                        break;
                    }
                    else{
                        break;
                    }
                }
                for (int i=piece.x-1;i>=0;i--){
                    if (boardPos[piece.y][i]==0){
                        moves.push_back(std::make_tuple(piece.y, i)); 
                    }
                    else if(boardPos[piece.y][i]==2){
                        moves.push_back(std::make_tuple(piece.y, i));
                        break;
                    }
                    else{
                        break;
                    }
                }
                for (int i=piece.y+1;i<8;i++){
                    if (boardPos[i][piece.x]==0){
                        moves.push_back(std::make_tuple(i, piece.x)); 
                    }
                    else if(boardPos[i][piece.x]==2){
                        moves.push_back(std::make_tuple(i, piece.x));
                        break;
                    }
                    else{
                        break;
                    }
                }
                for (int i=piece.y-1;i>=0;i--){
                    if (boardPos[i][piece.x]==0){
                        moves.push_back(std::make_tuple(i, piece.x)); 
                    }
                    else if(boardPos[i][piece.x]==2){
                        moves.push_back(std::make_tuple(i, piece.x));
                        break;
                    }
                    else{
                        break;
                    }
                }
                break;
            case PieceType::BLACK_ROOK:
                for (int i=piece.x+1;i<8;i++){
                    if (boardPos[piece.y][i]==0){
                        moves.push_back(std::make_tuple(piece.y, i)); 
                    }
                    else if(boardPos[piece.y][i]==1){
                        moves.push_back(std::make_tuple(piece.y, i));
                        break;
                    }
                    else{
                        break;
                    }
                }
                for (int i=piece.x-1;i>=0;i--){
                    if (boardPos[piece.y][i]==0){
                        moves.push_back(std::make_tuple(piece.y, i)); 
                    }
                    else if(boardPos[piece.y][i]==1){
                        moves.push_back(std::make_tuple(piece.y, i));
                        break;
                    }
                    else{
                        break;
                    }
                }
                for (int i=piece.y+1;i<8;i++){
                    if (boardPos[i][piece.x]==0){
                        moves.push_back(std::make_tuple(i, piece.x)); 
                    }
                    else if(boardPos[i][piece.x]==1){
                        moves.push_back(std::make_tuple(i, piece.x));
                        break;
                    }
                    else{
                        break;
                    }
                }
                for (int i=piece.y-1;i>=0;i--){
                    if (boardPos[i][piece.x]==0){
                        moves.push_back(std::make_tuple(i, piece.x)); 
                    }
                    else if(boardPos[i][piece.x]==1){
                        moves.push_back(std::make_tuple(i, piece.x));
                        break;
                    }
                    else{
                        break;
                    }
                }
                break;
            case PieceType::WHITE_BISHOP:
                for (int i=1;i<8;i++){//Unten Rechts
                    if (piece.y+i > 7 || piece.x+i > 7){
                        break;
                    }
                    else if (boardPos[piece.y+i][piece.x+i]==0){
                        moves.push_back(std::make_tuple(piece.y+i, piece.x+i)); 
                    }
                    else if(boardPos[piece.y+i][piece.x+i]==2){
                        moves.push_back(std::make_tuple(piece.y+i, piece.x+i));
                        break;
                    }
                    else{
                        break;
                    }
                }
                for (int i=1;i<0;i++){//Unten Links
                    if (piece.y+i > 7 || piece.x-i < 0){
                        break;
                    }
                    else if (boardPos[piece.y+i][piece.x-i]==0){
                        moves.push_back(std::make_tuple(piece.y+i, piece.x-i)); 
                    }
                    else if(boardPos[piece.y+i][piece.x-i]==2){
                        moves.push_back(std::make_tuple(piece.y+i, piece.x-i));
                        break;
                    }
                    else{
                        break;
                    }
                }
                for (int i=1;i<8;i++){//Oben Rechts
                    if (piece.y-i < 0 || piece.x+i > 7){
                        break;
                    }
                    else if (boardPos[piece.y-i][piece.x+i]==0){
                        moves.push_back(std::make_tuple(piece.y-i, piece.x+i)); 
                    }
                    else if(boardPos[piece.y-i][piece.x+i]==2){
                        moves.push_back(std::make_tuple(piece.y-i, piece.x+i));
                        break;
                    }
                    else{
                        break;
                    }
                }
                for (int i=1;i<0;i++){//Oben Links
                    if (piece.y-i < 0 || piece.x-i < 0){
                        break;
                    }
                    else if (boardPos[piece.y-i][piece.x-i]==0){
                        moves.push_back(std::make_tuple(piece.y-i, piece.x-i)); 
                    }
                    else if(boardPos[piece.y-i][piece.x-i]==2){
                        moves.push_back(std::make_tuple(piece.y-i, piece.x-i));
                        break;
                    }
                    else{
                        break;
                    }
                }
                break;
            case PieceType::BLACK_BISHOP:
                break;
            case PieceType::WHITE_KNIGHT:
                break;
            case PieceType::BLACK_KNIGHT:
                break;
            case PieceType::WHITE_QUEEN:
                break;
            case PieceType::BLACK_QUEEN:
                break;
            case PieceType::WHITE_PAWN:
                break;
            case PieceType::BLACK_PAWN:
                break;
        }  
        for (const auto& move : moves) {
                std::cout << "(" << std::get<0>(move) << ", " << std::get<1>(move) << ")" << std::endl;
        }
        return moves; 
    }

    bool allowedMove(Piece& piece, int target_x, int target_y, int x, int y){
        std::vector<std::tuple<int, int>> moves;
        switch (piece.type){
            case PieceType::WHITE_KING:
                break;
            case PieceType::BLACK_KING:
                break;
            case PieceType::WHITE_ROOK:
                moves = generatePossibleMoves(piece);
                if (isMoveInList(moves, target_y, target_x)){
                    boardPos[y][x]=0;
                    piece.y = target_y;
                    piece.x = target_x;
                    boardPos[target_y][target_x]=1;
                    return true;  
                }
                break;
            case PieceType::BLACK_ROOK:
                moves = generatePossibleMoves(piece);
                if (isMoveInList(moves, target_y, target_x)){
                    boardPos[y][x]=0;
                    piece.y = target_y;
                    piece.x = target_x;
                    boardPos[target_y][target_x]=2;
                    return true;  
                }
                break;
            case PieceType::WHITE_BISHOP:
                moves = generatePossibleMoves(piece);
                if (isMoveInList(moves, target_y, target_x)){
                    boardPos[y][x]=0;
                    piece.y = target_y;
                    piece.x = target_x;
                    boardPos[target_y][target_x]=2;
                    return true;  
                }
                break;
            case PieceType::BLACK_BISHOP:
                break;
            case PieceType::WHITE_KNIGHT:
                break;
            case PieceType::BLACK_KNIGHT:
                break;
            case PieceType::WHITE_QUEEN:
                break;
            case PieceType::BLACK_QUEEN:
                break;
            case PieceType::WHITE_PAWN:
                if (target_x == x && target_y+1 == y && boardPos[target_y][target_x]==0){//Move 1
                    boardPos[y][x]=0;
                    piece.y = y-1;
                    boardPos[target_y][target_x]=1;
                    return true;
                }
                if (target_x == x && target_y+2 == y && boardPos[target_y][target_x]==0 && y==6){//Move 2 on first Pos
                    boardPos[y][x]=0;
                    piece.y = y-2;
                    boardPos[target_y][target_x]=1;
                    return true;
                }
                if (target_x+1 == x && target_y+1 == y && boardPos[target_y][target_x]==2){//Take Left
                    boardPos[y][x]=0;
                    piece.y = y-1;
                    piece.x = x-1;
                    boardPos[target_y][target_x]=1;
                    return true;
                }
                if (target_x-1 == x && target_y+1 == y && boardPos[target_y][target_x]==2){//Take Right
                    boardPos[y][x]=0;
                    piece.y = y-1;
                    piece.x = x+1;
                    boardPos[target_y][target_x]=1;
                    return true;
                }
                return false;
                break;
            case PieceType::BLACK_PAWN:
                if (target_x == x && target_y-1 == y && boardPos[target_y][target_x]==0){//Move 1
                    boardPos[y][x]=0;
                    piece.y = y+1;
                    boardPos[target_y][target_x]=2;
                    std::cout << boardPos[y][x] << std::endl;
                    std::cout << boardPos[target_y][target_x] << std::endl;
                    return true;
                }
                if (target_x == x && target_y-2 == y && boardPos[target_y][target_x]==0 && y==1){//Move 2 on first Pos
                    boardPos[y][x]=0;
                    piece.y = y+2;
                    boardPos[target_y][target_x]=2;
                    return true;
                }
                if (target_x+1 == x && target_y-1 == y && boardPos[target_y][target_x]==1){//Take Left
                    boardPos[y][x]=0;
                    piece.y = y+1;
                    piece.x = x-1;
                    boardPos[target_y][target_x]=2;
                    return true;
                }
                if (target_x-1 == x && target_y-1 == y && boardPos[target_y][target_x]==1){//Take Right
                    boardPos[y][x]=0;
                    piece.y = y+1;
                    piece.x = x+1;
                    boardPos[target_y][target_x]=2;
                    return true;
                }
                return false;
                break;
        }
        moves.clear();
        return false;
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
        sf::RectangleShape* lastHighlightedField = nullptr;
        sf::Color lastHighlightedFieldColor;


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
                    //initializeBoard();
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

                                std::vector<std::tuple<int, int>> moves = generatePossibleMoves(*movingPiece);

                                for (const auto& move : moves) {
                                    // Highlight the move field
                                    sf::RectangleShape field = board[get<0>(move)][get<1>(move)];
                                    field.setFillColor(sf::Color(186,202,68));
                                }
                                drawBoard();
                            }
                        }
                    }
                }


                if (isMoving){
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    movingPiece->piece.setPosition(mousePosition.x - movingPiece->piece.getGlobalBounds().width / 2, mousePosition.y - movingPiece->piece.getGlobalBounds().height / 2);
                    
                    sf::Vector2i vector = getCoordinateIndex(mousePosition.x, mousePosition.y);

                    sf::RectangleShape* highlightedField = &board[vector.y][vector.x];

                    highlightedField->setFillColor(sf::Color(211, 221, 135));
                    
                    if (lastHighlightedField != nullptr) {
                        lastHighlightedField->setFillColor(lastHighlightedFieldColor);
                    }

                    lastHighlightedField = highlightedField;
                    lastHighlightedFieldColor = highlightedField->getFillColor();
                }

                if (event.type == sf::Event::MouseButtonReleased)
                {
                    isMoving = false;
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                    // Get the target field given the current mouse position
                    int targetRow = (mousePosition.y - (window.getSize().y - window.getSize().x) / 2) / (window.getSize().x / 8);
                    int targetCol = (mousePosition.x - (window.getSize().x - window.getSize().y) / 2) / (window.getSize().y / 8);

                    isAllowed = false;
                    isAllowed = allowedMove(*movingPiece, targetCol, targetRow, movingPiece->x, movingPiece->y);

                    // Check if the target field is valid
                    if (targetRow >= 0 && targetRow < 8 && targetCol >= 0 && targetCol < 8 && isAllowed)
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
                            movingPiece->piece.setPosition(pieceStartPositionX, pieceStartPositionY);
                            movingPiece->piece.setTextureRect(sf::IntRect());
                        // }
                    }
                    else
                    {
                        // Move the piece back to its original position
                        movingPiece->piece.setPosition(pieceStartPositionX, pieceStartPositionY);
                    }
                    reinterpret();
                    initializeBoard();
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