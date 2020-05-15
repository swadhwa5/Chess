#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"
#include <vector>

class Board {

  // Throughout, we will be accessing board positions using an std::pair<char, char>.
  // The assumption is that the first value is the column with values in
  // {'A','B','C','D','E','F','G','H'} (all caps)
  // and the second is the row, with values in {'1','2','3','4','5','6','7','8'}

public:
  // Default constructor
  Board();

  
  //destructor
  ~Board();
  
  // Returns a const pointer to the piece at a prescribed location if it exists,
  // or nullptr if there is nothing there.
  const Piece* operator() (std::pair<char, char> position) const;

  //return the map
  std::map<std::pair<char,char>,Piece*> get_map() const;
 

  // Attempts to add a new piece with the specified designator, at the given location.
  // Returns false if:
  // -- the designator is invalid,
  // -- the specified location is not on the board, or
  // -- if the specified location is occupied
  bool add_piece(std::pair<char, char> position, char piece_designator);
  
  // Displays the board by printing it to stdout
  void display() const;
  
  // Returns true if the board has the right number of kings on it
  bool has_valid_kings() const;

  //modify occ to move given piece, erase captured piece from board if indicated by bool value
  void move_piece(std::pair<char, char> from, std::pair<char, char> to, bool capture);

  //check if a given pos is valid
  bool valid_pos(std::pair<char, char> pos) const;

  //return true if there's a piece present on the horizonal path except start & end 
  bool hori_blocked(std::pair<char, char> start, std::pair<char, char> end) const;

  ////return true if there's a piece present on the vertical path except start & end
  bool vert_blocked(std::pair<char, char> start, std::pair<char, char> end) const;
  
  //return true if there's a piece present on the diagnol path except start & end
  bool diag_blocked(std::pair<char, char> start, std::pair<char, char> end) const;

  //detect path pattern and call block functions, return true if path is blocked 
  bool path_blocked(std::pair<char, char> start, std::pair<char, char> end) const;

  //return true and move curp to targ pos if no piece at target pos or target pos has an opponent's piece, false otherwise
  bool move_and_capture(const Piece* targ, bool cur_is_w, bool targ_is_w, std::pair<char, char> start, std::pair<char, char> end);

  //returns true if piece has reached the end of the board and is eligible for promotion
  bool has_reached_end(bool white, std::pair<char, char> end);

  //erase a piece at a given location
  void erase_piece(std::pair<char, char> pos);
  
private:
  // The sparse map storing the pieces, keyed off locations
  std::map<std::pair<char, char>, Piece*> occ;
};

// Write the board state to an output stream
std::ostream& operator<< (std::ostream& os, const Board& board);

#endif // BOARD_H
