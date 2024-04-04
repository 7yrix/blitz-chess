#include "../types.h"

std::array<std::array<Piece, 8>, 8> recreateBoardFromFENString(std::string fen) {
  std::array<std::array<Piece, 8>, 8> pieces;

  int row = 0;  // Starten Sie bei der letzten Reihe (Zeile 8)
  int col = 0;  // Starten Sie in der ersten Spalte (A)

  for (char c : fen) {
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
          pieces[row][col].x = col;
          pieces[row][col].y = row;
          col++;
        }

        continue;
      } else {
        pieces[row][col].type = PieceMap[c];
        pieces[row][col].x = col;
        pieces[row][col].y = row;

        if (isupper(c)) {
          pieces[row][col].color = Color::WHITE;
        } else {
          pieces[row][col].color = Color::BLACK;
        }
        col++;
      }
    }
  }

	return pieces;
}