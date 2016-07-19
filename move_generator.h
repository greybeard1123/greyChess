
//Macros to define sliding attacks
#define RANKMOVES(a)       (RANK_ATTACKS[(a)][((board.occupiedSquares & RANKMASK[(a)]) >> RANKSHIFT[(a)])] & targetBitmap)
#define FILEMOVES(a)       (FILE_ATTACKS[(a)][((board.occupiedSquares & FILEMASK[(a)]) * FILEMAGIC[(a)]) >> 57] & targetBitmap)
#define SLIDEA8H1MOVES(a)  (DIAGA8H1_ATTACKS[(a)][((board.occupiedSquares & DIAGA8H1MASK[(a)]) * DIAGA8H1MAGIC[(a)]) >> 57] & targetBitmap)
#define SLIDEA1H8MOVES(a)  (DIAGA1H8_ATTACKS[(a)][((board.occupiedSquares & DIAGA1H8MASK[(a)]) * DIAGA1H8MAGIC[(a)]) >> 57] & targetBitmap)
#define ROOKMOVES(a)       (RANKMOVES(a) | FILEMOVES(a))
#define BISHOPMOVES(a)     (SLIDEA8H1MOVES(a) | SLIDEA1H8MOVES(a))
#define QUEENMOVES(a)      (BISHOPMOVES(a) | ROOKMOVES(a))

bool      isValidTextMove(char *, Move &);
bool	  toSan(Move &, char *);
void      makeMove(Move &);
void      unmakeMove(Move &);
Move NOMOVE;



