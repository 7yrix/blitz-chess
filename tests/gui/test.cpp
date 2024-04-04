#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../../src/gui/MoveGeneration.h"
#include "../../src/gui/utils/fen.cpp"

TEST_CASE("testing the factorial function") {
	std::array<std::array<Piece, 8>, 8> pieces = recreateBoardFromFENString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	CHECK(pieces[0][0].type == PieceType::BLACK_ROOK);
	CHECK(pieces[0][0].color == Color::BLACK);
}
