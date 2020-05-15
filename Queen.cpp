#include "Piece.h"
#include "Queen.h"
#include <cmath>

//A queen may move any number of spaces in one direction, including diagonally.
//A queen may not move through other pieces.
bool Queen::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
  //cm=column_moved, rm=row_moved
  int cm = abs(start.first - end.first);
  int rm = abs(start.second - end.second);
  if((cm == 0 && rm != 0) || (cm != 0 && rm == 0))
    return true; //move by row or column
  else if(cm == rm && cm != 0 && rm != 0)                                                                                                               
    return true; //move diagnally 

  /* else if(cm == rm && !(cm == 0 && rm == 0))
    return true; //move diagnally
  */
  return false;
}
