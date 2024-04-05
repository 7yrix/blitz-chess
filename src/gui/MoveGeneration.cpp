#include "types.h"
#include <vector>

MoveList generatePossibleMoves(Piece& piece, Piece pieces[8][8], bool castlek, bool castleq, bool castleK, bool castleQ) {
    MoveList moves;

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
            (piece.y - 1) > 0 &&//??? I mean was soll das tun ???
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
