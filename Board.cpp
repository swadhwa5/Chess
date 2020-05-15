#include <iostream>
#include <utility>
#include <map>
#include "Board.h"
#include "CreatePiece.h"
#include "Terminal.h"
#include <vector>

using std::make_pair;
using std::vector;
using std::map;
using std::pair;
using std::cout;
using std::endl;

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Board::Board(){}

//destructor
Board::~Board() {
  for(map<pair<char,char>,Piece*>::iterator it = occ.begin();
      it != occ.end();it++){
    if(it->second != NULL) {
      delete it->second;
      it->second = NULL;
    }
  }
  occ.clear();
}

map<pair<char,char>,Piece*> Board::get_map() const{
  return occ;//return the map
}

void Board::move_piece(pair<char, char> from, pair<char, char> to, bool capture){
  if(capture){
    delete occ[to];//delete the captured piece
    occ[to] = NULL;
  }
  Piece * movep = occ[from];
  occ[to] = movep;//move piece to new pos
  occ.erase(from);
}


const Piece* Board::operator()(pair<char, char> position) const {
  map<pair<char,char>, Piece*>::const_iterator it = occ.find(position);
  if(it != occ.end()) {
    return it->second;//return pointer to piece 
  }
  return NULL;//no piece exist
}

bool Board::valid_pos(pair<char, char> pos) const{
  char col = pos.first;
  char row = pos.second;
  if(((col >= 'A' && col <='H') || (col >= 'a' && col <='h')) && (row >= '1' && row <= '8')) {
    return true;//valid pos
  }
  return false;//pos out of bound
}


bool Board::add_piece(pair<char, char> position, char piece_designator) {
  char C = piece_designator;
  if(!(C == 'K' || C == 'k' || C == 'Q' || C == 'q' || C == 'B' || C == 'b' || C == 'N' || C == 'n'|| C == 'R' || C == 'r' || C == 'P' || C == 'p' ||  C == 'M' ||  C == 'm')) 
    return false;//not a valid piece type
  if(!valid_pos(position))
    return false;//not a valid pos
  for(map<pair<char, char>, Piece*>::const_iterator it = occ.cbegin();
      it != occ.cend();
      it++) {
    if(it->first == position) 
      return false;//pos occupied
  }
  occ[position] = create_piece(piece_designator);
  return true;//passed all fail conditions
}

bool Board::has_valid_kings() const {
  int white_kings = 0;//count of white kings, should be 1
  int black_kings = 0;//count of black kings, should be 1
  for(std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.cbegin();
      it != occ.cend();
      it++) {//iterating over the map
    if(((it->second)->is_white()) && ((it->second)->to_ascii()== 'K')) {//checking for white king
      white_kings++;
    }
    if(!((it->second)->is_white()) && ((it->second)->to_ascii()== 'k')) {//checking for black king
      black_kings++;
    }
  }

  if(black_kings == 1 && white_kings == 1) {//if board has one white and one black king, then it has valid kings, else false
  return true;
  }
  return false;
}

void Board::display() const {
  //print title
  Terminal::color_all(true, Terminal::RED, Terminal::YELLOW);
  cout << "                                             ";
  Terminal::set_default();
  cout << endl;
  Terminal::color_all(true, Terminal::RED, Terminal::YELLOW);
  cout << "    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    ";
  Terminal::set_default();
  cout << endl;
  Terminal::color_all(true, Terminal::RED, Terminal::YELLOW);
  cout << "    |* S O S * M A S T E R * C H E S S *|    ";
  Terminal::set_default();
  cout << endl;
  Terminal::color_all(true, Terminal::RED, Terminal::YELLOW);
  cout << "    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    ";
  Terminal::set_default();
  cout << endl;
  Terminal::color_all(true, Terminal::RED, Terminal::YELLOW);
  cout << "                                             ";
  Terminal::set_default();
  cout << endl;

  //print board
  int x = 0;
  for(char r = '8'; r >= '1'; r--) {
    Terminal::color_bg(Terminal::YELLOW);
    cout << "          ";
    int y = 0;
    if(x % 2 == 0)//start with a different color for a new line
      y = 1;
    for(char c = 'A'; c <= 'H'; c++) {
      if(y % 2 == 0)//switch background color for adjacent grid
	Terminal::color_bg(Terminal::BLUE);
      else
	Terminal::color_bg(Terminal::CYAN);
      const Piece* piece = (*this)(pair<char, char>(c, r));
      if (piece && piece->is_white()) {
	Terminal::color_fg(true, Terminal::WHITE);//player A has white piece
        cout << " " << piece->to_ascii() << " ";
      }
      else if(piece && !(piece->is_white())){
	Terminal::color_fg(true, Terminal::BLACK);//player B has black piece
        cout << " " << piece->to_ascii() << " ";
      }
      else {
        cout << "   ";//empty grid
      }
      y++;
    }
    Terminal::color_bg(Terminal::YELLOW);
    cout << "           ";
    Terminal::set_default();
    cout << endl;
    x++;
  }

  //print quote
  Terminal::color_all(true, Terminal::MAGENTA, Terminal::YELLOW);
  cout << "                                             ";
  Terminal::set_default();
  cout << endl;
  Terminal::color_all(true, Terminal::MAGENTA, Terminal::YELLOW);
  cout << "  Be the chess player, not the chess piece.  ";
  Terminal::set_default();
  cout << endl;
  Terminal::color_all(true, Terminal::MAGENTA, Terminal::YELLOW);
  cout << "                            –-Ralph Charell  ";
  Terminal::set_default();
  cout << endl;
  Terminal::color_all(true, Terminal::MAGENTA, Terminal::YELLOW);
  cout << "                                             ";
  Terminal::set_default();
  cout << endl;
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Board& board) {
  for(char r = '8'; r >= '1'; r--) {
    for(char c = 'A'; c <= 'H'; c++) {
      const Piece* piece = board(pair<char, char>(c, r));
      if (piece) {
	os << piece->to_ascii();
      } else {
	os << '-';
      }
    }
    os << std::endl;
  }
  return os;
}

