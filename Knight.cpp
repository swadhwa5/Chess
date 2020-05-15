#include "Piece.h"
#include "Knight.h"
#include <cmath>

//A knight may move in an L-shape, of length either two-by-one or one-by-two.
//Does not stop by other piece.
bool Knight::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
  //cm=column_moved, rm=row_moved                                                      
  int cm = abs(start.first - end.first);
  int rm = abs(start.second - end.second);
  if((cm == 2 && rm == 1) || (cm == 1 && rm == 2))
    return true;//move by L-shape
  return false;
}
