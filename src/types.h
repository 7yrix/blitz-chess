#include <SFML/Graphics.hpp>

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

enum Color
{
		WHITE,
		BLACK,
		NO_COLOR
};

struct Piece{
    sf::Sprite piece;
    PieceType type;
		Color color;
		int x;
		int y;
};