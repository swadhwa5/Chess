#include "Chess.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cctype>

using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Chess::Chess() : is_white_turn(true) {
  // Add the pawns
  for (int i = 0; i < 8; i++) {
    board.add_piece(std::pair<char, char>('A' + i, '1' + 1), 'P');
    board.add_piece(std::pair<char, char>('A' + i, '1' + 6), 'p');
  }
  
  // Add the rooks
  board.add_piece(std::pair<char, char>( 'A'+0 , '1'+0 ) , 'R' );
  board.add_piece(std::pair<char, char>( 'A'+7 , '1'+0 ) , 'R' );
  board.add_piece(std::pair<char, char>( 'A'+0 , '1'+7 ) , 'r' );
  board.add_piece(std::pair<char, char>( 'A'+7 , '1'+7 ) , 'r' );
  
  // Add the knights
  board.add_piece(std::pair<char, char>( 'A'+1 , '1'+0 ) , 'N' );
  board.add_piece(std::pair<char, char>( 'A'+6 , '1'+0 ) , 'N' );
  board.add_piece(std::pair<char, char>( 'A'+1 , '1'+7 ) , 'n' );
  board.add_piece(std::pair<char, char>( 'A'+6 , '1'+7 ) , 'n' );
  
  // Add the bishops
  board.add_piece(std::pair<char, char>( 'A'+2 , '1'+0 ) , 'B' );
  board.add_piece(std::pair<char, char>( 'A'+5 , '1'+0 ) , 'B' );
  board.add_piece(std::pair<char, char>( 'A'+2 , '1'+7 ) , 'b' );
  board.add_piece(std::pair<char, char>( 'A'+5 , '1'+7 ) , 'b' );
  
  // Add the kings and queens
  board.add_piece(std::pair<char, char>( 'A'+3 , '1'+0 ) , 'Q' );
  board.add_piece(std::pair<char, char>( 'A'+4 , '1'+0 ) , 'K' );
  board.add_piece(std::pair<char, char>( 'A'+3 , '1'+7 ) , 'q' );
  board.add_piece(std::pair<char, char>( 'A'+4 , '1'+7 ) , 'k' );
}

Chess::Chess(const Chess& chess){
  map<pair<char,char>, Piece*> chess_board_map_copy = chess.board.get_map();
  for(map<pair<char, char>, Piece*>::const_iterator it = chess_board_map_copy.cbegin();
      it != chess_board_map_copy.cend();
      it++) { //loop through map and create a copy of it
    board.add_piece(it->first, it->second->to_ascii());
  }
  is_white_turn = chess.is_white_turn;//setting turn of player
}

//load a new game from input file by changing current board status
std::istream& Chess::new_game(std::istream& in){
  board.~Board();
  char piece;//char variable that records the piece
  int count=0;//count of pieces
  int r=0;//record rows
  int c=0;//record columns
  while(in>>piece&&count<64){
    c=count%8;
    r=count/8;
    if(piece!='-')
      board.add_piece(pair<char,char>('A'+c, '8'-r), piece);
    count++;
  }
  //takes in the last char in file and checks if it is white turn
  if(piece == 'w')
    is_white_turn=true;
  else
    is_white_turn=false;
  return in;
}

