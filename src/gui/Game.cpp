#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "utils/filesystem.cpp"

using namespace std;

enum class PieceType {
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
  NONE
};

enum class Color { NONE, WHITE, BLACK };

std::map<char, PieceType> PieceMap = {
    {'K', PieceType::WHITE_KING},   {'Q', PieceType::WHITE_QUEEN},
    {'R', PieceType::WHITE_ROOK},   {'B', PieceType::WHITE_BISHOP},
    {'N', PieceType::WHITE_KNIGHT}, {'P', PieceType::WHITE_PAWN},
    {'k', PieceType::BLACK_KING},   {'q', PieceType::BLACK_QUEEN},
    {'r', PieceType::BLACK_ROOK},   {'b', PieceType::BLACK_BISHOP},
    {'n', PieceType::BLACK_KNIGHT}, {'p', PieceType::BLACK_PAWN},
    {' ', PieceType::NONE}};

std::map<PieceType, char> ReversePieceMap = {
		{PieceType::WHITE_KING, 'K'},   {PieceType::WHITE_QUEEN, 'Q'},
		{PieceType::WHITE_ROOK, 'R'},   {PieceType::WHITE_BISHOP, 'B'},
		{PieceType::WHITE_KNIGHT, 'N'}, {PieceType::WHITE_PAWN, 'P'},
		{PieceType::BLACK_KING, 'k'},   {PieceType::BLACK_QUEEN, 'q'},
		{PieceType::BLACK_ROOK, 'r'},   {PieceType::BLACK_BISHOP, 'b'},
		{PieceType::BLACK_KNIGHT, 'n'}, {PieceType::BLACK_PAWN, 'p'},
		{PieceType::NONE, ' '}};

struct Piece {
  sf::Sprite piece;
  PieceType type;
  int x;
  int y;
  Color color;
  bool enPassant;
};

class Game {
 public:
  sf::RenderWindow window;
  std::string position;            // Position in FEN
  sf::RectangleShape board[8][8];  // Array für die Schachbrettfelder
  sf::Texture pieceTextures[12];   // Array für die Texturen der Figuren
  std::map<char, sf::Texture> TextureMap;
	sf::Font font;
  // Initialize the pieces array with -1
  Piece pieces[8][8] = {{{.piece = sf::Sprite(),
                          .type = PieceType::NONE,
                          .color = Color::NONE,
                          .enPassant = false}}};  // Array für die Schachfiguren
  // Create a transparent texture for empty fields
  sf::Texture emptyTexture;
  bool isAllowed;
  bool castleK = true;
  bool castleQ = true;
  bool castlek = true;
  bool castleq = true;
  std::tuple<int, int> lastPosition;
  Color toMove = Color::WHITE;
  int whiteKingX = 4;
  int whiteKingY = 7;
  int blackKingX = 4;
  int blackKingY = 0;

