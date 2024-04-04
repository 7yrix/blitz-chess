#include "types.h"
#include <vector>

std::vector<std::tuple<int, int>> generatePossibleMoves(
    Piece& piece, Piece pieces[8][8], bool castlek, bool castleq, bool castleK,
    bool castleQ);