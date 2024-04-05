#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "MoveGeneration.h"
#include "types.h"
#include "utils/filesystem.cpp"

using namespace std;

// GAME

class Game {
 public:
  sf::RenderWindow window;
  std::string position;            // Position in FEN
  sf::RectangleShape board[8][8];  // Array für die Schachbrettfelder
  sf::Sprite sprites[12];          // Array für die Texturen der Figuren
  sf::Texture texture;
  std::map<char, sf::Sprite> TextureMap;
  sf::Font font;
  // Initialize the pieces array with -1
  Piece pieces[8][8] = {{{.piece = sf::Sprite(),
                          .type = PieceType::NONE,
                          .color = Color::NONE,
                          .enPassant = false}}};  // Array für die Schachfiguren
  // Create a transparent texture for empty fields
  sf::Texture emptyTexture;
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

 void printMovesFull(const std::vector<std::tuple<int, int, int, int>>& moves) {
    for (const auto& move : moves) {
			printf("From: (%d, %d) To: (%d, %d)\n", std::get<2>(move), std::get<3>(move), std::get<0>(move), std::get<1>(move));
    }
  }

	void printMovesAsBoard(const std::vector<std::tuple<int, int, int, int>>& moves) {
		// Simulate actually playing the move then print the resulting board.
		Piece piecesCopy[8][8];

		for (const auto& move : moves) {
			std::copy(&pieces[0][0], &pieces[0][0]+8*8, &piecesCopy[0][0]);
			Piece movedTo = piecesCopy[std::get<0>(move)][std::get<1>(move)];
			Piece movedFrom = piecesCopy[std::get<2>(move)][std::get<3>(move)];

			piecesCopy[std::get<0>(move)][std::get<1>(move)].color = movedFrom.color;
			piecesCopy[std::get<0>(move)][std::get<1>(move)].type = movedFrom.type;
			piecesCopy[std::get<0>(move)][std::get<1>(move)].enPassant = movedFrom.enPassant;
			piecesCopy[std::get<2>(move)][std::get<3>(move)].color = movedTo.color;
			piecesCopy[std::get<2>(move)][std::get<3>(move)].type = movedTo.type;
			piecesCopy[std::get<2>(move)][std::get<3>(move)].enPassant = movedTo.enPassant;

			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					printf("%ls ", piecesCopy[i][j].print());
				}
				cout << endl;
			}
			printf("---------------\n");
		}
	}


  void loadTextures() {
    std::filesystem::path executableDirectory =
        getAbsoluteExecutableDirectory();

    texture.loadFromFile(executableDirectory / "../../assets" / "pieces.png");

    // Load all sprites from single texture file
    for (int i; i < 12; i++) {
      sprites[i].setTexture(texture);
      sprites[i].setTextureRect(
          sf::IntRect(i * PIECE_WIDTH, 0, PIECE_WIDTH, PIECE_HEIGHT));
    }

    sf::Sprite empty = sf::Sprite();
    emptyTexture.loadFromFile(executableDirectory / "../../assets" / "empty.png");
    empty.setTexture(emptyTexture);

    TextureMap = {{'K', sprites[0]}, {'Q', sprites[1]}, {'B', sprites[2]},
                  {'N', sprites[3]}, {'R', sprites[4]}, {'P', sprites[5]},
                  {'k', sprites[6]}, {'q', sprites[7]}, {'r', sprites[10]},
                  {'b', sprites[8]}, {'n', sprites[9]}, {'p', sprites[11]},
                  {' ', empty}};

    font.loadFromFile((executableDirectory / "../../assets" / "font.otf"));
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
            pieces[row][col].piece.setTexture(emptyTexture);
            pieces[row][col].x = col;
            pieces[row][col].y = row;
            col++;
          }

          continue;
        } else {
          pieces[row][col].piece = TextureMap[c];
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

          float scaleX = (float)fieldSize / PIECE_WIDTH;
          float scaleY = (float)fieldSize / PIECE_HEIGHT;

          // Check the sprite was loaded correctly
          if (pieces[row][col].piece.getTexture() == nullptr) {
            std::cout << "Error loading texture for piece " << c << std::endl;
          }

          pieces[row][col].piece.setScale(scaleX, scaleY);
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

  /*----------------------------- Fast Check for Check on Castle Rules
   * -------------*/
  bool castleCheck(char castleSide, Color color) {
    std::vector<std::tuple<int, int>> tmp = movesList(color);
    switch (castleSide) {
      case 'k':
        return (!isMoveInList(tmp, blackKingY, blackKingX) &&
                !isMoveInList(tmp, blackKingY, blackKingX + 1));
      case 'q':
        return (!isMoveInList(tmp, blackKingY, blackKingX) &&
                !isMoveInList(tmp, blackKingY, blackKingX - 1));
      case 'K':
        return (!isMoveInList(tmp, whiteKingY, whiteKingX) &&
                !isMoveInList(tmp, whiteKingY, whiteKingX - 1));
      case 'Q':
        return (!isMoveInList(tmp, whiteKingY, whiteKingX) &&
                !isMoveInList(tmp, whiteKingY, whiteKingX + 1));
      default:
        return false;
    }
  }

  /* ---------------------------- Move Generation ----------------------------
   */

  /* ---------------------------- Move Validation ----------------------------
   */

  bool allowedMove(Piece& piece, int target_x, int target_y,
                   std::vector<std::tuple<int, int>> moves, Color color) {
    if (isMoveInList(moves, target_y, target_x)) {
			// Not your turn bro!
      if (piece.color == Color::BLACK && color == Color::WHITE ||
          piece.color == Color::WHITE && color == Color::BLACK) {
        return false;
      }

			// Trying to move the piece onto itself
      if (piece.x == target_x && piece.y == target_y) {
        return false;
      }
      // Reset old position to color none
      if (piece.type == PieceType::BLACK_PAWN && piece.y == target_y - 2) {
        pieces[target_y][target_x].enPassant = true;
      } else if (piece.type == PieceType::WHITE_PAWN &&
                 piece.y == target_y + 2) {
        pieces[target_y][target_x].enPassant = true;
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
        if (castleCheck('k',Color::WHITE)==false){
          return false;
        }
        pieces[0][5].color = Color::BLACK;
        pieces[0][5].type = PieceType::BLACK_ROOK;
        pieces[0][7].color = Color::NONE;
        pieces[0][7].type = PieceType::NONE;
      }
      if (piece.type == PieceType::BLACK_ROOK && piece.x == 7) {
        castlek = false;
      }
      if (piece.type == PieceType::BLACK_KING && target_x == piece.x - 2) {  // q
        if (castleCheck('q', Color::WHITE)==false){
          return false;
        }
        pieces[0][3].color = Color::BLACK;
        pieces[0][3].type = PieceType::BLACK_ROOK;
        pieces[0][0].color = Color::NONE;
        pieces[0][0].type = PieceType::NONE;
      }
      if (piece.type == PieceType::BLACK_ROOK && piece.x == 0) {
        castlek = false;
      }
      if (piece.type == PieceType::WHITE_KING && target_x == piece.x + 2) {  // K
        // King will move below
        if (castleCheck('k',Color::BLACK)==false){
          return false;
        }
        pieces[7][5].color = Color::WHITE;
        pieces[7][5].type = PieceType::WHITE_ROOK;
        pieces[7][7].color = Color::NONE;
        pieces[7][7].type = PieceType::NONE;
      }
      if (piece.type == PieceType::WHITE_ROOK && piece.x == 7) {
        castleK = false;
      }
      if (piece.type == PieceType::WHITE_KING && target_x == piece.x - 2) {  // Q
        if (castleCheck('Q', Color::BLACK)==false){
          return false;
        }
        pieces[7][3].color = Color::WHITE;
        pieces[7][3].type = PieceType::WHITE_ROOK;
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


      /* ---------------------------- Move Piece ---------------------------- */
      Color tmpA = pieces[piece.y][piece.x].color;
      PieceType tmpB = pieces[piece.y][piece.x].type;
      Color tmpC = pieces[target_y][target_x].color;
      PieceType tmpD = pieces[target_y][target_x].type;
      pieces[target_y][target_x].color = piece.color;
      pieces[target_y][target_x].type = piece.type;
      pieces[piece.y][piece.x].color = Color::NONE;
      pieces[piece.y][piece.x].type = PieceType::NONE;
      Color otherColor = color == Color::BLACK ? Color::WHITE : Color::BLACK;
      std::vector<std::tuple<int, int>> check = movesList(otherColor);
      if (color == Color::BLACK && isMoveInList(check, blackKingY, blackKingX)) {
        pieces[target_y][target_x].color = tmpC;//Wir updaten die Positionen vorher, falls er im Schach steht muss alles wieder zurück
        pieces[target_y][target_x].type = tmpD;//Vielleicht ist CopyBoard effektiver? Voralldem weil der Bot ja alles prüfen muss
        pieces[piece.y][piece.x].color = tmpA;//dann müsste er nur die Copy nutzen und nicht jedes mal diese tmps erstellen
        pieces[piece.y][piece.x].type = tmpB;    
        pieces[target_y][target_x].enPassant = false;
        return false;
      }
      if (color == Color::WHITE && isMoveInList(check, whiteKingY, whiteKingX)) {
        pieces[target_y][target_x].color = tmpC;
        pieces[target_y][target_x].type = tmpD;
        pieces[piece.y][piece.x].color = tmpA;
        pieces[piece.y][piece.x].type = tmpB; 
        pieces[target_y][target_x].enPassant = false;
        return false;
      }
      pieces[get<0>(lastPosition)][get<1>(lastPosition)].enPassant = false;
      lastPosition = std::make_tuple(target_y, target_x);
      position = reinterpret();

      /* ---------------------------- Print Board --------------------------- */

      //std::cout << position << endl;
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          printf("%ls ", pieces[i][j].print());
        }
        cout << endl;
      }
      cout << "-----------------" << endl;
      return true;
    }
    return false;
  }
  
	std::vector<std::tuple<int, int>> movesList(Color color) {
    std::vector<std::tuple<int, int>> tmp;
    std::vector<std::tuple<int, int>> list;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (pieces[i][j].color == color) {
          tmp = generatePossibleMoves(pieces[i][j], pieces, castlek, castleq,
                                      castleK, castleQ);
          list.insert(list.end(), tmp.begin(), tmp.end());
        }
      }
    }
    return list;
  }

  std::vector<std::tuple<int, int, int, int>> movesListEngine(Color color) {
    std::vector<std::tuple<int, int>> tmp;
    std::vector<std::tuple<int, int, int, int>> list;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (pieces[i][j].color == color) {
          tmp = generatePossibleMoves(pieces[i][j], pieces, castlek, castleq,
                                      castleK, castleQ);
          for (const auto& move : tmp) {
            int move_i = std::get<0>(move);
            int move_j = std::get<1>(move);
            list.push_back(std::make_tuple(move_i, move_j, i, j));
          }
        }
      }
    }
    return list;
  }

  /* ------------------------------- Rendering -------------------------------
   */

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
    return window.getSize().x < window.getSize().y ? window.getSize().x / 8
                                                   : window.getSize().y / 8;
  }

  void run() {
    interpret();
		std::cout << deepEngine(position, toMove, 3);
    sf::Event event;
    bool isMoving = false;
    Piece* movingPiece = nullptr;
    sf::Clock clock;

    std::vector<std::tuple<int, int>> possibleMoves;

    int lastFrameTime = 0;
    int targetFrameTime = 1000 / 144;

    while (window.isOpen()) {
      if (clock.getElapsedTime().asMilliseconds() - lastFrameTime <
          targetFrameTime) {
        continue;
      }

      clock.restart();

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
        if (event.type == sf::Event::MouseButtonPressed) {
          sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
          sf::Vector2i target =
              getCoordinateIndex(mousePosition.x, mousePosition.y);

          if (target.x >= 0 && target.x < 8 && target.y >= 0 && target.y < 8) {
            if (pieces[target.y][target.x].color == toMove) {
              // User wants to move one of his pieces
              isMoving = true;
              movingPiece = &pieces[target.y][target.x];

              possibleMoves = generatePossibleMoves(
                  *movingPiece, pieces, castlek, castleq, castleK, castleQ);
              initializeBoard();
            } else {
              bool moveAllowed =
                  allowedMove(*movingPiece, target.x, target.y, possibleMoves, toMove);

              if (moveAllowed) {
                isMoving = false;
                possibleMoves.clear();
                movingPiece = nullptr;
								reverseTurn();
                initializeBoard();
                interpret();
                engine();
                interpret();
              }
            }
          }
        }
      }

      if (isMoving) {
        for (const auto& move : possibleMoves) {
          // Highlight the move field
          sf::RectangleShape& field = board[get<0>(move)][get<1>(move)];
          field.setFillColor(sf::Color(186, 202, 68));
        }
      }
      drawBoard();
      drawPieces();
      drawFieldLegend();
      window.display();
    }
  }

  void reverseTurn() {
		if (toMove == Color::BLACK) {
			toMove = Color::WHITE;
		} else {
			toMove = Color::BLACK;
		}
	}

  void engine(){
    std::vector<std::tuple<int, int, int, int>> tmp = movesListEngine(Color::BLACK);
    std::vector<std::tuple<int, int>> tmpMove = movesList(Color::BLACK);
    if (tmp.size()==0){
      std::cout << "CHECKMATE";
      return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, tmp.size() - 1);
    int randomIndex = dis(gen);
    std::tuple<int, int, int, int> randomMove = tmp[randomIndex];
    bool fun = allowedMove(pieces[std::get<2>(randomMove)][std::get<3>(randomMove)], std::get<1>(randomMove), std::get<0>(randomMove), tmpMove, toMove);
    while(!fun){
			//reverseTurn();
      int randomIndex = dis(gen);
      std::tuple<int, int, int, int> randomMove = tmp[randomIndex];
      fun = allowedMove(pieces[std::get<2>(randomMove)][std::get<3>(randomMove)], std::get<1>(randomMove), std::get<0>(randomMove), tmpMove, toMove);
    }
		reverseTurn();
  }

	int deepEngine(std::string startPosition, Color color, int depth){
    if(depth == 0){
      return 1;
    }
    int count = 0;
    Color otherColor; 
    interpret();
    if (color == Color::BLACK){
      otherColor=Color::WHITE;
    }
    else{
      otherColor=Color::BLACK;
    }
    std::vector<std::tuple<int, int, int, int>> tmpList = movesListEngine(otherColor);
    std::vector<std::tuple<int, int>> tmpMove = movesList(otherColor);
    for (const auto& tuple : tmpList){
      if (allowedMove(pieces[std::get<2>(tuple)][std::get<3>(tuple)], std::get<1>(tuple), std::get<0>(tuple), tmpMove, otherColor)){
        count += deepEngine(position, otherColor, depth-1);
      }
      else{
        std::cout << "Move failed, moving to: " << std::get<0>(tuple) << ", " << std::get<1>(tuple) << std::endl;
        std::cout << "Tried moving from: " << std::get<2>(tuple) << ", " << std::get<3>(tuple) << std::endl;
        printf("Piece is: %ls\n", pieces[std::get<2>(tuple)][std::get<3>(tuple)].print());
        printMovesFull(tmpList);
				// printMovesAsBoard(tmpList);
        exit(0);
      }
      position = startPosition;
      interpret();
    }
    return count;
  }
};

int main() {
  setlocale(LC_ALL, "");
  Game main;
  main.run();
  return 0;
}