int movegen(int index)
{
	//Bitmap move generator (no check if a move leaves the king in check)
	unsigned char opponentSide;
	unsigned int from, to;
	BitMap tempPiece, tempMove;
	BitMap targetBitmap, freeSquares;
	Move move;

	move.clear();
	opponentSide = (!board.get_nextMove());
	freeSquares = ~board.occupiedSquares;

	// ---------------------------------------BLACK'S MOVE GENERATOR-------------------------------------------------

	if (board.get_nextMove()) // black to move
	{
		targetBitmap = ~board.blackPieces; //only whitePieces are checked


		// -----------------Black Pawns-----------------
		move.setPiec(BLACK_PAWN);
		tempPiece = board.blackPawns;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = BLACK_PAWN_MOVES[from] & freeSquares;                // normal moves

			if (RANKS[from] == 7 && tempMove)
				tempMove |= (BLACK_PAWN_DOUBLE_MOVES[from] & freeSquares);  // add double moves

			tempMove |= BLACK_PAWN_ATTACKS[from] & board.whitePieces;       // add captures

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);

				if ((RANKS[to]) == 1)                                       // add promotions
				{
					move.setProm(BLACK_QUEEN);
					board.moveBuffer[index++].moveInt = move.moveInt;

					move.setProm(BLACK_ROOK);
					board.moveBuffer[index++].moveInt = move.moveInt;

					move.setProm(BLACK_BISHOP);
					board.moveBuffer[index++].moveInt = move.moveInt;

					move.setProm(BLACK_KNIGHT);
					board.moveBuffer[index++].moveInt = move.moveInt;

					move.setProm(EMPTY);
				}
				else
				{
					board.moveBuffer[index++].moveInt = move.moveInt;
				}

				tempMove ^= BITSET[to];
			}

			// add en-passant captures:
			if (board.get_epSquare())   // do a quick check first
			{
				if (BLACK_PAWN_ATTACKS[from] & BITSET[board.get_epSquare()])
				{
					move.setProm(BLACK_PAWN);
					move.setCapt(WHITE_PAWN);
					move.setTosq(board.get_epSquare());
					board.moveBuffer[index++].moveInt = move.moveInt;
				}
			}
			tempPiece ^= BITSET[from];
			move.setProm(EMPTY);
		}


		// -----------------Black Knights----------------- THE DARK KNIGHT!!!
		move.setPiec(BLACK_KNIGHT);
		tempPiece = board.blackKnights;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = KNIGHT_ATTACKS[from] & targetBitmap;

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}
			tempPiece ^= BITSET[from];
		}

		// -----------------Black Bishops-----------------
		move.setPiec(BLACK_BISHOP);
		tempPiece = board.blackBishops;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = BISHOPMOVES(from);   // see Macro

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}
			tempPiece ^= BITSET[from];
		}

		// -----------------Black Rooks-----------------
		move.setPiec(BLACK_ROOK);
		tempPiece = board.blackRooks;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = ROOKMOVES(from);   // see Macro

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}
			tempPiece ^= BITSET[from];
		}

		// -----------------Black Queens-----------------
		move.setPiec(BLACK_QUEEN);
		tempPiece = board.blackQueens;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = QUEENMOVES(from);   // see Macro

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}
			tempPiece ^= BITSET[from];
		}

		// -----------------Black King-----------------
		move.setPiec(BLACK_KING);
		tempPiece = board.blackKing;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = KING_ATTACKS[from] & targetBitmap;

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}

			//     Black 0-0 Castling:
			if (board.get_castleBlack() & CANCASTLEOO)
			{
				if (!(maskFG[1] & board.occupiedSquares))
				{
					if (!isAttacked(maskEG[BLACK_MOVE], WHITE_MOVE))
					{
						board.moveBuffer[index++].moveInt = BLACK_OO_CASTL;
					}
				}
			}
			//     Black 0-0-0 Castling:
			if (board.get_castleBlack() & CANCASTLEOOO)
			{
				if (!(maskBD[1] & board.occupiedSquares))
				{
					if (!isAttacked(maskCE[BLACK_MOVE], WHITE_MOVE))
					{
						board.moveBuffer[index++].moveInt = BLACK_OOO_CASTL;
					}
				}
			}
			tempPiece ^= BITSET[from];
			move.setProm(EMPTY);
		}
	}

	// ---------------------------------------WHITES'S MOVE GENERATOR-------------------------------------------------

	else
	{
		targetBitmap = ~board.whitePieces; // select only black pieces

		// -----------------White King-----------------
		move.setPiec(WHITE_PAWN);
		tempPiece = board.whitePawns;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = WHITE_PAWN_MOVES[from] & freeSquares;                // normal moves

			if (RANKS[from] == 2 && tempMove)
				tempMove |= (WHITE_PAWN_DOUBLE_MOVES[from] & freeSquares);  // add double moves

			tempMove |= WHITE_PAWN_ATTACKS[from] & board.blackPieces;       // add captures
			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);

				if ((RANKS[to]) == 8)                                       // add promotions
				{
					move.setProm(WHITE_QUEEN);
					board.moveBuffer[index++].moveInt = move.moveInt;

					move.setProm(WHITE_ROOK);
					board.moveBuffer[index++].moveInt = move.moveInt;

					move.setProm(WHITE_BISHOP);
					board.moveBuffer[index++].moveInt = move.moveInt;

					move.setProm(WHITE_KNIGHT);
					board.moveBuffer[index++].moveInt = move.moveInt;

					move.setProm(EMPTY);
				}
				else
				{
					board.moveBuffer[index++].moveInt = move.moveInt;
				}
				tempMove ^= BITSET[to];
			}

			// add en-passant captures:
			if (board.get_epSquare())   // do a quick check first
			{
				if (WHITE_PAWN_ATTACKS[from] & BITSET[board.get_epSquare()])
				{
					move.setProm(WHITE_PAWN);
					move.setCapt(BLACK_PAWN);
					move.setTosq(board.get_epSquare());
					board.moveBuffer[index++].moveInt = move.moveInt;
				}
			}
			tempPiece ^= BITSET[from];
			move.setProm(EMPTY);
		}

		// -----------------White Knight's-----------------
		move.setPiec(WHITE_KNIGHT);
		tempPiece = board.whiteKnights;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = KNIGHT_ATTACKS[from] & targetBitmap;

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}
			tempPiece ^= BITSET[from];
		}

		// -----------------White Bishops-----------------
		move.setPiec(WHITE_BISHOP);
		tempPiece = board.whiteBishops;
		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = BISHOPMOVES(from);   // see Macro's
			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}
			tempPiece ^= BITSET[from];
		}

		// -----------------White King-----------------
		move.setPiec(WHITE_ROOK);
		tempPiece = board.whiteRooks;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = ROOKMOVES(from);   // see Macro

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}
			tempPiece ^= BITSET[from];
		}

		// -----------------White Queens-----------------
		move.setPiec(WHITE_QUEEN);
		tempPiece = board.whiteQueens;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = QUEENMOVES(from);   // see Macro

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}
			tempPiece ^= BITSET[from];
		}

		// -----------------White King-----------------
		move.setPiec(WHITE_KING);
		tempPiece = board.whiteKing;

		while (tempPiece)
		{
			from = firstOne(tempPiece);
			move.setFrom(from);
			tempMove = KING_ATTACKS[from] & targetBitmap;

			while (tempMove)
			{
				to = firstOne(tempMove);
				move.setTosq(to);
				move.setCapt(board.square[to]);
				board.moveBuffer[index++].moveInt = move.moveInt;
				tempMove ^= BITSET[to];
			}

			//     White 0-0 Castling:
			if (board.get_castleWhite() & CANCASTLEOO)
			{
				if (!(maskFG[0] & board.occupiedSquares))
				{
					if (!isAttacked(maskEG[WHITE_MOVE], BLACK_MOVE))
					{
						board.moveBuffer[index++].moveInt = WHITE_OO_CASTL;
					}
				}
			}

			//     White 0-0-0 Castling:
			if (board.get_castleWhite() & CANCASTLEOOO)
			{
				if (!(maskBD[0] & board.occupiedSquares))
				{
					if (!isAttacked(maskCE[WHITE_MOVE], BLACK_MOVE))
					{
						board.moveBuffer[index++].moveInt = WHITE_OOO_CASTL;
					}
				}
			}
			tempPiece ^= BITSET[from];
			move.setProm(EMPTY);
		}
	}
	return index;
}

