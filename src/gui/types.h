#include <SFML/Graphics.hpp>

#ifndef TYPES_H
#define TYPES_H

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

struct Piece {
  sf::Sprite piece;
  PieceType type;
  int x;
  int y;
  Color color;
  bool enPassant;

  const wchar_t* print() {
    std::map<PieceType, const wchar_t*> map = {
        {PieceType::WHITE_KING, L"♔"},   {PieceType::WHITE_ROOK, L"♖"},
        {PieceType::WHITE_BISHOP, L"♗"}, {PieceType::WHITE_PAWN, L"♙"},
        {PieceType::WHITE_QUEEN, L"♕"},  {PieceType::WHITE_KNIGHT, L"♘"},
        {PieceType::BLACK_KING, L"♚"},   {PieceType::BLACK_ROOK, L"♜"},
        {PieceType::BLACK_BISHOP, L"♝"}, {PieceType::BLACK_PAWN, L"♟"},
        {PieceType::BLACK_QUEEN, L"♛"},  {PieceType::BLACK_KNIGHT, L"♞"},
        {PieceType::NONE, L" "}};

    return map[type];
  }
};


// Needs to be inline, otherwise the linker complains...
inline std::map<char, PieceType> PieceMap = {
    {'K', PieceType::WHITE_KING},   {'Q', PieceType::WHITE_QUEEN},
    {'R', PieceType::WHITE_ROOK},   {'B', PieceType::WHITE_BISHOP},
    {'N', PieceType::WHITE_KNIGHT}, {'P', PieceType::WHITE_PAWN},
    {'k', PieceType::BLACK_KING},   {'q', PieceType::BLACK_QUEEN},
    {'r', PieceType::BLACK_ROOK},   {'b', PieceType::BLACK_BISHOP},
    {'n', PieceType::BLACK_KNIGHT}, {'p', PieceType::BLACK_PAWN},
    {' ', PieceType::NONE}};

inline std::map<PieceType, char> ReversePieceMap = {
    {PieceType::WHITE_KING, 'K'},   {PieceType::WHITE_QUEEN, 'Q'},
    {PieceType::WHITE_ROOK, 'R'},   {PieceType::WHITE_BISHOP, 'B'},
    {PieceType::WHITE_KNIGHT, 'N'}, {PieceType::WHITE_PAWN, 'P'},
    {PieceType::BLACK_KING, 'k'},   {PieceType::BLACK_QUEEN, 'q'},
    {PieceType::BLACK_ROOK, 'r'},   {PieceType::BLACK_BISHOP, 'b'},
    {PieceType::BLACK_KNIGHT, 'n'}, {PieceType::BLACK_PAWN, 'p'},
    {PieceType::NONE, ' '}};

const int PIECE_HEIGHT = 250;
const int PIECE_WIDTH = 250;

struct MoveStruct {
  int x;
  int y;
};

struct FullMoveStruct {
  MoveStruct from;
  MoveStruct to;
};

typedef std::tuple<int, int> Move;
typedef std::tuple<int, int, int, int> FullMove;

typedef std::vector<Move> MoveList;
typedef std::vector<FullMove> FullMoveList;

#endif