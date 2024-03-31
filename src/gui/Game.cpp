#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <string>

#include "utils/filesystem.cpp"

enum PieceType {
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

enum Color { WHITE = 1, BLACK = 0, NO_COLOR = 2 };

struct Piece {
  sf::Sprite piece;
  PieceType type;
  Color color;
  int x;
  int y;
};

std::map<char, PieceType> CharacterMap = {
    {'k', PieceType::BLACK_KING},   {'q', PieceType::BLACK_QUEEN},
    {'r', PieceType::BLACK_ROOK},   {'b', PieceType::BLACK_BISHOP},
    {'n', PieceType::BLACK_KNIGHT}, {'p', PieceType::BLACK_PAWN},
    {'K', PieceType::WHITE_KING},   {'Q', PieceType::WHITE_QUEEN},
    {'R', PieceType::WHITE_ROOK},   {'B', PieceType::WHITE_BISHOP},
    {'N', PieceType::WHITE_KNIGHT}, {'P', PieceType::WHITE_PAWN}};

std::map<PieceType, char> ReverseCharacterMap = {
		{PieceType::BLACK_KING, 'k'},   {PieceType::BLACK_QUEEN, 'q'},
		{PieceType::BLACK_ROOK, 'r'},   {PieceType::BLACK_BISHOP, 'b'},
		{PieceType::BLACK_KNIGHT, 'n'}, {PieceType::BLACK_PAWN, 'p'},
		{PieceType::WHITE_KING, 'K'},   {PieceType::WHITE_QUEEN, 'Q'},
		{PieceType::WHITE_ROOK, 'R'},   {PieceType::WHITE_BISHOP, 'B'},
		{PieceType::WHITE_KNIGHT, 'N'}, {PieceType::WHITE_PAWN, 'P'}};

std::map<char, Color> ColorMap = {
    {'k', Color::BLACK}, {'q', Color::BLACK}, {'r', Color::BLACK},
    {'b', Color::BLACK}, {'n', Color::BLACK}, {'p', Color::BLACK},
    {'K', Color::WHITE}, {'Q', Color::WHITE}, {'R', Color::WHITE},
    {'B', Color::WHITE}, {'N', Color::WHITE}, {'P', Color::WHITE}};

std::filesystem::path executableDirectory = getAbsoluteExecutableDirectory();

std::map<char, std::filesystem::path> TextureMap = {
    {'k', executableDirectory / "assets/k.png"},
    {'q', executableDirectory / "assets/q.png"},
    {'r', executableDirectory / "assets/r.png"},
    {'b', executableDirectory / "assets/b.png"},
    {'n', executableDirectory / "assets/n.png"},
    {'p', executableDirectory / "assets/p.png"},
    {'K', executableDirectory / "assets/K.png"},
    {'Q', executableDirectory / "assets/Q.png"},
    {'R', executableDirectory / "assets/R.png"},
    {'B', executableDirectory / "assets/B.png"},
    {'N', executableDirectory / "assets/N.png"},
    {'P', executableDirectory / "assets/P.png"}};

uint64_t RANK_1 = 0x00000000000000FFULL;
uint64_t RANK_2 = 0x000000000000FF00ULL;
uint64_t RANK_3 = 0x0000000000FF0000ULL;
uint64_t RANK_4 = 0x00000000FF000000ULL;
uint64_t RANK_5 = 0x000000FF00000000ULL;
uint64_t RANK_6 = 0x0000FF0000000000ULL;
uint64_t RANK_7 = 0x00FF000000000000ULL;
uint64_t RANK_8 = 0xFF00000000000000ULL;

uint64_t FILE_A = 0x0101010101010101ULL;
uint64_t FILE_B = 0x0202020202020202ULL;
uint64_t FILE_C = 0x0404040404040404ULL;
uint64_t FILE_D = 0x0808080808080808ULL;
uint64_t FILE_E = 0x1010101010101010ULL;
uint64_t FILE_F = 0x2020202020202020ULL;
uint64_t FILE_G = 0x4040404040404040ULL;
uint64_t FILE_H = 0x8080808080808080ULL;

// Get the western field on a bitboard
uint64_t west(uint64_t b) { return (b & ~FILE_A) >> 1; }
uint64_t east(uint64_t b) { return (b & ~FILE_H) << 1; }
uint64_t north_west(uint64_t b) { return (b & ~FILE_A) >> 9; }
uint64_t north_east(uint64_t b) { return (b & ~FILE_H) >> 7; }
uint64_t south_west(uint64_t b) { return (b & ~FILE_A) << 7; }
uint64_t south_east(uint64_t b) { return (b & ~FILE_H) << 9; }
uint64_t north(uint64_t b) { return b >> 8; }

// 0 0 0 1 1 0 0 0
//         ^
// 0 0 0 1 0 0 0 0

class Game {
 public:
  sf::RenderWindow window;
  std::string position;            // Position in FEN
  sf::RectangleShape board[8][8];  // Array für die Schachbrettfelder
  std::map<char, sf::Texture> textures;   // Array für die Texturen der Figuren
	sf::Font font;
  // Initialize the pieces array with -1
  Piece pieces[8][8] = {
      {{.piece = sf::Sprite(),
        .type = PieceType::EMPTY,
        .color = Color::NO_COLOR}}};  // Array für die Schachfiguren
  // Create a transparent texture for empty fields
  sf::Texture emptyTexture;
  int64_t bitboards[12];