/*
Move Legality tester
Used to check if targetBitmap was attacked by blackor white
Returns true if fisrt attack found and false if no attack found
It is also used for "check" detection and castling legality
*/
bool isAttacked(BitMap &targetBitmap, const char &fromSide)
{
	BitMap tempTarget;
	BitMap slidingAttackers;
	unsigned int to;

	tempTarget = targetBitmap;

	if (fromSide) // test for attacks from BLACK on targetBitmap
	{
		while (tempTarget)
		{
			to = firstOne(tempTarget);

			if (board.blackPawns & WHITE_PAWN_ATTACKS[to]) return true;
			if (board.blackKnights & KNIGHT_ATTACKS[to]) return true;
			if (board.blackKing & KING_ATTACKS[to]) return true;

			// file or rank attacks
			slidingAttackers = board.blackQueens | board.blackRooks;
			if (slidingAttackers)
			{
				if (RANK_ATTACKS[to][((board.occupiedSquares & RANKMASK[to]) >> RANKSHIFT[to])] & slidingAttackers) return true;
				if (FILE_ATTACKS[to][((board.occupiedSquares & FILEMASK[to]) * FILEMAGIC[to]) >> 57] & slidingAttackers) return true;
			}

			// diagonals
			slidingAttackers = board.blackQueens | board.blackBishops;
			if (slidingAttackers)
			{
				if (DIAGA8H1_ATTACKS[to][((board.occupiedSquares & DIAGA8H1MASK[to]) * DIAGA8H1MAGIC[to]) >> 57] & slidingAttackers) return true;
				if (DIAGA1H8_ATTACKS[to][((board.occupiedSquares & DIAGA1H8MASK[to]) * DIAGA1H8MAGIC[to]) >> 57] & slidingAttackers) return true;
			}

			tempTarget ^= BITSET[to];
		}
	}
	else // test for attacks from WHITE on targetBitmap
	{
		while (tempTarget)
		{
			to = firstOne(tempTarget);

			if (board.whitePawns & BLACK_PAWN_ATTACKS[to]) return true;
			if (board.whiteKnights & KNIGHT_ATTACKS[to]) return true;
			if (board.whiteKing & KING_ATTACKS[to]) return true;

			// file or rank attacks
			slidingAttackers = board.whiteQueens | board.whiteRooks;
			if (slidingAttackers)
			{
				if (RANK_ATTACKS[to][((board.occupiedSquares & RANKMASK[to]) >> RANKSHIFT[to])] & slidingAttackers) return true;
				if (FILE_ATTACKS[to][((board.occupiedSquares & FILEMASK[to]) * FILEMAGIC[to]) >> 57] & slidingAttackers) return true;
			}

			// diagonals:
			slidingAttackers = board.whiteQueens | board.whiteBishops;
			if (slidingAttackers)
			{
				if (DIAGA8H1_ATTACKS[to][((board.occupiedSquares & DIAGA8H1MASK[to]) * DIAGA8H1MAGIC[to]) >> 57] & slidingAttackers) return true;
				if (DIAGA1H8_ATTACKS[to][((board.occupiedSquares & DIAGA1H8MASK[to]) * DIAGA1H8MAGIC[to]) >> 57] & slidingAttackers) return true;
			}

			tempTarget ^= BITSET[to];
		}
	}
	return false;
}

