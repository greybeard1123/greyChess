class Board
{
	unsigned int nextMove;        // WHITE_MOVE or BLACK_MOVE
	unsigned int castleWhite;     // White's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
	unsigned int castleBlack;     // Black's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
	int epSquare;                  // En-passant target square after double pawn move
	int fiftyMove;                 // Moves since the last pawn move or capture

	int Material;				   // total material on board in centipawns, from white’s side of view
	bool viewRotated;			   // used for displaying the board. TRUE or FALSE.

public:

	BitMap whiteKing, whiteQueens, whiteRooks, whiteBishops, whiteKnights, whitePawns;
	BitMap blackKing, blackQueens, blackRooks, blackBishops, blackKnights, blackPawns;
	BitMap whitePieces, blackPieces, occupiedSquares;

	int square[64];				   // tells what kind of piece is on a particular square.

	// storing moves:
	Move moveBuffer[MAX_MOV_BUFF]; // all generated moves of the current search tree are stored in this array.
	unsigned int moveBufLen[MAX_PLY];       // this arrays keeps track of which moves belong to which ply

	// search variables:
	int triangularLength[MAX_PLY];
	Move triangularArray[MAX_PLY][MAX_PLY];
	Timer timer;
	U64 msStart, msStop;
	int searchDepth;
	U64 inodes;

	int endOfGame;				   // index for board.move_tree_object
	int endOfSearch;			   // index for board.move_tree_object
	move_tree move_tree_object[MAX_MOVES_DEPTH];

	void init();
	Move think();
	int minimax(int ply, int depth);
	int alphabeta(int ply, int depth, int alpha, int beta);
	int alphabetapvs(int ply, int depth, int alpha, int beta);
	void displaySearchStats(int mode, int depth, int score);
	bool isEndOfgame(int &legalmoves, Move &singlemove);
	bool is_viewRotated(){ return viewRotated; }
	unsigned int get_nextMove(){ return nextMove; }
	int get_Material(){ return Material; }
	int get_epSquare(){ return epSquare; }
	int get_fiftyMove(){ return fiftyMove; }
	unsigned int get_castleWhite(){ return castleWhite; }
	unsigned int get_castleBlack(){ return castleBlack; }

	void set_nextMove(unsigned int mov){ nextMove = mov; }
	void set_viewRotated(bool view){ viewRotated = view; }
	void set_castleBlack(unsigned int cb){ castleBlack = cb; }
	void set_castleWhite(unsigned int cw){ castleWhite = cw; }
	void set_fiftyMove(int fm){ fiftyMove = fm; }
	void set_epSquare(int ep){ epSquare = ep; }
	void set_Material(int m){ Material = m; }

	void initFromSquares(int input[64], unsigned char next, int fiftyM, int castleW, int castleB, int epSq);
	void display();

	int eval();
	void mirror();
}board;

 void Board::init()
{
       viewRotated = false;
 
       for (int i = 0; i < 64; i++) square[i] = EMPTY;
 
       square[E1] = WHITE_KING;
       square[D1] = WHITE_QUEEN;
       square[A1] = WHITE_ROOK;
       square[H1] = WHITE_ROOK;
       square[B1] = WHITE_KNIGHT;
       square[G1] = WHITE_KNIGHT;
       square[C1] = WHITE_BISHOP;
       square[F1] = WHITE_BISHOP;
       square[A2] = WHITE_PAWN;
       square[B2] = WHITE_PAWN;
       square[C2] = WHITE_PAWN;
       square[D2] = WHITE_PAWN;
       square[E2] = WHITE_PAWN;
       square[F2] = WHITE_PAWN;
       square[G2] = WHITE_PAWN;
       square[H2] = WHITE_PAWN;
 
       square[E8] = BLACK_KING;
       square[D8] = BLACK_QUEEN;
       square[A8] = BLACK_ROOK;
       square[H8] = BLACK_ROOK;
       square[B8] = BLACK_KNIGHT;
       square[G8] = BLACK_KNIGHT;
       square[C8] = BLACK_BISHOP;
       square[F8] = BLACK_BISHOP;
       square[A7] = BLACK_PAWN;
       square[B7] = BLACK_PAWN;
       square[C7] = BLACK_PAWN;
       square[D7] = BLACK_PAWN;
       square[E7] = BLACK_PAWN;
       square[F7] = BLACK_PAWN;
       square[G7] = BLACK_PAWN;
       square[H7] = BLACK_PAWN;
 
       initFromSquares(square, WHITE_MOVE, 0, CANCASTLEOO + CANCASTLEOOO, CANCASTLEOO + CANCASTLEOOO, 0);
 
       return;
}
 
