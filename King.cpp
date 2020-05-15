#include "Piece.h"
#include "King.h"
#include <cmath>

//where does the program checks if user input is out of game board??
//A king may move in any direction including diagonally, but may only move one square.
bool King::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
  //cm=column_moved, rm=row_moved                                                                                                                                                                          
  int cm = abs(start.first - end.first);
  int rm = abs(start.second - end.second);
  if((cm == 0 && rm == 1) || ( cm == 1 && rm == 0) || (cm == 1 && rm == 1)) 
    return true;//move in any direction by 1 only
  return false;
}
  

