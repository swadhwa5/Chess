#include "Piece.h"
#include "Pawn.h"
#include <cmath>

//A pawn can only move forward towards the opponent’s side of the board.
//white player starts at bottom, black player starts at top
//white pawn birth row: 2, black pawn birth row: 7
//the pawn may not use a forward move to land in a square that is occupied by any player’s piece.

bool Pawn::legal_move_shape(std::pair<char, char> start,std::pair<char, char> end) const {
  
  bool is_w = this->is_white();
  //cm=column_moved, rm=row_moved 
  //white pawn's move must be upward(positive), black pawn's move must be downward(negative)
  int cm = end.first - start.first;
  int rm = end.second - start.second;
 
  if(cm != 0) {
    return false;
  }
  
  if(is_w && start.second == '2') {
    if(rm == 1 || rm == 2) {
      return true;//case where white pawn is in birth row and makes normal move
    }
    return false;
  }
  
  if(!is_w && start.second == '7') {
    if(rm == -1 || rm == -2) {
      return true;//case where black pawn is in birth row and make normal move
      }
    return false;
  }

  if(is_w && start.second !='2') {
    if(rm == 1) {//white pawn not in birth row and makes normal move, i.e. not capture so only forward one step
      return true;
    }
    return false;
  }

  if(!is_w && start.second != '7') {
     if(rm == -1) {//black pawn not in birth row and makes normal move, i.e. not capture so only forward one step
       return true;
    }
     return false;
  }
  return false;
}
      

bool Pawn::legal_capture_shape(std::pair<char, char> start,std::pair<char, char> end) const {
  
  bool is_w = this->is_white();
  //cm=column_moved, rm=row_moved
  int cm = end.first - start.first;
  int rm = end.second - start.second;

  if(is_w) {
    if((rm == 1) && (abs(cm) == 1)) {
      return true;//white pawn capture: move up diagnoly by 1 space only
    }
    return false;
  }
  else if((rm == -1) && (abs(cm) == 1)) {
    return true;//black pawn capture: move down diagonly by 1 space only
  }
  return false;
}