void Board::initFromSquares(int input[64], unsigned char next, int fiftyM, int castleW, int castleB, int epSq)
{
       // sets up the board variables according to the information found in the input[64] array
       // All board & game initializations are done through this function (including setup).
 
       // bitboards
       whiteKing    = 0;
       whiteQueens  = 0;
       whiteRooks   = 0;
       whiteBishops = 0;
       whiteKnights = 0;
       whitePawns   = 0;
       blackKing    = 0;
       blackQueens  = 0;
       blackRooks   = 0;
       blackBishops = 0;
       blackKnights = 0;
       blackPawns   = 0;
       whitePieces  = 0;
       blackPieces  = 0;
       occupiedSquares = 0;
 
       // populate the 12 bitboard:
       for (int i = 0; i < 64; i++)
       {
              square[i] = input[i];
              if (square[i] == WHITE_KING)   whiteKing    = whiteKing    | BITSET[i];
              if (square[i] == WHITE_QUEEN)  whiteQueens  = whiteQueens  | BITSET[i];
              if (square[i] == WHITE_ROOK)   whiteRooks   = whiteRooks   | BITSET[i];
              if (square[i] == WHITE_BISHOP) whiteBishops = whiteBishops | BITSET[i];
              if (square[i] == WHITE_KNIGHT) whiteKnights = whiteKnights | BITSET[i];
              if (square[i] == WHITE_PAWN)   whitePawns   = whitePawns   | BITSET[i];
              if (square[i] == BLACK_KING)   blackKing    = blackKing    | BITSET[i];
              if (square[i] == BLACK_QUEEN)  blackQueens  = blackQueens  | BITSET[i];
              if (square[i] == BLACK_ROOK)   blackRooks   = blackRooks   | BITSET[i];
              if (square[i] == BLACK_BISHOP) blackBishops = blackBishops | BITSET[i];
              if (square[i] == BLACK_KNIGHT) blackKnights = blackKnights | BITSET[i];
              if (square[i] == BLACK_PAWN)   blackPawns   = blackPawns   | BITSET[i];
       }
 
       whitePieces = whiteKing | whiteQueens | whiteRooks | whiteBishops | whiteKnights | whitePawns;
       blackPieces = blackKing | blackQueens | blackRooks | blackBishops | blackKnights | blackPawns;
       occupiedSquares = whitePieces | blackPieces;
 
       nextMove = next;
       castleWhite = castleW;
       castleBlack = castleB;
       epSquare = epSq;
       fiftyMove = fiftyM;
 
       Material    =        bitCnt(whitePawns) * PAWN_VALUE +
                                  bitCnt(whiteKnights) * KNIGHT_VALUE +
                                  bitCnt(whiteBishops) * BISHOP_VALUE +
                                  bitCnt(whiteRooks) * ROOK_VALUE +
                                  bitCnt(whiteQueens) * QUEEN_VALUE;
       Material      -= (bitCnt(blackPawns) * PAWN_VALUE +
                                  bitCnt(blackKnights) * KNIGHT_VALUE +
                                  bitCnt(blackBishops) * BISHOP_VALUE +
                                  bitCnt(blackRooks) * ROOK_VALUE +
                                  bitCnt(blackQueens) * QUEEN_VALUE);
 
       return;
}
 
void Board::display()
{
       int rank, file;
 
       cout << endl;
       {
              if (!viewRotated)
              {
                     for (rank = 8; rank >= 1; rank--)
                     {
                           cout << "    +---+---+---+---+---+---+---+---+" << endl;
                           cout << setw(3) << rank <<  " |";
                           for (file = 1; file <= 8; file++)
                           {
                                  cout << " " << PIECENAMES[square[BOARDINDEX[file][rank]]] << "|";
                           }
                           cout << endl;
                     }
                     cout << "    +---+---+---+---+---+---+---+---+" << endl;
                     cout << "      a   b   c   d   e   f   g   h" << endl << endl;
              }
              else
              {
                     cout << "      h   g   f   e   d   c   b   a" << endl;
                     for (rank = 1; rank <= 8; rank++)
                     {
                           cout << "    +---+---+---+---+---+---+---+---+" << endl;
                           cout << "    |";
                           for (file = 8; file >= 1; file--)
                           {
                    cout << " " << PIECENAMES[square[BOARDINDEX[file][rank]]] << "|";
                           }
                           cout << setw(3) << rank << endl;
                     }
                     cout << "    +---+---+---+---+---+---+---+---+" << endl << endl;
              }
       }
       return;
}