bool Chess::make_move(std::pair<char, char> start, std::pair<char, char> end) {

  if(!board.valid_pos(start) || !board.valid_pos(end)){
    return false;//both start and end must be valid pos
  }

  //changing all pos to uppercase
  start.first = toupper(start.first);
  start.second = toupper(start.second);
  end.first = toupper(end.first);
  end.second = toupper(end.second);
  
  const Piece* curp = board(start);//create a pointer that points to the piece the user try to move
  if(curp == NULL){
    return false; //invalid start pos, no piece exist
  }
  
  char cur_type = curp->to_ascii();//record piece type
  bool cur_is_w = curp->is_white();//record piece color

  if(is_white_turn && !cur_is_w)
    return false;//it's white's turn but player trying to move a black piece
  else if(!is_white_turn && cur_is_w)
    return false;//the otherway around
  
  const Piece* targ = board(end);//creates a pointer that points to the piece at end pos

  bool targ_is_w;
  if(targ != NULL){
    targ_is_w = targ->is_white();
  }

  if(targ == NULL){
    targ_is_w = false;//although no targ piece still assign default color but won't affect capture decision
  }
  //make sure the movement does not cause player to be in check through a copy board
  Chess copy = *this;
  if(cur_type == 'P' || cur_type == 'p'){
    if(curp->legal_move_shape(start,end) && !copy.board.path_blocked(start, end) && targ == NULL){//is a legal normal move(straight line) and the path is clear 
      copy.board.move_piece(start, end, false);
    }
    else if(curp->legal_capture_shape(start,end) && targ != NULL){//is a legal capture move(diagnol)and a target is present                                                                             
      copy.board.move_and_capture(targ, cur_is_w, targ_is_w, start, end);
    }
    if(copy.in_check(copy.is_white_turn)){//after the pawn move player is in check
      return false;//movement causes player to be in check 
    }
  }
  else if(copy.board.move_and_capture(targ, cur_is_w, targ_is_w, start, end) && copy.in_check(copy.is_white_turn)){//able to move piece and after moving the player is in check
    return false;//movement causes player to be in check
  }

  switch (cur_type){
    //king and knight does not need to check path
  case 'K':
  case 'k':
  case 'N':
  case 'n':
    if(!curp->legal_move_shape(start,end)){
      return false;//illegal move
    }
    else{
      if(board.move_and_capture(targ, cur_is_w, targ_is_w, start, end)){
	is_white_turn = !is_white_turn;
	return true;//switch turn upon successful move
      }
      else
	return false;
    }
    //queen, bishop, rook need to check path
  case 'P':
  case 'p':
    if(curp->legal_move_shape(start,end) && (!board.path_blocked(start, end)) && targ == NULL){//is a legal normal move(straight line) and the path is clear
      board.move_piece(start, end, false);
      if(board.has_reached_end(cur_is_w,end)) {
	board.erase_piece(end);
	if(cur_is_w) {//promote white pawn to queen
	  delete curp;
	  board.add_piece(end,'Q');
	}
	else {//promote black pawn to queen
	  delete curp;
          board.add_piece(end,'q');
	}
      }
      is_white_turn = !is_white_turn;
      return true;//switch turn upon successful move 
    }
    else if(curp->legal_capture_shape(start,end) && targ != NULL){//is a legal capture move(diagnol) and a target is present
      board.move_and_capture(targ, cur_is_w, targ_is_w, start, end);
      if(board.has_reached_end(curp->is_white(),end)) {
	board.erase_piece(end);
	if(curp->is_white()) {//promote white pawn to queen
	  delete curp;
          board.add_piece(end,'Q');
	}
        else{//promote black pawn to queen
	  delete curp;
          board.add_piece(end,'q');
	}
      }
      is_white_turn = !is_white_turn;
      return true;//switch turn upon successful move 
    }
    return false;//not a normal nor a capture move
  case 'Q':
  case 'q':
  case 'B':
  case 'b':
  case 'R':
  case 'r':
  default://reserved for mystery piece
    if(!curp->legal_move_shape(start,end)){
      return false;//illegal move
    }
    else if(!board.path_blocked(start, end)){//no piece blocks path
      if(board.move_and_capture(targ, cur_is_w, targ_is_w, start, end)){//only returns false if there is a team piece at the end loc
	is_white_turn = !is_white_turn;
        return true;//switch turn upon successful move
      }
    }
    return false;//piece present on path, cannot move though
  }
  return false; //should never reach here
}


bool Chess::in_check(bool white) const{
  pair<char,char> king;// store the location of king
  bool success_capture=false;
  //iterate the map to find and store the white king
  map<pair<char, char>, Piece*> mapCopy = board.get_map();
  for(map<pair<char, char>, Piece*>::iterator it = mapCopy.begin();
      it != mapCopy.end();
      it++) {
    if(it->second != NULL){//no need to check if there's no piece present
      if( (it->second->to_ascii()=='K' && white)
	  || (it->second->to_ascii()=='k' && !white) ){
	king=it->first;//store the position of king
      }
    }
  }
    
  //now iterate and test if pieces from black side could capture the king
  for(map<pair<char, char>, Piece*>::iterator it = mapCopy.begin();
      it != mapCopy.end();
      it++) {
    if(it->second != NULL && it->second->is_white() != white){//it's a opposite color piece
      if((it->second)->legal_capture_shape(it->first,king) && !(board.path_blocked((it->first),king))){//can legal capture king and path is clear
	success_capture=true;
      }
    }
  }
  return success_capture;
}


bool Chess::in_mate(bool white) const {
  if(!in_check(white)){
    return false; //the player is not in check so in_mate need not to be considered
  }
  return !available(white);//returns true if in_check no available moves possible for this player
}


bool Chess::in_stalemate(bool white) const {
  if(in_check(white)) {//checks if the player is in check, then return false
    return false;
  }
  return !available(white);//returns true if there are available positions on the board where a piece can move or capture legally, and making that move doesnt put the player in check
}
 

bool Chess::available(bool white) const {//helper function that returns true if there are available positions on the board where a piece can move or capture legally, and making that move doesnt put the player in check
  map<pair<char, char>, Piece*> mapCopy = board.get_map();
  for(map<pair<char, char>, Piece*>::iterator it = mapCopy.begin();
      it != mapCopy.end();
      it++) {
    if(it->second != NULL){
      if((it->second)->is_white() == white) {
	for(char i = 'A'; i<= 'H' ; i++) {//a loop to go over every single box in the board, and enter that as the end position in all funcs             
	  for(char j = '1';j <= '8';j++){
	    pair<char,char> board_pos= make_pair(i,j);
	    //make sure the movement does not cause player to be in check through a copy board   
	    Chess copy = *this;
	    if(copy.make_move(it->first,board_pos)){//able to move piece and after moving the player is not in check
	      return true;//if legal move or legal capture move, and path not blocked and making the move doesnt put the player in_check,then not stalemate as moves are available                  
	    }
	  }
	}
      }
    }
  }
  return false;//no moves available whatsoever
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
std::ostream& operator<< (std::ostream& os, const Chess& chess) {
  // Write the board out and then either the character 'w' or the character 'b',
  // depending on whose turn it is
  return os << chess.get_board() << (chess.turn_white() ? 'w' : 'b');
}


std::istream& operator>> (std::istream& is, Chess& chess) {
  return chess.new_game(is);//call to helper function
}