  Game()
      : window(sf::VideoMode(800, 800), "Blitz Chess"),
        position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
		loadTextures();
		loadFont();
    initializeBoard();
  }

	void loadFont() {
		font.loadFromFile(executableDirectory / "assets/font.otf");
	}
	
	void loadTextures() {
		for (auto const& [key, val] : TextureMap) {
			sf::Texture texture;
			texture.loadFromFile(val);
			textures[key] = texture;
		}
	}

  void initializeBoard() {
		float fieldSize = getFieldSize();

    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
				sf::Vector2u position = getFieldPosition(j, i);
        board[i][j] = sf::RectangleShape(sf::Vector2f(fieldSize, fieldSize));
        board[i][j].setPosition(position.x, position.y);
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
    int row = 0;  // Starten Sie bei der letzten Reihe (Zeile 8)
    int col = 0;  // Starten Sie in der ersten Spalte (A)

    for (char c : position) {
      if (c == ' ') {
        // Informationen über die Möglichkeit der Rochade etc. erreicht
        break;
      } else if (c == '/') {
        row++;
        col = 0;
      } else {
        // Check if the character is a number
        // If it is a number, skip the number of fields
        if (isdigit(c)) {
          for (int i = 0; i < c - '0'; i++) {
            pieces[row][col].type = PieceType::EMPTY;
            pieces[row][col].color = Color::NO_COLOR;
						pieces[row][col].x = col;
						pieces[row][col].y = row;
						col++;
          }
          continue;
        } else {
          pieces[row][col].type = CharacterMap[c];
          pieces[row][col].color = ColorMap[c];
					sf::Texture& texture = textures[c];
					pieces[row][col].piece.setTexture(texture);
					pieces[row][col].x = col;
					pieces[row][col].y = row;
          float fieldSize = getFieldSize();

					sf::Vector2u position = getFieldPosition(col, row);

          float scale = (float)fieldSize / texture.getSize().x;

          pieces[row][col].piece.setScale(
              scale, scale);  // Skalieren Sie die Größe der Sprites
          pieces[row][col].piece.setPosition(position.x,
                                             position.y);
					col++;
        }
      }
    }
  }