/*
void displayMove(Move &move)
{
	// displays a single move on the console, no disambiguation

	if ((move.getPiec() == WHITE_KING) && (move.isCastleOO()))
	{
		cout << "O-O";
		return;
	};
	if ((move.getPiec() == WHITE_KING) && (move.isCastleOOO()))
	{
		cout << "O-O-O";
		return;
	};
	if ((move.getPiec() == BLACK_KING) && (move.isCastleOO()))
	{
		cout << "O-O";
		return;
	};
	if ((move.getPiec() == BLACK_KING) && (move.isCastleOOO()))
	{
		cout << "O-O-O";
		return;
	};

	if ((move.getPiec() == WHITE_ROOK) || (move.getPiec() == BLACK_ROOK))   cout << "R";
	if ((move.getPiec() == WHITE_BISHOP) || (move.getPiec() == BLACK_BISHOP)) cout << "B";
	if ((move.getPiec() == WHITE_KNIGHT) || (move.getPiec() == BLACK_KNIGHT)) cout << "N";
	if ((move.getPiec() == WHITE_KING) || (move.getPiec() == BLACK_KING))   cout << "K";
	if ((move.getPiec() == WHITE_QUEEN) || (move.getPiec() == BLACK_QUEEN))  cout << "Q";
	if (((move.getPiec() == WHITE_PAWN) || (move.getPiec() == BLACK_PAWN)) && move.isCapture())
	{
		if (FILES[move.getFrom()] == 1) cout << "a";
		if (FILES[move.getFrom()] == 2) cout << "b";
		if (FILES[move.getFrom()] == 3) cout << "c";
		if (FILES[move.getFrom()] == 4) cout << "d";
		if (FILES[move.getFrom()] == 5) cout << "e";
		if (FILES[move.getFrom()] == 6) cout << "f";
		if (FILES[move.getFrom()] == 7) cout << "g";
		if (FILES[move.getFrom()] == 8) cout << "h";
	}

	if (move.isCapture()) cout << "x";

	if (FILES[move.getTosq()] == 1) cout << "a";
	if (FILES[move.getTosq()] == 2) cout << "b";
	if (FILES[move.getTosq()] == 3) cout << "c";
	if (FILES[move.getTosq()] == 4) cout << "d";
	if (FILES[move.getTosq()] == 5) cout << "e";
	if (FILES[move.getTosq()] == 6) cout << "f";
	if (FILES[move.getTosq()] == 7) cout << "g";
	if (FILES[move.getTosq()] == 8) cout << "h";

	cout << RANKS[move.getTosq()];

	if (move.isPromotion())
	{
		if ((move.getProm() == WHITE_ROOK) || (move.getProm() == BLACK_ROOK))   cout << "=R";
		if ((move.getProm() == WHITE_BISHOP) || (move.getProm() == BLACK_BISHOP)) cout << "=B";
		if ((move.getProm() == WHITE_KNIGHT) || (move.getProm() == BLACK_KNIGHT)) cout << "=N";
		if ((move.getProm() == WHITE_KING) || (move.getProm() == BLACK_KING))   cout << "=K";
		if ((move.getProm() == WHITE_QUEEN) || (move.getProm() == BLACK_QUEEN))  cout << "=Q";
	}
	cout.flush();
	return;
} */

