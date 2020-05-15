#include "Piece.h"
#include "Bishop.h"
#include <cmath>

//A bishop may move diagonally only, but may move any number of spaces.
//A bishop may not move through other pieces.
bool Bishop::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
  //cm=column_moved, rm=row_moved                                                      
  int cm = abs(start.first - end.first);
  int rm = abs(start.second - end.second);
  if(cm == rm && !(cm == 0 && rm == 0))
    return true; //move diagnoly
  return false;
}
