#include "Piece.h"
#include "Rook.h"
#include <cmath>

//A rook may move any number of squares, horizontically or vertically only.
//may not move through other piece.
bool Rook::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
  //cm=column_moved, rm=row_moved                                     
  int cm = abs(start.first - end.first);
  int rm = abs(start.second - end.second);
  if((cm == 0 && rm != 0) || (cm != 0 && rm == 0))
    return true; //move by row or column
  return false;
}