int Board::eval()
{

	/*
	This is the evaluation function where the score is calculated from White's perspective (in centipawns)
	and returned from the perspective of the side to move.
	*/
	int score, square;
	int whitepawns, whiteknights, whitebishops, whiterooks, whitequeens;
	int blackpawns, blackknights, blackbishops, blackrooks, blackqueens;
	int whitekingsquare, blackkingsquare;
	int whitetotalmat, blacktotalmat;
	int whitetotal, blacktotal;
	bool endgame;
	BitMap temp, whitepassedpawns, blackpassedpawns;

	//Pieces type
	score = board.Material;
	
	// Remember where the kings are
	whitekingsquare = firstOne(board.whiteKing);
	blackkingsquare = firstOne(board.blackKing);
	
	// Piece counts
	whitepawns = bitCnt(board.whitePawns);
	whiteknights = bitCnt(board.whiteKnights);
	whitebishops = bitCnt(board.whiteBishops);
	whiterooks = bitCnt(board.whiteRooks);
	whitequeens = bitCnt(board.whiteQueens);
	whitetotalmat = 3 * whiteknights + 3 * whitebishops + 5 * whiterooks + 10 * whitequeens;
	whitetotal = whitepawns + whiteknights + whitebishops + whiterooks + whitequeens;
	blackpawns = bitCnt(board.blackPawns);
	blackknights = bitCnt(board.blackKnights);
	blackbishops = bitCnt(board.blackBishops);
	blackrooks = bitCnt(board.blackRooks);
	blackqueens = bitCnt(board.blackQueens);
	blacktotalmat = 3 * blackknights + 3 * blackbishops + 5 * blackrooks + 10 * blackqueens;
	blacktotal = blackpawns + blackknights + blackbishops + blackrooks + blackqueens;


	/*
	Check if it is the endgame
	Anything less than a queen (=10) + rook (=5) is considered endgame (pawns excluded in this count)
	*/
	endgame = (whitetotalmat < 15 || blacktotalmat < 15);


	// Draw check due to insufficient peices:
	if (!whitepawns && !blackpawns)
	{
		// king versus king:
		if ((whitetotalmat == 0) && (blacktotalmat == 0)) return 0;

		// king and knight versus king:
		if (((whitetotalmat == 3) && (whiteknights == 1) && (blacktotalmat == 0)) ||
			((blacktotalmat == 3) && (blackknights == 1) && (whitetotalmat == 0))) return 0;

		// 2 kings with one or more bishops, and all bishops on the same colour:
		if ((whitebishops + blackbishops) > 0)
		{
			if ((whiteknights == 0) && (whiterooks == 0) && (whitequeens == 0) &&
				(blackknights == 0) && (blackrooks == 0) && (blackqueens == 0))
			{
				if (!((board.whiteBishops | board.blackBishops) & WHITE_SQUARES) ||
					!((board.whiteBishops | board.blackBishops) & BLACK_SQUARES)) return 0;
			}
		}
	}

	/*
	Evaluating Pieces
	Have the winning side prefer to exchange pieces
	Every exchange with unequal material adds 3 centipawns to the score
	Loosing a piece (from balanced piece) becomes more severe in the endgame
	*/

	if (whitetotalmat + whitepawns > blacktotalmat + blackpawns)
	{
		score += 45 + 3 * whitetotal - 6 * blacktotal;
	}
	else
	{
		if (whitetotalmat + whitepawns < blacktotalmat + blackpawns)
		{
			score += -45 - 3 * blacktotal + 6 * whitetotal;
		}
	}

	/*
	Evaluate White Pieces
	*/

	/*
	Evaluate white pawns, based on:
	- position on the board
	- distance from opponent king
	- distance from own king
	- passed, doubled, isolated or backward pawns
	*/
	whitepassedpawns = 0;
	temp = board.whitePawns;
	while (temp)
	{
		square = firstOne(temp);
		score += PAWNPOS_W[square];
		score += PAWN_OPPONENT_DISTANCE[DISTANCE[square][blackkingsquare]];
		if (endgame)
		{
			score += PAWN_OWN_DISTANCE[DISTANCE[square][whitekingsquare]];
		}

		if (!(PASSED_WHITE[square] & board.blackPawns))
		{
			score += BONUS_PASSED_PAWN;

			// remember its location, used later when evaluating the white rooks:
			whitepassedpawns ^= BITSET[square];
		}

		if ((board.whitePawns ^ BITSET[square]) & FILEMASK[square])
		{
			score -= PENALTY_DOUBLED_PAWN;
		}

		if (!(ISOLATED_WHITE[square] & board.whitePawns))
		{
			score -= PENALTY_ISOLATED_PAWN;
		}
		else
		{
			/*
			If it is not isolated, then it might be backward. Two conditions must be true:
			1) if the next square is controlled by an enemy pawn - we use the PAWN_ATTACKS bitmaps to check this
			2) if there are no pawns left that could defend this pawn
			*/
			if ((WHITE_PAWN_ATTACKS[square + 8] & board.blackPawns))
			{
				if (!(BACKWARD_WHITE[square] & board.whitePawns))
				{
					score -= PENALTY_BACKWARD_PAWN;
				}
			}
		}

		temp ^= BITSET[square];
	}

	/*
	Evaluate white knights, based on:
	- position on the board
	- distance from opponent king
	*/

	temp = board.whiteKnights;
	while (temp)
	{
		square = firstOne(temp);
		score += KNIGHTPOS_W[square];
		score += KNIGHT_DISTANCE[DISTANCE[square][blackkingsquare]];
		temp ^= BITSET[square];
	}

	/*
	Evaluate white bishops, based on:
	- having the pair
	- position on the board
	- distance from opponent king
	*/

	if (board.whiteBishops)
	{
		if (board.whiteBishops & (board.whiteBishops - 1))
		{
			score += BONUS_BISHOP_PAIR;
		}
	}

	temp = board.whiteBishops;
	while (temp)
	{
		square = firstOne(temp);
		score += BISHOPPOS_W[square];
		score += BISHOP_DISTANCE[DISTANCE[square][blackkingsquare]];
		temp ^= BITSET[square];
	}

	/*
	Evaluate white rooks, based on:
	- position on the board
	- distance from opponent king
	- on the same file as a passed pawn
	*/

	temp = board.whiteRooks;
	while (temp)
	{
		square = firstOne(temp);
		score += ROOKPOS_W[square];
		score += ROOK_DISTANCE[DISTANCE[square][blackkingsquare]];
		if (FILEMASK[square] & whitepassedpawns)
		{
			if ((unsigned int)square < firstOne(FILEMASK[square] & whitepassedpawns))
			{
				score += BONUS_ROOK_BEHIND_PASSED_PAWN;
			}
		}
		temp ^= BITSET[square];
	}

	/*
	Evaluate white queens
	- position on the board
	- distance from opponent king
	*/

	temp = board.whiteQueens;
	while (temp)
	{
		square = firstOne(temp);
		score += QUEENPOS_W[square];
		score += QUEEN_DISTANCE[DISTANCE[square][blackkingsquare]];
		temp ^= BITSET[square];
	}

	/*
	Evaluate the white king, based on:
	- position on the board
	- proximity to the pawns
	- pawn shield (not in the endgame)
	*/

	if (endgame)
	{
		score += KINGPOS_ENDGAME_W[whitekingsquare];
	}
	else
	{
		score += KINGPOS_W[whitekingsquare];

		// add pawn shield bonus if not in the endgame:
		// strong pawn shield bonus if the pawns are near the king:
		score += BONUS_PAWN_SHIELD_STRONG * bitCnt(KINGSHIELD_STRONG_W[whitekingsquare] & board.whitePawns);

		// weaker pawn shield bonus if the pawns are not so near the king:
		score += BONUS_PAWN_SHIELD_WEAK * bitCnt(KINGSHIELD_WEAK_W[whitekingsquare] & board.whitePawns);
	}

	/*
	Evaluate BLACK PIECES
	*/

	/*
	Evaluate black pawns
	- position on the board
	- distance from opponent king
	- distance from own king
	- passed, doubled, isolated or backward pawns
	*/

	blackpassedpawns = 0;
	temp = board.blackPawns;
	while (temp)
	{
		square = firstOne(temp);
		score -= PAWNPOS_B[square];
		score -= PAWN_OPPONENT_DISTANCE[DISTANCE[square][whitekingsquare]];
		if (endgame)
		{
			score -= PAWN_OWN_DISTANCE[DISTANCE[square][blackkingsquare]];
		}

		if (!(PASSED_BLACK[square] & board.whitePawns))
		{
			score -= BONUS_PASSED_PAWN;

			// remember its location, used later when evaluating the black rooks:
			blackpassedpawns ^= BITSET[square];

		}

		if ((board.blackPawns ^ BITSET[square]) & FILEMASK[square])
		{
			score += PENALTY_DOUBLED_PAWN;
		}

		if (!(ISOLATED_BLACK[square] & board.blackPawns))
		{
			score += PENALTY_ISOLATED_PAWN;
		}
		else
		{
			/*If it is not isolated, then it might be backward. Two conditions must be true:
			1) if the next square is controlled by an enemy pawn - we use the PAWN_ATTACKS bitmaps to check this
			2) if there are no pawns left that could defend this pawn
			*/
			if ((BLACK_PAWN_ATTACKS[square - 8] & board.whitePawns))
			{
				if (!(BACKWARD_BLACK[square] & board.blackPawns))
				{
					score += PENALTY_BACKWARD_PAWN;
				}
			}
		}

		temp ^= BITSET[square];

	}

	/*
	Evaluate black knights
	- position on the board
	- distance from opponent king
	*/
	temp = board.blackKnights;
	while (temp)
	{
		square = firstOne(temp);
		score -= KNIGHTPOS_B[square];
		score -= KNIGHT_DISTANCE[DISTANCE[square][whitekingsquare]];
		temp ^= BITSET[square];
	}

	/*
	Evaluate black bishops, based on:
	- having the pair
	- position on the board
	- distance from opponent king
	*/

	if (board.blackBishops)
	{
		if (board.blackBishops & (board.blackBishops - 1))
		{
			score -= BONUS_BISHOP_PAIR;
		}
	}

	temp = board.blackBishops;
	while (temp)
	{
		square = firstOne(temp);
		score -= BISHOPPOS_B[square];
		score -= BISHOP_DISTANCE[DISTANCE[square][whitekingsquare]];
		temp ^= BITSET[square];
	}

	/*
	Evaluate black rooks, based on:
	- position on the board
	- distance from opponent king
	- on the same file as a passed pawn
	*/

	temp = board.blackRooks;
	while (temp)
	{
		square = firstOne(temp);
		score -= ROOKPOS_B[square];
		score -= ROOK_DISTANCE[DISTANCE[square][whitekingsquare]];
		if (FILEMASK[square] & blackpassedpawns)
		{
			if ((unsigned int)square > firstOne(FILEMASK[square] & blackpassedpawns))
			{
				score -= BONUS_ROOK_BEHIND_PASSED_PAWN;
			}
		}
		temp ^= BITSET[square];
	}

	/*
	Evaluate black queens, based on:
	- position on the board
	- distance from opponent king
	*/

	temp = board.blackQueens;
	while (temp)
	{
		square = firstOne(temp);
		score -= QUEENPOS_B[square];
		score -= QUEEN_DISTANCE[DISTANCE[square][whitekingsquare]];
		temp ^= BITSET[square];
	}

	/*
	Evaluate the black king
	- position on the board
	- proximity to the pawns
	- pawn shield (not in the endgame)
	*/

	if (endgame)
	{
		score -= KINGPOS_ENDGAME_B[blackkingsquare];
	}
	else
	{
		score -= KINGPOS_B[blackkingsquare];

		// add pawn shield bonus if not in the endgame:
		// strong pawn shield bonus if the pawns are near the king:
		score -= BONUS_PAWN_SHIELD_STRONG * bitCnt(KINGSHIELD_STRONG_B[blackkingsquare] & board.blackPawns);

		// weaker pawn shield bonus if the pawns are not so near the king:
		score -= BONUS_PAWN_SHIELD_WEAK * bitCnt(KINGSHIELD_WEAK_B[blackkingsquare] & board.blackPawns);

	}

	/*
	Return the score
	*/

	if (board.nextMove) return -score;
	else return score;

}


void Board::mirror()
{
	// Mirrors the board position

	int i, mirroredBoard[64];
	unsigned char nextMirror;
	int fiftyMMirror, castleWMirror, castleBMirror, epSqMirror;
	int kmoveBufLen, kendOfGame, kendOfSearch;

	nextMirror = !nextMove;
	fiftyMMirror = fiftyMove;
	castleWMirror = castleBlack;
	castleBMirror = castleWhite;
	if (epSquare)
	{
		if (epSquare < 24) epSqMirror = epSquare + 24;
		else epSqMirror = epSquare - 24;
	}

	for (i = 0; i < 64; i++)
	{
		mirroredBoard[i] = square[MIRROR[i]];
		// swap piece color (4th bit):
		if (mirroredBoard[i] != EMPTY) mirroredBoard[i] ^= 8;
	}

	// keep the move buffer as original:
	kmoveBufLen = moveBufLen[0];
	kendOfGame = endOfGame;
	kendOfSearch = endOfSearch;
	initFromSquares(mirroredBoard, nextMirror, fiftyMMirror, castleWMirror, castleBMirror, epSqMirror);
	moveBufLen[0] = kmoveBufLen;
	endOfGame = kendOfGame;
	endOfSearch = kendOfSearch;

	return;
}