bool Board::hori_blocked(pair<char, char> start, pair<char, char> end) const{
  char sc = start.first;//start column
  char ec = end.first;//end column
  int direc;//negative if move toward left, positive if move toward right
  if((ec - sc) > 0)
    direc = 1;//right
  else
    direc = -1;//left
  pair<char, char> cur((sc + direc), start.second);
  //loop through path with direction
  for(; cur.first != ec; cur.first+=direc){
    if((*this)(cur) != NULL)//!!!!syntax???
      return true;//there is piece present on path
  }
  return false;//no piece detected
}


bool Board::vert_blocked(pair<char, char> start, pair<char, char> end) const{
  char sr = start.second;//start row                                                           
  char er = end.second;//end row                                                               
  int direc;//negative if move down, positive if move up
  if((er - sr) > 0)
    direc = 1;//up
  else
    direc = -1;//down
  pair<char, char> cur(start.first, (sr + direc));
  for(; cur.second != er; cur.second+=direc){
    if((*this)(cur) != NULL)
      return true;//there is piece present on path                                                
  }
  return false;//no piece detected
}


bool Board::diag_blocked(pair<char, char> start, pair<char, char> end) const{
  int r_change = end.second - start.second;
  int c_change = end.first - start.first;
  int rdirec;
  int cdirec;
  if(r_change > 0)
    rdirec = 1;//moving up
  else
    rdirec = -1;//moving down
  if(c_change > 0)
    cdirec = 1;//moving right
  else
    cdirec = -1;//moving left
  pair<char, char> cur(start.first+cdirec, start.second+rdirec);
  //loop through path
  for(; cur.first != end.first; cur.first+=cdirec, cur.second+=rdirec){
    if((*this)(cur) != NULL)
      return true;//a piece exist on path
  }
  return false;//no piece detected
}

bool Board::path_blocked(std::pair<char, char> start, std::pair<char, char> end) const{
  int r_change = end.second - start.second;
  int c_change = end.first - start.first;
  if(r_change == 0)//horizontal movement
    return hori_blocked(start, end);
  else if(c_change == 0)//vertical movement
    return vert_blocked(start, end);
  else if(abs(r_change) == abs(c_change))//diagnol movement
    return diag_blocked(start, end);
  return false;//weird path pattern which allows move through pieces. e.g. L-shape
}

bool Board::move_and_capture(const Piece* targ, bool cur_is_w, bool targ_is_w, std::pair<char, char> start, std::pair<char, char> end){
  if(targ == NULL){
    move_piece(start, end, false);//no capture but move piece                     
    return true;//is a legal move and no piece present at end pos                                 
  }
  else if(cur_is_w != targ_is_w && targ != NULL){
    move_piece(start, end, true);//opponent's piece RIP, capture and move     
    return true;//is a legal move and opponent's piece present at end pos                         
  }
  return false;//curp cannot capture teammates   
}

bool Board::has_reached_end(bool white, pair<char, char> end) {
  if(white) {
    if((end.second == '8') && ((end.first>='A' && end.first <= 'H') ||(end.first >='a' && end.first<='h'))) {//checking if white player has reached the end on its opposite side
	return true;
    }
  }
  if(!white) {
    if((end.second == '1') && ((end.first>='A' && end.first <= 'H') ||(end.first >='a' && end.first<='h'))) {//checking if black player has reached end on its opposite side
      return true;
    }   
  }     
  return false;//if none has reached end, the return false
}
    
void Board::erase_piece(pair<char, char> pos){//used to erase a piece from the map so that a new piece can be added at that position in piece promotion
  occ[pos] = NULL;//done to fulfil condition in add_piece that the specifies position is not occupied
  occ.erase(pos);//done to erase key from the map 
}