void makeMove(Move &move)
{
	unsigned int from = move.getFrom();
	unsigned int to = move.getTosq();
	unsigned int piece = move.getPiec();
	unsigned int captured = move.getCapt();

	BitMap fromBitMap = BITSET[from];
	BitMap fromToBitMap = fromBitMap | BITSET[to];

	board.move_tree_object[board.endOfSearch].move.moveInt = move.moveInt;
	board.move_tree_object[board.endOfSearch].castleWhite = board.get_castleWhite();
	board.move_tree_object[board.endOfSearch].castleBlack = board.get_castleBlack();
	board.move_tree_object[board.endOfSearch].fiftyMove = board.get_fiftyMove();
	board.move_tree_object[board.endOfSearch].epSquare = board.get_epSquare();
	board.endOfSearch++;

	switch (piece)
	{
	case 1: // white pawn:
		board.whitePawns ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = WHITE_PAWN;
		board.set_epSquare(0);
		board.set_fiftyMove(0);
		if (RANKS[from] == 2)
		if (RANKS[to] == 4) board.set_epSquare(from + 8);
		if (captured)
		{
			if (move.isEnpassant())
			{
				board.blackPawns ^= BITSET[to - 8];
				board.blackPieces ^= BITSET[to - 8];
				board.occupiedSquares ^= fromToBitMap | BITSET[to - 8];
				board.square[to - 8] = EMPTY;
				board.set_Material( board.get_Material() + PAWN_VALUE);
			}
			else
			{
				makeCapture(captured, to);
				board.occupiedSquares ^= fromBitMap;
			}
		}
		else board.occupiedSquares ^= fromToBitMap;

		if (move.isPromotion())
		{
			makeWhitePromotion(move.getProm(), to);
			board.square[to] = move.getProm();
		}
		break;

	case 2: // white king:
		board.whiteKing ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = WHITE_KING;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		board.set_castleWhite(0);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;

		if (move.isCastle())
		{
			if (move.isCastleOO())
			{
				board.whiteRooks ^= BITSET[H1] | BITSET[F1];
				board.whitePieces ^= BITSET[H1] | BITSET[F1];
				board.occupiedSquares ^= BITSET[H1] | BITSET[F1];
				board.square[H1] = EMPTY;
				board.square[F1] = WHITE_ROOK;
			}
			else
			{
				board.whiteRooks ^= BITSET[A1] | BITSET[D1];
				board.whitePieces ^= BITSET[A1] | BITSET[D1];
				board.occupiedSquares ^= BITSET[A1] | BITSET[D1];
				board.square[A1] = EMPTY;
				board.square[D1] = WHITE_ROOK;
			}
		}
		break;

	case 3: // white knight:
		board.whiteKnights ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = WHITE_KNIGHT;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 5: // white bishop:
		board.whiteBishops ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = WHITE_BISHOP;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 6: // white rook:
		board.whiteRooks ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = WHITE_ROOK;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		if (from == A1) board.set_castleWhite(board.get_castleWhite() & ~CANCASTLEOOO);
		if (from == H1) board.set_castleWhite(board.get_castleWhite() & ~CANCASTLEOO);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 7: // white queen:
		board.whiteQueens ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = WHITE_QUEEN;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 9: // black pawn:
		board.blackPawns ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = BLACK_PAWN;
		board.set_epSquare(0);
		board.set_fiftyMove(0);
		if (RANKS[from] == 7)
		if (RANKS[to] == 5) board.set_epSquare(from - 8);
		if (captured)
		{
			if (move.isEnpassant())
			{
				board.whitePawns ^= BITSET[to + 8];
				board.whitePieces ^= BITSET[to + 8];
				board.occupiedSquares ^= fromToBitMap | BITSET[to + 8];
				board.square[to + 8] = EMPTY;
				board.set_Material( board.get_Material() - PAWN_VALUE);
			}
			else
			{
				makeCapture(captured, to);
				board.occupiedSquares ^= fromBitMap;
			}
		}
		else board.occupiedSquares ^= fromToBitMap;

		if (move.isPromotion())
		{
			makeBlackPromotion(move.getProm(), to);
			board.square[to] = move.getProm();
		}
		break;

	case 10: // black king:
		board.blackKing ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = BLACK_KING;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		board.set_castleBlack(0);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;

		if (move.isCastle())
		{
			if (move.isCastleOO())
			{
				board.blackRooks ^= BITSET[H8] | BITSET[F8];
				board.blackPieces ^= BITSET[H8] | BITSET[F8];
				board.occupiedSquares ^= BITSET[H8] | BITSET[F8];
				board.square[H8] = EMPTY;
				board.square[F8] = BLACK_ROOK;
			}
			else
			{
				board.blackRooks ^= BITSET[A8] | BITSET[D8];
				board.blackPieces ^= BITSET[A8] | BITSET[D8];
				board.occupiedSquares ^= BITSET[A8] | BITSET[D8];
				board.square[A8] = EMPTY;
				board.square[D8] = BLACK_ROOK;
			}
		}
		break;

	case 11: // black knight:
		board.blackKnights ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = BLACK_KNIGHT;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 13: // black bishop:
		board.blackBishops ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = BLACK_BISHOP;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 14: // black rook:
		board.blackRooks ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = BLACK_ROOK;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		if (from == A8) board.set_castleBlack(board.get_castleBlack() & ~CANCASTLEOOO);
		if (from == H8) board.set_castleBlack(board.get_castleBlack() & ~CANCASTLEOO);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 15: // black queen:
		board.blackQueens ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = EMPTY;
		board.square[to] = BLACK_QUEEN;
		board.set_epSquare(0);
		board.set_fiftyMove(board.get_fiftyMove() + 1);
		if (captured)
		{
			makeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;
	}
	board.set_nextMove(!board.get_nextMove());


}

void unmakeMove(Move &move)
{
	unsigned int piece = move.getPiec();
	unsigned int captured = move.getCapt();
	unsigned int from = move.getFrom();
	unsigned int to = move.getTosq();

	BitMap fromBitMap = BITSET[from];
	BitMap fromToBitMap = fromBitMap | BITSET[to];

	switch (piece)
	{
	case 1: // white pawn:
		board.whitePawns ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = WHITE_PAWN;
		board.square[to] = EMPTY;
		if (captured)
		{
			if (move.isEnpassant())
			{
				board.blackPawns ^= BITSET[to - 8];
				board.blackPieces ^= BITSET[to - 8];
				board.occupiedSquares ^= fromToBitMap | BITSET[to - 8];
				board.square[to - 8] = BLACK_PAWN;
				board.set_Material( board.get_Material() - PAWN_VALUE);
			}
			else
			{
				unmakeCapture(captured, to);
				board.occupiedSquares ^= fromBitMap;
			}
		}
		else board.occupiedSquares ^= fromToBitMap;

		if (move.isPromotion())
		{
			unmakeWhitePromotion(move.getProm(), to);
		}
		break;

	case 2: // white king:
		board.whiteKing ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = WHITE_KING;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;

		if (move.isCastle())
		{
			if (move.isCastleOO())
			{
				board.whiteRooks ^= BITSET[H1] | BITSET[F1];
				board.whitePieces ^= BITSET[H1] | BITSET[F1];
				board.occupiedSquares ^= BITSET[H1] | BITSET[F1];
				board.square[H1] = WHITE_ROOK;
				board.square[F1] = EMPTY;
			}
			else
			{
				board.whiteRooks ^= BITSET[A1] | BITSET[D1];
				board.whitePieces ^= BITSET[A1] | BITSET[D1];
				board.occupiedSquares ^= BITSET[A1] | BITSET[D1];
				board.square[A1] = WHITE_ROOK;
				board.square[D1] = EMPTY;
			}
		}
		break;

	case 3: // white knight:
		board.whiteKnights ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = WHITE_KNIGHT;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 5: // white bishop:
		board.whiteBishops ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = WHITE_BISHOP;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;

		break;

	case 6: // white rook:
		board.whiteRooks ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = WHITE_ROOK;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 7: // white queen:
		board.whiteQueens ^= fromToBitMap;
		board.whitePieces ^= fromToBitMap;
		board.square[from] = WHITE_QUEEN;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 9: // black pawn:
		board.blackPawns ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = BLACK_PAWN;
		board.square[to] = EMPTY;
		if (captured)
		{
			if (move.isEnpassant())
			{
				board.whitePawns ^= BITSET[to + 8];
				board.whitePieces ^= BITSET[to + 8];
				board.occupiedSquares ^= fromToBitMap | BITSET[to + 8];
				board.square[to + 8] = WHITE_PAWN;
				board.set_Material( board.get_Material() + PAWN_VALUE);
			}
			else
			{
				unmakeCapture(captured, to);
				board.occupiedSquares ^= fromBitMap;
			}
		}
		else board.occupiedSquares ^= fromToBitMap;

		if (move.isPromotion())
		{
			unmakeBlackPromotion(move.getProm(), to);
		}
		break;

	case 10: // black king:
		board.blackKing ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = BLACK_KING;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;

		if (move.isCastle())
		{
			if (move.isCastleOO())
			{
				board.blackRooks ^= BITSET[H8] | BITSET[F8];
				board.blackPieces ^= BITSET[H8] | BITSET[F8];
				board.occupiedSquares ^= BITSET[H8] | BITSET[F8];
				board.square[H8] = BLACK_ROOK;
				board.square[F8] = EMPTY;
			}
			else
			{
				board.blackRooks ^= BITSET[A8] | BITSET[D8];
				board.blackPieces ^= BITSET[A8] | BITSET[D8];
				board.occupiedSquares ^= BITSET[A8] | BITSET[D8];
				board.square[A8] = BLACK_ROOK;
				board.square[D8] = EMPTY;
			}
		}
		break;

	case 11: // black knight:
		board.blackKnights ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = BLACK_KNIGHT;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 13: // black bishop:
		board.blackBishops ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = BLACK_BISHOP;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 14: // black rook:
		board.blackRooks ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = BLACK_ROOK;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;

	case 15: // black queen:
		board.blackQueens ^= fromToBitMap;
		board.blackPieces ^= fromToBitMap;
		board.square[from] = BLACK_QUEEN;
		board.square[to] = EMPTY;
		if (captured)
		{
			unmakeCapture(captured, to);
			board.occupiedSquares ^= fromBitMap;
		}
		else board.occupiedSquares ^= fromToBitMap;
		break;
	}

	board.endOfSearch--;
	board.set_castleWhite(board.move_tree_object[board.endOfSearch].castleWhite);
	board.set_castleBlack(board.move_tree_object[board.endOfSearch].castleBlack);
	board.set_epSquare(board.move_tree_object[board.endOfSearch].epSquare);
	board.set_fiftyMove(board.move_tree_object[board.endOfSearch].fiftyMove);

	board.set_nextMove(!board.get_nextMove());

}

void makeCapture(unsigned int &captured, unsigned int &to)
{
	// deals with all captures, except en-passant
	BitMap toBitMap;
	toBitMap = BITSET[to];

	switch (captured)
	{
	case 1: // white pawn:
		board.whitePawns ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.set_Material( board.get_Material() - PAWN_VALUE);
		break;

	case 2: // white king:
		board.whiteKing ^= toBitMap;
		board.whitePieces ^= toBitMap;
		break;

	case 3: // white knight:
		board.whiteKnights ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.set_Material( board.get_Material() - KNIGHT_VALUE);
		break;

	case 5: // white bishop:
		board.whiteBishops ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.set_Material( board.get_Material() - BISHOP_VALUE);
		break;

	case 6: // white rook:
		board.whiteRooks ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.set_Material( board.get_Material() - ROOK_VALUE);
		if (to == A1) board.set_castleWhite(board.get_castleWhite() & ~CANCASTLEOOO);
		if (to == H1) board.set_castleWhite(board.get_castleWhite() & ~CANCASTLEOO);
		break;

	case 7: // white queen:
		board.whiteQueens ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.set_Material( board.get_Material() - QUEEN_VALUE);
		break;

	case 9: // black pawn:
		board.blackPawns ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.set_Material( board.get_Material() + PAWN_VALUE);
		break;

	case 10: // black king:
		board.blackKing ^= toBitMap;
		board.blackPieces ^= toBitMap;
		break;

	case 11: // black knight:
		board.blackKnights ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.set_Material( board.get_Material() + KNIGHT_VALUE);
		break;

	case 13: // black bishop:
		board.blackBishops ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.set_Material( board.get_Material() + BISHOP_VALUE);
		break;

	case 14: // black rook:
		board.blackRooks ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.set_Material( board.get_Material() + ROOK_VALUE);
		if (to == A8) board.set_castleBlack(board.get_castleBlack() & ~CANCASTLEOOO);
		if (to == H8) board.set_castleBlack(board.get_castleBlack() & ~CANCASTLEOO);
		break;

	case 15: // black queen:
		board.blackQueens ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.set_Material( board.get_Material() + QUEEN_VALUE);
		break;
	}
	board.set_fiftyMove(0);
}

void unmakeCapture(unsigned int &captured, unsigned int &to)
{
	// deals with all captures, except en-passant
	BitMap toBitMap;
	toBitMap = BITSET[to];

	switch (captured)
	{
	case 1: // white pawn:
		board.whitePawns ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.square[to] = WHITE_PAWN;
		board.set_Material( board.get_Material() + PAWN_VALUE);
		break;

	case 2: // white king:
		board.whiteKing ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.square[to] = WHITE_KING;
		break;

	case 3: // white knight:
		board.whiteKnights ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.square[to] = WHITE_KNIGHT;
		board.set_Material( board.get_Material() + KNIGHT_VALUE);
		break;

	case 5: // white bishop:
		board.whiteBishops ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.square[to] = WHITE_BISHOP;
		board.set_Material( board.get_Material() + BISHOP_VALUE);
		break;

	case 6: // white rook:
		board.whiteRooks ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.square[to] = WHITE_ROOK;
		board.set_Material( board.get_Material() + ROOK_VALUE);
		break;

	case 7: // white queen:
		board.whiteQueens ^= toBitMap;
		board.whitePieces ^= toBitMap;
		board.square[to] = WHITE_QUEEN;
		board.set_Material( board.get_Material() + QUEEN_VALUE);
		break;

	case 9: // black pawn:
		board.blackPawns ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.square[to] = BLACK_PAWN;
		board.set_Material( board.get_Material() - PAWN_VALUE);
		break;

	case 10: // black king:
		board.blackKing ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.square[to] = BLACK_KING;
		break;

	case 11: // black knight:
		board.blackKnights ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.square[to] = BLACK_KNIGHT;
		board.set_Material( board.get_Material() - KNIGHT_VALUE);
		break;

	case 13: // black bishop:
		board.blackBishops ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.square[to] = BLACK_BISHOP;
		board.set_Material( board.get_Material() - BISHOP_VALUE);
		break;

	case 14: // black rook:
		board.blackRooks ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.square[to] = BLACK_ROOK;
		board.set_Material( board.get_Material() - ROOK_VALUE);
		break;

	case 15: // black queen:
		board.blackQueens ^= toBitMap;
		board.blackPieces ^= toBitMap;
		board.square[to] = BLACK_QUEEN;
		board.set_Material( board.get_Material() - QUEEN_VALUE);
		break;
	}
}

void makeWhitePromotion(unsigned int prom, unsigned int &to)
{
	BitMap toBitMap;
	toBitMap = BITSET[to];

	board.whitePawns ^= toBitMap;
	board.set_Material( board.get_Material() - PAWN_VALUE);

	if (prom == 7)
	{
		board.whiteQueens ^= toBitMap;
		board.set_Material( board.get_Material() + QUEEN_VALUE);
	}
	else if (prom == 6)
	{
		board.whiteRooks ^= toBitMap;
		board.set_Material( board.get_Material() + ROOK_VALUE);
	}
	else if (prom == 5)
	{
		board.whiteBishops ^= toBitMap;
		board.set_Material( board.get_Material() + BISHOP_VALUE);
	}
	else if (prom == 3)
	{
		board.whiteKnights ^= toBitMap;
		board.set_Material( board.get_Material() + KNIGHT_VALUE);
	}
}

void unmakeWhitePromotion(unsigned int prom, unsigned int &to)
{
	BitMap toBitMap;
	toBitMap = BITSET[to];

	board.whitePawns ^= toBitMap;
	board.set_Material( board.get_Material() + PAWN_VALUE);

	if (prom == 7)
	{
		board.whiteQueens ^= toBitMap;
		board.set_Material( board.get_Material() - QUEEN_VALUE);
	}
	else if (prom == 6)
	{
		board.whiteRooks ^= toBitMap;
		board.set_Material( board.get_Material() - ROOK_VALUE);
	}
	else if (prom == 5)
	{
		board.whiteBishops ^= toBitMap;
		board.set_Material( board.get_Material() - BISHOP_VALUE);
	}
	else if (prom == 3)
	{
		board.whiteKnights ^= toBitMap;
		board.set_Material( board.get_Material() - KNIGHT_VALUE);
	}
}


void makeBlackPromotion(unsigned int prom, unsigned int &to)
{
	BitMap toBitMap;
	toBitMap = BITSET[to];

	board.blackPawns ^= toBitMap;
	board.set_Material(board.get_Material() + PAWN_VALUE);

	if (prom == 15)
	{
		board.blackQueens ^= toBitMap;
		board.set_Material(board.get_Material() - QUEEN_VALUE);
	}
	else if (prom == 14)
	{
		board.blackRooks ^= toBitMap;
		board.set_Material(board.get_Material() - ROOK_VALUE);
	}
	else if (prom == 13)
	{
		board.blackBishops ^= toBitMap;
		board.set_Material(board.get_Material() - BISHOP_VALUE);
	}
	else if (prom == 11)
	{
		board.blackKnights ^= toBitMap;
		board.set_Material(board.get_Material() - KNIGHT_VALUE);
	}
}

void unmakeBlackPromotion(unsigned int prom, unsigned int &to)
{
	BitMap toBitMap;
	toBitMap = BITSET[to];

	board.blackPawns ^= toBitMap;
	board.set_Material(board.get_Material() - PAWN_VALUE);

	if (prom == 15)
	{
		board.blackQueens ^= toBitMap;
		board.set_Material(board.get_Material() + QUEEN_VALUE);
	}
	else if (prom == 14)
	{
		board.blackRooks ^= toBitMap;
		board.set_Material(board.get_Material() + ROOK_VALUE);
	}
	else if (prom == 13)
	{
		board.blackBishops ^= toBitMap;
		board.set_Material(board.get_Material() + BISHOP_VALUE);
	}
	else if (prom == 11)
	{
		board.blackKnights ^= toBitMap;
		board.set_Material(board.get_Material() + KNIGHT_VALUE);
	}
}

bool isOtherKingAttacked()
{
	// check to see if we are leaving our king in check
	if (board.get_nextMove())
	{
		return isAttacked(board.whiteKing, board.get_nextMove());
	}
	else
	{
		return isAttacked(board.blackKing, board.get_nextMove());
	}
}

bool isOwnKingAttacked()
{
	// check to see if we are leaving our king in check
	if (board.get_nextMove())
	{
		return isAttacked(board.blackKing, !(board.get_nextMove()));
	}
	else
	{
		return isAttacked(board.whiteKing, !(board.get_nextMove()));
	}
}


bool isValidTextMove(char *userMove, Move &move)
{
	// Checks if userMove is valid by comparing it with moves from the move generator
	// If found valid, the move is returned

	unsigned char userFrom, userTo, userPromote;
	bool moveFound;
	unsigned int i;

	if (strlen(userMove) > 3)
	{
		userFrom = userMove[0] - 97;
		userFrom += 8 * (userMove[1] - 49);
		userTo = userMove[2] - 97;
		userTo += 8 * (userMove[3] - 49);
	}

	userPromote = 0;
	if (strlen(userMove) > 4)
	{
		if (board.get_nextMove() == WHITE_MOVE)
		{
			switch (userMove[4])
			{
			case 'q': userPromote = WHITE_QUEEN; break;
			case 'r': userPromote = WHITE_ROOK; break;
			case 'b': userPromote = WHITE_BISHOP; break;
			case 'n': userPromote = WHITE_KNIGHT; break;
			}
		}
		else
			switch (userMove[4])
		{
			case 'q': userPromote = BLACK_QUEEN; break;
			case 'r': userPromote = BLACK_ROOK; break;
			case 'b': userPromote = BLACK_BISHOP; break;
			case 'n': userPromote = BLACK_KNIGHT; break;
		}
	}

	moveFound = false;
	for (i = board.moveBufLen[0]; i < board.moveBufLen[1]; i++)
	{
		if ((board.moveBuffer[i].getFrom() == userFrom) && (board.moveBuffer[i].getTosq() == userTo))
		{
			if (((board.square[userFrom] == WHITE_PAWN) && (RANKS[userFrom] == 7)) ||
				((board.square[userFrom] == BLACK_PAWN) && (RANKS[userFrom] == 2)))
			{
				if (board.moveBuffer[i].getProm() == userPromote)
				{
					move.moveInt = board.moveBuffer[i].moveInt;
					return true;
				}
			}
			else
			{
				move.moveInt = board.moveBuffer[i].moveInt;
				return true;
			}
		}
	}
	move.moveInt = 0;
	return false;
}
