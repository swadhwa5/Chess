CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

chess: main.cpp Chess.o Board.o Bishop.o King.o Knight.o Pawn.o Queen.o Rook.o Mystery.h Terminal.h CreatePiece.o
	g++ -o  chess main.cpp Chess.o Board.o Bishop.o King.o Knight.o Pawn.o Queen.o Rook.o CreatePiece.o Mystery.h

Bishop.o: Piece.h Bishop.cpp Bishop.h
	g++ -c Bishop.cpp $(CFLAGS)

King.o: Piece.h King.cpp King.h
	g++ -c King.cpp $(CFLAGS)

Knight.o: Piece.h Knight.cpp Knight.h
	g++ -c Knight.cpp $(CFLAGS)

Pawn.o: Piece.h Pawn.cpp Pawn.h
	g++ -c Pawn.cpp $(CFLAGS)

Queen.o: Piece.h Queen.cpp Queen.h
	g++ -c Queen.cpp $(CFLAGS)

Rook.o: Piece.h Rook.cpp Rook.h
	g++ -c Rook.cpp $(CFLAGS)

Board.o: Piece.h Rook.h King.h Knight.h Pawn.h Queen.h Bishop.h Mystery.h Board.h Board.cpp
	g++ -c Board.cpp $(CFLAGS)

CreatePiece.o: CreatePiece.cpp CreatePiece.h Piece.h Rook.h King.h Knight.h Pawn.h Queen.h Bishop.h Mystery.h
	g++ -c CreatePiece.cpp $(CFLAGS)

Chess.o: Chess.cpp Chess.h
	g++ -c Chess.cpp $(CFLAGS)

clean:
	rm -f *.o chess