  void reinterpret() {
    std::string token =
        position.substr(position.find(' '), position.length() - 1);
    int count = 1;
    std::string tmp = "";
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (board[i][j].getGlobalBounds().intersects(
                pieces[i][j].piece.getGlobalBounds())) {
					PieceType type = pieces[i][j].type;
					if (type != PieceType::EMPTY) {
						if (count != 1) {
							tmp += std::to_string(count);
						}
						tmp += ReverseCharacterMap[type];
						count = 0;
					}
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
    tmp += token;
    std::cout << tmp << std::endl;
  }

  void visualizeBitboard(uint64_t bitboard) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (bitboard & (1ULL << (i * 8 + j))) {
          // Color the field in red
          board[i][j].setFillColor(sf::Color::Red);
        } else {
          board[i][j].setFillColor(sf::Color::Blue);
        }
      }
    }
  }

  uint64_t getValidMoves(Piece* piece) {
    PieceType type = piece->type;

    // Create a bitboard for the piece
    uint64_t pieceBitboard = 1ULL << (piece->y * 8 + piece->x);

    // Get the bitboard for the opponent
    uint64_t opponent = 0;
    uint64_t own = 0;

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
				if (pieces[i][j].type == PieceType::EMPTY) {
					continue;
				}
        if (pieces[i][j].color != piece->color) {
          opponent |= 1ULL << (i * 8 + j);
        } else {
          own |= 1ULL << (i * 8 + j);
        }
      }
    }

		// Get all files to the right of the piece
		// TODO
		uint64_t rightFileMask = (1 >> (8 - piece->x)) - 1;

		uint64_t rightFiles = pieceBitboard >> piece->x & rightFileMask;

    uint64_t moves = 0;

    if (type == PieceType::WHITE_PAWN) {
      // Generate a bitboard for the valid moves
      moves = north(pieceBitboard) & ~(opponent | own);
      moves |= opponent & north_east(pieceBitboard);
			moves |= opponent & north_west(pieceBitboard);

			// Let the pawn move two fields if it is on the second rank
			// But not if there is a piece in front of it
			if ((pieceBitboard & RANK_7) && !((opponent | own) & north(pieceBitboard))) {
				moves |= RANK_5 & north(north(pieceBitboard)) & ~(opponent | own);
			}

    } else if (type == PieceType::WHITE_KNIGHT) {
			/*
			0 0 0 0 0 0 0 0
			0 0 0 / 0 / 0 0
			0 0 / 0 0 0 / 0
			0 0 0 0 N 0 0 0
			0 0 / 0 0 0 / 0
			0 0 0 / 0 / 0 0
			0 0 0 0 0 0 0 0
			0 0 0 0 0 0 0 0
			We start with the top right one and go clockwise
			*/

			// TODO: We need to make sure the moves don't loop around the board
			moves = pieceBitboard >> 15 & ~own;
			moves |= pieceBitboard >> 6 & ~own;
			moves |= pieceBitboard << 10 & ~own;
			moves |= pieceBitboard << 17 & ~own;
			moves |= pieceBitboard << 15 & ~own;
			moves |= pieceBitboard << 6 & ~own;
			moves |= pieceBitboard >> 10 & ~own;
			moves |= pieceBitboard >> 17 & ~own;

		} else if (type == PieceType::WHITE_BISHOP) {
			/*
			0 0 0 0 0 0 0 /
			/ 0 0 0 0 0 / 0
			0 / 0 0 0 / 0 0
			0 0 / 0 / 0 0 0
			0 0 0 B 0 0 0 0
			0 0 / 0 / 0 0 0
			0 / 0 0 0 / 0 0
			/ 0 0 0 0 0 / 0
			If there is an enemy/own piece on a field between us and the
			end of that diagonal, we can't move there.
			*/



			if (pieceBitboard >> 7 & ~(own | opponent)) {
				moves |= pieceBitboard >> 7;
				if (pieceBitboard >> 14 & ~(own | opponent)) {
					moves |= pieceBitboard >> 14;
				}
			}

			//moves = pieceBitboard << 7 & ~own;
			//moves |= pieceBitboard << 9 & ~own;
			//moves |= pieceBitboard >> 7 & ~own;
			//moves |= pieceBitboard >> 9 & ~own;
		}

		visualizeBitboard(rightFiles);

    return moves;
  }

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
				if (pieces[i][j].type != PieceType::EMPTY) {
					window.draw(pieces[i][j].piece);
				}
      }
    }
  }

  void run() {
    interpret();
    sf::Event event;
    bool isMoving = false;
    Piece* movingPiece = nullptr;
    int pieceStartPositionX;
    int pieceStartPositionY;
    int pieceStartIndexX;
    int pieceStartIndexY;

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
              if (pieces[i][j].type != PieceType::EMPTY &&
                  pieces[i][j].piece.getGlobalBounds().contains(
                      mousePosition.x, mousePosition.y)) {
                isMoving = true;
                movingPiece = &pieces[i][j];
                pieceStartPositionX = movingPiece->piece.getPosition().x;
                pieceStartPositionY = movingPiece->piece.getPosition().y;
                pieceStartIndexX = i;
                pieceStartIndexY = j;
                uint64_t validMoves = getValidMoves(movingPiece);
              }
            }
          }
        }

        if (isMoving) {
          // TODO ENABLE AGAIN
          // initializeBoard();
          sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
          movingPiece->piece.setPosition(
              mousePosition.x - movingPiece->piece.getGlobalBounds().width / 2,
              mousePosition.y -
                  movingPiece->piece.getGlobalBounds().height / 2);

          sf::RectangleShape* highlightedField =
              &board[movingPiece->y][movingPiece->x];

          // highlightedField->setFillColor(sf::Color(186,202,68));
        }

        if (event.type == sf::Event::MouseButtonReleased && isMoving) {
          isMoving = false;
          sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

          // Get the target field given the current mouse position
          int targetRow = (mousePosition.y -
                           (window.getSize().y - window.getSize().x) / 2) /
                          (window.getSize().x / 8);
          int targetCol = (mousePosition.x -
                           (window.getSize().x - window.getSize().y) / 2) /
                          (window.getSize().y / 8);

					float fieldSize = getFieldSize();

					uint64_t validMoves = getValidMoves(movingPiece);
					bool isValidMove = validMoves & (1ULL << (targetRow * 8 + targetCol));

          // Check if the target field is valid
          if (targetRow >= 0 && targetRow < 8 && targetCol >= 0 &&
              targetCol < 8 && (targetRow != movingPiece->y || targetCol != movingPiece->x) && isValidMove) {
						// TODO: Check if the move is valid
            // Move the piece to the target field
            

            // pieces[targetRow][targetCol] = *movingPiece;
						sf::Vector2u targetPosition = getFieldPosition(targetCol, targetRow);

						pieces[targetRow][targetCol].piece = movingPiece->piece;
						pieces[targetRow][targetCol].piece.setPosition(targetPosition.x, targetPosition.y);
						pieces[targetRow][targetCol].type = movingPiece->type;
						pieces[targetRow][targetCol].color = movingPiece->color;
						
						
            movingPiece->type = PieceType::EMPTY;
						sf::Vector2u position = getFieldPosition(movingPiece->x, movingPiece->y);
						movingPiece->piece.setPosition(position.x, position.y);
            movingPiece->piece.setTextureRect(sf::IntRect());
            reinterpret();
          } else {
						sf::Vector2u position = getFieldPosition(movingPiece->x, movingPiece->y);
            // Move the piece back to its original position
            movingPiece->piece.setPosition(position.x, position.y);
          }

          movingPiece = nullptr;
        }
      }
      drawBoard();
			drawFileAndRankNames();
      drawPieces();
      window.display();
    }
  }

	void drawFileAndRankNames() {
		float fieldSize = getFieldSize();
		float boardY = (window.getSize().y - fieldSize * 8) / 2 + 25;
		float boardX = (window.getSize().x - fieldSize * 8) / 2 + 25;
		for (int i = 0; i < 8; i++) {
			sf::Text text;
			text.setFont(font);
			text.setString(std::string(1, 'A' + i));
			text.setCharacterSize(24);
			text.setFillColor(sf::Color::White);
			text.setPosition(boardX + i * fieldSize + fieldSize / 2 - 10, boardY - 30);
			window.draw(text);
		}

		for (int i = 0; i < 8; i++) {
			sf::Text text;
			text.setFont(font);
			text.setString(std::to_string(8 - i));
			text.setCharacterSize(24);
			text.setFillColor(sf::Color::White);
			text.setPosition(boardX - 30, boardY + i * fieldSize + fieldSize / 2 - 10);
			window.draw(text);
		}
	}

	sf::Vector2u getFieldPosition(int x, int y) {
		float fieldSize = getFieldSize();
		float boardY = (window.getSize().y - fieldSize * 8) / 2 + 25;
		float boardX = (window.getSize().x - fieldSize * 8) / 2 + 25;
		return sf::Vector2u(boardX + x * fieldSize, boardY + y * fieldSize);
	}

	float getFieldSize() {
		return (window.getSize().x < window.getSize().y ? window.getSize().x / 8 : window.getSize().y / 8) - 50 / 8;
	}
};

int main() {
  Game main;
  main.run();
  return 0;
}