  Game()
      : window(sf::VideoMode(800, 800), "Blitz Chess"),
        position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    loadTextures();
    initializeBoard();
  }

  void printMoves(const std::vector<std::tuple<int, int>>& moves) {
    for (const auto& move : moves) {
      std::cout << "(" << std::get<0>(move) << ", " << std::get<1>(move)
                << ") ";
    }
    std::cout << std::endl;
  }

  void loadTextures() {
		std::filesystem::path executableDirectory = getAbsoluteExecutableDirectory();
    for (int i = 0; i < 12; i++) {
      pieceTextures[i].loadFromFile(executableDirectory / "assets" /
                                    (std::to_string(i) + ".png"));
    }

    TextureMap = {{'K', pieceTextures[6]},  {'Q', pieceTextures[7]},
                  {'R', pieceTextures[8]},  {'B', pieceTextures[9]},
                  {'N', pieceTextures[10]}, {'P', pieceTextures[11]},
                  {'k', pieceTextures[0]},  {'q', pieceTextures[1]},
                  {'r', pieceTextures[2]},  {'b', pieceTextures[3]},
                  {'n', pieceTextures[4]},  {'p', pieceTextures[5]},
                  {' ', emptyTexture}};

		font.loadFromFile((executableDirectory / "assets" / "font.otf"));
  }

  void initializeBoard() {
    int windowWidth = window.getSize().x;
    int windowHeight = window.getSize().y;

    // Since we need 64 equally sized fields, we need to determine which size is
    // smaller
    float fieldSize =
        windowWidth < windowHeight ? windowWidth / 8 : windowHeight / 8;

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
          board[i][j].setFillColor(sf::Color(118, 150, 86));  // Dunkelgrün
        }
      }
    }
  }

  void interpret() {
    emptyTexture.create(1, 1);
    int row = 0;  // Starten Sie bei der letzten Reihe (Zeile 8)
    int col = 0;  // Starten Sie in der ersten Spalte (A)

    for (char c : position) {
      if (c == ' ') {
        // Informationen über die Möglichkeit der Rochade etc. erreicht
        break;
      } else if (c == '/') {
        // Neue Zeile erreicht, gehen Sie zur nächsten Zeile und setzen Sie die
        // Spalte auf 0 zurück
        row++;
        col = 0;
      } else {
        if (isdigit(c)) {
          for (int i = 0; i < c - '0'; i++) {
            pieces[row][col].type = PieceType::NONE;
            pieces[row][col].color = Color::NONE;
            pieces[row][col].piece.setTextureRect(sf::IntRect());
            pieces[row][col].x = col;
            pieces[row][col].y = row;
            col++;
          }

          continue;
        } else {
          // Handle white pieces
          pieces[row][col].piece.setTexture(
              TextureMap[c]);  // z. B. Textur für schwarzen König
          pieces[row][col].type = PieceMap[c];
          pieces[row][col].x = col;
          pieces[row][col].y = row;

          if (isupper(c)) {
            pieces[row][col].color = Color::WHITE;
          } else {
            pieces[row][col].color = Color::BLACK;
          }

          int fieldSize = window.getSize().x < window.getSize().y
                              ? window.getSize().x / 8
                              : window.getSize().y / 8;

          int boardY = (window.getSize().y - fieldSize * 8) / 2;
          int boardX = (window.getSize().x - fieldSize * 8) / 2;

          float scale = (float)fieldSize / pieceTextures[0].getSize().x;

          pieces[row][col].piece.setScale(
              scale, scale);  // Skalieren Sie die Größe der Sprites
          pieces[row][col].piece.setPosition(boardX + col * fieldSize,
                                             boardY + row * fieldSize);
          col++;
        }
      }
    }
  }

	// Draws the field numbers and letters
	void drawFieldLegend() {
		int fieldSize = window.getSize().x < window.getSize().y
			? window.getSize().x / 8
			: window.getSize().y / 8;

		int boardY = (window.getSize().y - fieldSize * 8) / 2;
		int boardX = (window.getSize().x - fieldSize * 8) / 2;

		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::Black);

		for (int i = 0; i < 8; i++) {
			text.setString(std::to_string(8 - i));
			text.setPosition(boardX + 5, boardY + i * fieldSize + 5);
			window.draw(text);
		}

		for (int i = 0; i < 8; i++) {
			text.setString(std::string(1, 'A' + i));
			text.setPosition(boardX + i * fieldSize + 5, boardY + 8 * fieldSize - 30);
			window.draw(text);
		}
	}

	// Returns the absolute position of a board field
	// given the x and y coordinates of the field
	sf::Vector2u getAbsoluteBoardPosition(int x, int y) {
		int fieldSize = window.getSize().x < window.getSize().y
			? window.getSize().x / 8
			: window.getSize().y / 8;

		int boardY = (window.getSize().y - fieldSize * 8) / 2;
		int boardX = (window.getSize().x - fieldSize * 8) / 2;

		return sf::Vector2u(boardX + x * fieldSize, boardY + y * fieldSize);
	}

  std::string reinterpret() {
    std::string additionalProperties =
        position.substr(position.find(' '), position.length() - 1);
    int count = 1;
    std::string tmp = "";
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
				if (pieces[i][j].type != PieceType::NONE) {
					char pieceChar = ReversePieceMap[pieces[i][j].type];

					if (count != 1) {
						tmp += std::to_string(count - 1);
					}

					tmp += pieceChar;

					count = 0;
				}

				count++;
        if (j == 7) {
					if (count != 1) {
						tmp += std::to_string(count - 1);
					}

					if (i != 7) {
						tmp += "/";
					}
          count = 1;
        }
      }
    }
    tmp += additionalProperties;
    return tmp;
  }

  bool isMoveInList(const std::vector<std::tuple<int, int>>& moves,
                    int target_x, int target_y) {
    return std::find_if(moves.begin(), moves.end(), [&](const auto& tuple) {
             return std::get<0>(tuple) == target_x &&
                    std::get<1>(tuple) == target_y;
           }) != moves.end();
  }

	/* ---------------------------- Move Generation ---------------------------- */

  std::vector<std::tuple<int, int>> generatePossibleMoves(Piece& piece) {
    std::vector<std::tuple<int, int>> moves;

    Color oppositeColor =
        piece.color == Color::WHITE ? Color::BLACK : Color::WHITE;

    switch (piece.type) {
      case PieceType::WHITE_KING:
      case PieceType::BLACK_KING:
        if (pieces[piece.y - 1][piece.x].color != piece.color &&
            piece.y - 1 >= 0) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x));
        }
        if (pieces[piece.y - 1][piece.x - 1].color != piece.color &&
            piece.y - 1 >= 0 && piece.x - 1 >= 0) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x - 1));
        }
        if (pieces[piece.y - 1][piece.x + 1].color != piece.color &&
            piece.y - 1 >= 0 && piece.x + 1 < 8) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x + 1));
        }
        if (pieces[piece.y][piece.x - 1].color != piece.color &&
            piece.x - 1 >= 0) {
          moves.push_back(std::make_tuple(piece.y, piece.x - 1));
        }
        if (pieces[piece.y][piece.x + 1].color != piece.color &&
            piece.x + 1 < 8) {
          moves.push_back(std::make_tuple(piece.y, piece.x + 1));
        }
        if (pieces[piece.y + 1][piece.x].color != piece.color &&
            piece.y + 1 < 8) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x));
        }
        if (pieces[piece.y + 1][piece.x + 1].color != piece.color &&
            piece.y + 1 < 8 && piece.x + 1 < 8) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x + 1));
        }
        if (pieces[piece.y + 1][piece.x - 1].color != piece.color &&
            piece.y + 1 < 8 && piece.x - 1 >= 0) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x - 1));
        }
        if (piece.type == PieceType::BLACK_KING && castlek &&
            pieces[piece.y][piece.x + 1].type == PieceType::NONE &&
            pieces[piece.y][piece.x + 2].type == PieceType::NONE) {
          moves.push_back(std::make_tuple(piece.y, piece.x + 2));
        }
        if (piece.type == PieceType::BLACK_KING && castleq &&
            pieces[piece.y][piece.x - 1].type == PieceType::NONE &&
            pieces[piece.y][piece.x - 2].type == PieceType::NONE &&
            pieces[piece.y][piece.x - 3].type == PieceType::NONE) {
          moves.push_back(std::make_tuple(piece.y, piece.x - 2));
        }
        if (piece.type == PieceType::WHITE_KING && castleK &&
            pieces[piece.y][piece.x + 1].type == PieceType::NONE &&
            pieces[piece.y][piece.x + 2].type == PieceType::NONE) {
          moves.push_back(std::make_tuple(piece.y, piece.x + 2));
        }
        if (piece.type == PieceType::WHITE_KING && castleQ &&
            pieces[piece.y][piece.x - 1].type == PieceType::NONE &&
            pieces[piece.y][piece.x - 2].type == PieceType::NONE &&
            pieces[piece.y][piece.x - 3].type == PieceType::NONE) {
          moves.push_back(std::make_tuple(piece.y, piece.x - 2));
        }
        break;
      case PieceType::WHITE_ROOK:
      case PieceType::BLACK_ROOK:
        for (int i = piece.x + 1; i < 8; i++) {
          if (pieces[piece.y][i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y, i));
          } else if (pieces[piece.y][i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y, i));
            break;
          } else {
            break;
          }
        }
        for (int i = piece.x - 1; i >= 0; i--) {
          if (pieces[piece.y][i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y, i));
          } else if (pieces[piece.y][i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y, i));
            break;
          } else {
            break;
          }
        }
        for (int i = piece.y + 1; i < 8; i++) {
          if (pieces[i][piece.x].color == Color::NONE) {
            moves.push_back(std::make_tuple(i, piece.x));
          } else if (pieces[i][piece.x].color == oppositeColor) {
            moves.push_back(std::make_tuple(i, piece.x));
            break;
          } else {
            break;
          }
        }
        for (int i = piece.y - 1; i >= 0; i--) {
          if (pieces[i][piece.x].color == Color::NONE) {
            moves.push_back(std::make_tuple(i, piece.x));
          } else if (pieces[i][piece.x].color == oppositeColor) {
            moves.push_back(std::make_tuple(i, piece.x));
            break;
          } else {
            break;
          }
        }
        break;
      case PieceType::WHITE_BISHOP:
      case PieceType::BLACK_BISHOP:
        for (int i = 1; i < 8; i++) {  // Unten Rechts
          if (piece.y + i > 7 || piece.x + i > 7) {
            break;
          } else if (pieces[piece.y + i][piece.x + i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y + i, piece.x + i));
          } else if (pieces[piece.y + i][piece.x + i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y + i, piece.x + i));
            break;
          } else {
            break;
          }
        }
        for (int i = 1; i < 8; i++) {  // Unten Links
          if (piece.y + i > 7 || piece.x - i < 0) {
            break;
          } else if (pieces[piece.y + i][piece.x - i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y + i, piece.x - i));
          } else if (pieces[piece.y + i][piece.x - i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y + i, piece.x - i));
            break;
          } else {
            break;
          }
        }
        for (int i = 1; i < 8; i++) {  // Oben Rechts
          if (piece.y - i < 0 || piece.x + i > 7) {
            break;
          } else if (pieces[piece.y - i][piece.x + i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y - i, piece.x + i));
          } else if (pieces[piece.y - i][piece.x + i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y - i, piece.x + i));
            break;
          } else {
            break;
          }
        }
        for (int i = 1; i < 8; i++) {  // Oben Links
          if (piece.y - i < 0 || piece.x - i < 0) {
            break;
          } else if (pieces[piece.y - i][piece.x - i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y - i, piece.x - i));
          } else if (pieces[piece.y - i][piece.x - i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y - i, piece.x - i));
            break;
          } else {
            break;
          }
        }
        break;
      case PieceType::WHITE_KNIGHT:
      case PieceType::BLACK_KNIGHT:
        if (pieces[piece.y - 1][piece.x + 2].color != piece.color &&
            piece.y - 1 >= 0 && piece.x + 2 < 8) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x + 2));
        }
        if (pieces[piece.y - 1][piece.x - 2].color != piece.color &&
            piece.y - 1 >= 0 && piece.x - 2 >= 0) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x - 2));
        }
        if (pieces[piece.y + 1][piece.x + 2].color != piece.color &&
            piece.y + 1 < 8 && piece.x + 2 < 8) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x + 2));
        }
        if (pieces[piece.y + 1][piece.x - 2].color != piece.color &&
            piece.y + 1 < 8 && piece.x - 2 >= 0) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x - 2));
        }
        if (pieces[piece.y - 2][piece.x + 1].color != piece.color &&
            piece.y - 2 >= 0 && piece.x + 1 < 8) {
          moves.push_back(std::make_tuple(piece.y - 2, piece.x + 1));
        }
        if (pieces[piece.y - 2][piece.x - 1].color != piece.color &&
            piece.y - 2 >= 0 && piece.x - 1 >= 0) {
          moves.push_back(std::make_tuple(piece.y - 2, piece.x - 1));
        }
        if (pieces[piece.y + 2][piece.x + 1].color != piece.color &&
            piece.y + 2 < 8 && piece.x + 1 < 8) {
          moves.push_back(std::make_tuple(piece.y + 2, piece.x + 1));
        }
        if (pieces[piece.y + 2][piece.x - 1].color != piece.color &&
            piece.y + 2 < 8 && piece.x - 1 >= 0) {
          moves.push_back(std::make_tuple(piece.y + 2, piece.x - 1));
        }
        break;
      case PieceType::WHITE_QUEEN:
      case PieceType::BLACK_QUEEN:
        for (int i = piece.x + 1; i < 8; i++) {
          if (pieces[piece.y][i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y, i));
          } else if (pieces[piece.y][i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y, i));
            break;
          } else {
            break;
          }
        }
        for (int i = piece.x - 1; i >= 0; i--) {
          if (pieces[piece.y][i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y, i));
          } else if (pieces[piece.y][i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y, i));
            break;
          } else {
            break;
          }
        }
        for (int i = piece.y + 1; i < 8; i++) {
          if (pieces[i][piece.x].color == Color::NONE) {
            moves.push_back(std::make_tuple(i, piece.x));
          } else if (pieces[i][piece.x].color == oppositeColor) {
            moves.push_back(std::make_tuple(i, piece.x));
            break;
          } else {
            break;
          }
        }
        for (int i = piece.y - 1; i >= 0; i--) {
          if (pieces[i][piece.x].color == Color::NONE) {
            moves.push_back(std::make_tuple(i, piece.x));
          } else if (pieces[i][piece.x].color == oppositeColor) {
            moves.push_back(std::make_tuple(i, piece.x));
            break;
          } else {
            break;
          }
        }
        for (int i = 1; i < 8; i++) {  // Unten Rechts
          if (piece.y + i > 7 || piece.x + i > 7) {
            break;
          } else if (pieces[piece.y + i][piece.x + i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y + i, piece.x + i));
          } else if (pieces[piece.y + i][piece.x + i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y + i, piece.x + i));
            break;
          } else {
            break;
          }
        }
        for (int i = 1; i < 8; i++) {  // Unten Links
          if (piece.y + i > 7 || piece.x - i < 0) {
            break;
          } else if (pieces[piece.y + i][piece.x - i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y + i, piece.x - i));
          } else if (pieces[piece.y + i][piece.x - i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y + i, piece.x - i));
            break;
          } else {
            break;
          }
        }
        for (int i = 1; i < 8; i++) {  // Oben Rechts
          if (piece.y - i < 0 || piece.x + i > 7) {
            break;
          } else if (pieces[piece.y - i][piece.x + i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y - i, piece.x + i));
          } else if (pieces[piece.y - i][piece.x + i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y - i, piece.x + i));
            break;
          } else {
            break;
          }
        }
        for (int i = 1; i < 8; i++) {  // Oben Links
          if (piece.y - i < 0 || piece.x - i < 0) {
            break;
          } else if (pieces[piece.y - i][piece.x - i].color == Color::NONE) {
            moves.push_back(std::make_tuple(piece.y - i, piece.x - i));
          } else if (pieces[piece.y - i][piece.x - i].color == oppositeColor) {
            moves.push_back(std::make_tuple(piece.y - i, piece.x - i));
            break;
          } else {
            break;
          }
        }
        break;
      case PieceType::WHITE_PAWN:
        if (piece.y == 6 &&
            pieces[piece.y - 2][piece.x].type == PieceType::NONE &&
            pieces[piece.y - 1][piece.x].type == PieceType::NONE) {
          moves.push_back(std::make_tuple(piece.y - 2, piece.x));
        }
        if (pieces[piece.y - 1][piece.x].type == PieceType::NONE &&
            (piece.y - 1) >= 0) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x));
        }
        if (pieces[piece.y - 1][piece.x + 1].color == oppositeColor &&
            (piece.y - 1) >= 0) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x + 1));
        }
        if (pieces[piece.y - 1][piece.x - 1].color == oppositeColor &&
            (piece.y - 1) >= 0) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x - 1));
        }
        if (pieces[piece.y][piece.x + 1].color == oppositeColor &&
            (piece.y - 1) > 0 &&
            pieces[piece.y][piece.x + 1].enPassant == true) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x + 1));
        }
        if (pieces[piece.y][piece.x - 1].color == oppositeColor &&
            (piece.y - 1) > 0 &&
            pieces[piece.y][piece.x - 1].enPassant == true) {
          moves.push_back(std::make_tuple(piece.y - 1, piece.x - 1));
        }
        break;
      case PieceType::BLACK_PAWN:
        if (piece.y == 1 &&
            pieces[piece.y + 2][piece.x].type == PieceType::NONE &&
            pieces[piece.y + 1][piece.x].type == PieceType::NONE) {
          moves.push_back(std::make_tuple(piece.y + 2, piece.x));
        }
        if (pieces[piece.y + 1][piece.x].type == PieceType::NONE &&
            (piece.y + 1) > 0) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x));
        }
        if (pieces[piece.y + 1][piece.x + 1].color == oppositeColor &&
            (piece.y + 1) > 0) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x + 1));
        }
        if (pieces[piece.y + 1][piece.x - 1].color == oppositeColor &&
            (piece.y + 1) > 0) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x - 1));
        }
        if (pieces[piece.y][piece.x + 1].color == oppositeColor &&
            (piece.y + 1) > 0 &&
            pieces[piece.y][piece.x + 1].enPassant == true) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x + 1));
        }
        if (pieces[piece.y][piece.x - 1].color == oppositeColor &&
            (piece.y + 1) > 0 &&
            pieces[piece.y][piece.x - 1].enPassant == true) {
          moves.push_back(std::make_tuple(piece.y + 1, piece.x - 1));
        }
        break;
    }
    return moves;
  }

	/* ---------------------------- Move Validation ---------------------------- */

  bool allowedMove(Piece& piece, int target_x, int target_y,
                   std::vector<std::tuple<int, int>> moves) {
    if (isMoveInList(moves, target_y, target_x)) {
      if (piece.color == Color::BLACK && toMove == Color::WHITE ||
          piece.color == Color::WHITE && toMove == Color::BLACK) {
        return false;
      }
      if (piece.x == target_x && piece.y == target_y) {
        return false;
      }
      // Reset old position to color none
      if (piece.type == PieceType::BLACK_PAWN && piece.y == target_y - 2) {
        piece.enPassant = true;
      } else if (piece.type == PieceType::WHITE_PAWN &&
                 piece.y == target_y + 2) {
        piece.enPassant = true;
      }
      if (piece.type == PieceType::WHITE_PAWN && target_x == piece.x + 1 &&
              pieces[get<0>(lastPosition)][get<1>(lastPosition)].enPassant ==
                  true ||
          piece.type == PieceType::WHITE_PAWN && target_x == piece.x - 1 &&
              pieces[get<0>(lastPosition)][get<1>(lastPosition)].enPassant ==
                  true) {
        pieces[get<0>(lastPosition)][get<1>(lastPosition)].type =
            PieceType::NONE;
        pieces[get<0>(lastPosition)][get<1>(lastPosition)].color = Color::NONE;
      }
      if (piece.type == PieceType::BLACK_PAWN && target_x == piece.x + 1 &&
              pieces[get<0>(lastPosition)][get<1>(lastPosition)].enPassant ==
                  true ||
          piece.type == PieceType::BLACK_PAWN && target_x == piece.x - 1 &&
              pieces[get<0>(lastPosition)][get<1>(lastPosition)].enPassant ==
                  true) {
        pieces[get<0>(lastPosition)][get<1>(lastPosition)].type =
            PieceType::NONE;
        pieces[get<0>(lastPosition)][get<1>(lastPosition)].color = Color::NONE;
      }
      if (piece.type == PieceType::BLACK_KING && target_x == piece.x + 2) {  // k
        pieces[0][5] = pieces[0][7];
        pieces[0][7].color = Color::NONE;
        pieces[0][7].type = PieceType::NONE;
      }
      if (piece.type == PieceType::BLACK_ROOK && piece.x == 7) {
        castlek = false;
      }
      if (piece.type == PieceType::BLACK_KING && target_x == piece.x - 2) {  // q
        pieces[0][3] = pieces[0][0];
        pieces[0][0].color = Color::NONE;
        pieces[0][0].type = PieceType::NONE;
      }
      if (piece.type == PieceType::BLACK_ROOK && piece.x == 0) {
        castlek = false;
      }
      if (piece.type == PieceType::WHITE_KING && target_x == piece.x + 2) {  // K
        pieces[7][5] = pieces[7][7];
        pieces[7][7].color = Color::NONE;
        pieces[7][7].type = PieceType::NONE;
      }
      if (piece.type == PieceType::WHITE_ROOK && piece.x == 7) {
        castleK = false;
      }
      if (piece.type == PieceType::WHITE_KING && target_x == piece.x - 2) {  // Q
        pieces[7][3] = pieces[7][0];
        pieces[7][0].color = Color::NONE;
        pieces[7][0].type = PieceType::NONE;
      }
      if (piece.type == PieceType::WHITE_ROOK && piece.x == 0) {
        castleQ = false;
      }
      if (piece.type == PieceType::BLACK_KING) {
        castlek = false;
        castleq = false;
        blackKingX = target_x;
        blackKingY = target_y;
      }
      if (piece.type == PieceType::WHITE_KING) {
        castleK = false;
        castleQ = false;
        whiteKingX = target_x;
        whiteKingY = target_y;
      }
      pieces[target_y][target_x].color = piece.color;
      pieces[target_y][target_x].type = piece.type;
      pieces[piece.y][piece.x].color = Color::NONE;
      pieces[piece.y][piece.x].type = PieceType::NONE;
      Color tmp = toMove == Color::BLACK ? Color::WHITE : Color::BLACK;
      std::vector<std::tuple<int, int>> check = movesList(tmp);
      if (toMove == Color::BLACK &&
          isMoveInList(check, blackKingY, blackKingX)) {
        return false;
      }
      if (toMove == Color::WHITE &&
          isMoveInList(check, whiteKingY, whiteKingX)) {
        return false;
      }
      pieces[get<0>(lastPosition)][get<1>(lastPosition)].enPassant = false;
      lastPosition = std::make_tuple(target_y, target_x);
      position = reinterpret();
      toMove == Color::BLACK ? toMove = Color::WHITE : toMove = Color::BLACK;
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          std::cout << static_cast<int>(pieces[i][j].type) << " ";
        }
        cout << endl;
      }
      cout << "-----------------" << endl;
      return true;
    }
    // NOTE: Maybe important???????
    // moves.clear();
    return false;
  }

  std::vector<std::tuple<int, int>> movesList(Color color) {
    std::vector<std::tuple<int, int>> tmp;
    std::vector<std::tuple<int, int>> list;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (pieces[i][j].color == color) {
          tmp = generatePossibleMoves(pieces[i][j]);
          list.insert(list.end(), tmp.begin(), tmp.end());
        }
      }
    }
    return list;
  }

	/* ------------------------------- Rendering ------------------------------- */

  void drawBoard() {
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        window.draw(board[i][j]);
      }
    }
  }

  void drawPieces() {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        window.draw(pieces[i][j].piece);
      }
    }
  }

  sf::Vector2i getCoordinateIndex(int x, int y) {
    float fieldSize = getFieldSize();
    int boardY = (window.getSize().y - fieldSize * 8) / 2;
    int boardX = (window.getSize().x - fieldSize * 8) / 2;

    int row = (y - boardY) / fieldSize;
    int col = (x - boardX) / fieldSize;

    return sf::Vector2(col, row);
  }

	float getFieldSize() {
		return window.getSize().x < window.getSize().y
			? window.getSize().x / 8
			: window.getSize().y / 8;
	}

  void run() {
    interpret();
    sf::Event event;
    bool isMoving = false;
    Piece* movingPiece = nullptr;
    sf::RectangleShape* lastHighlightedField = nullptr;
    sf::Color lastHighlightedFieldColor;

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

        // Listen for mouse drag
        if (event.type == sf::Event::MouseButtonPressed && !isMoving) {
          sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
          // Get piece size
          int fieldSize = window.getSize().x < window.getSize().y
                              ? window.getSize().x / 8
                              : window.getSize().y / 8;

          for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
              if (pieces[i][j].type != PieceType::NONE &&
                  pieces[i][j].piece.getGlobalBounds().contains(
                      mousePosition.x, mousePosition.y)) {
                isMoving = true;
                movingPiece = &pieces[i][j];

                std::vector<std::tuple<int, int>> moves =
                    generatePossibleMoves(*movingPiece);

                for (const auto& move : moves) {
                  // Highlight the move field
                  sf::RectangleShape& field = board[get<0>(move)][get<1>(move)];
                  field.setFillColor(sf::Color(186, 202, 68));
                }
              }
            }
          }
        }

        if (isMoving) {
          // sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
          // movingPiece->piece.setPosition(mousePosition.x -
          // movingPiece->piece.getGlobalBounds().width / 2, mousePosition.y -
          // movingPiece->piece.getGlobalBounds().height / 2);

          // sf::Vector2i vector = getCoordinateIndex(mousePosition.x,
          // mousePosition.y);

          // sf::RectangleShape* highlightedField = &board[vector.y][vector.x];

          // highlightedField->setFillColor(sf::Color(211, 221, 135));

          // if (lastHighlightedField != nullptr) {
          //     lastHighlightedField->setFillColor(lastHighlightedFieldColor);
          // }

          // lastHighlightedField = highlightedField;
          // lastHighlightedFieldColor = highlightedField->getFillColor();
        }

				// Listen for mouse drop event
        if (event.type == sf::Event::MouseButtonReleased) {
          isMoving = false;
          sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

          // Get the target field given the current mouse position
          int targetRow = (mousePosition.y -
                           (window.getSize().y - window.getSize().x) / 2) /
                          (window.getSize().x / 8);
          int targetCol = (mousePosition.x -
                           (window.getSize().x - window.getSize().y) / 2) /
                          (window.getSize().y / 8);

          isAllowed = false;
          isAllowed = allowedMove(*movingPiece, targetCol, targetRow,
                                  generatePossibleMoves(*movingPiece));

          // Check if the target field is valid
          if (isAllowed) {
            // Move the piece to the target field
            pieces[targetRow][targetCol].color = movingPiece->color;
						pieces[targetRow][targetCol].type = movingPiece->type;
						pieces[targetRow][targetCol].enPassant = movingPiece->enPassant;

            movingPiece->type = PieceType::NONE;
            movingPiece->color = Color::NONE;
						movingPiece->enPassant = false;
          }
					movingPiece = nullptr;
          initializeBoard();
          interpret();
        }
      }
      drawBoard();
      drawPieces();
			drawFieldLegend();
      window.display();
    }
  }
};

int main() {
  Game main;
  main.run();
  return 0;
}