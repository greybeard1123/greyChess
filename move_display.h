void displayMove(Move &move)
{
	// displays a single move on the console

	if (((move.getPiec() == WHITE_KING) && (move.isCastleOO())) || ((move.getPiec() == BLACK_KING) && (move.isCastleOO())))
	{
		cout << "O-O";
		return;
	};
	if (((move.getPiec() == WHITE_KING) && (move.isCastleOOO())) || ((move.getPiec() == BLACK_KING) && (move.isCastleOOO())))
	{
		cout << "O-O-O";
		return;
	};

	if (!move.isPawnmove()) cout << PIECECHARS[move.getPiec()];
	if (move.isPawnmove() && move.isCapture()) cout << char('a' + FILES[move.getFrom()] - 1);
	if (move.isCapture()) cout << "x";
	cout << char('a' + FILES[move.getTosq()] - 1);
	cout << RANKS[move.getTosq()];
	if (move.isPromotion())
	{
		cout << "=";
		cout << PIECECHARS[move.getProm()];
	}
	cout.flush();
	return;
}

void displayPV()
{
	int i;
	char sanMove[12];

	for (i = 0; i < board.triangularLength[0]; i++)
	{
		toSan(board.triangularArray[0][i], sanMove);
		cout << sanMove << " ";
		makeMove(board.triangularArray[0][i]);
	}
	for (i = board.triangularLength[0] - 1; i >= 0; i--)
	{
		unmakeMove(board.triangularArray[0][i]);
	}
	if (i < 3) cout << "     ";   // make sure to overwrite any remaining output of mode 3
	cout << endl;
	cout.flush();
}

bool toSan(Move &move, char *sanMove)
{

	// toSan will convert a move into non-ambiguous SAN-notation, returned in char sanMove[].
	// Returns true if successful.
	
	unsigned int i, j, k, ibuf, from, to, piece, capt, prom, ambigfile, ambigrank;
	int asciiShift;
	bool legal, check, mate, ambig;

	asciiShift = (int)'a';
	piece = move.getPiec();
	from = move.getFrom();
	to = move.getTosq();
	capt = move.getCapt();
	prom = move.getProm();
	ibuf = 0;
	ambig = false;
	ambigfile = 0;
	ambigrank = 0;
	legal = false;
	check = false;
	mate = false;
	sprintf(sanMove, "");

	//     Generate all pseudo-legal moves to be able to remove any ambiguities
	//     and check legality. Take the next free location in moveBufLen:
	while (board.moveBufLen[ibuf + 1]) ibuf++;
	board.moveBufLen[ibuf + 1] = movegen(board.moveBufLen[ibuf]);

	//     Loop over the moves to see what kind(s) of ambiguities exist, if any:
	for (i = board.moveBufLen[ibuf]; i < board.moveBufLen[ibuf + 1]; i++)
	{
		makeMove(board.moveBuffer[i]);
		if (!isOtherKingAttacked())
		{
			if (board.moveBuffer[i].moveInt == move.moveInt)
			{
				legal = true;
				// it is check:
				if (isOwnKingAttacked())
				{
					check = true;
					// is it checkmate?
					k = 0;
					board.moveBufLen[ibuf + 2] = movegen(board.moveBufLen[ibuf + 1]);
					for (j = board.moveBufLen[ibuf + 1]; j < board.moveBufLen[ibuf + 2]; j++)
					{
						makeMove(board.moveBuffer[j]);
						if (!isOtherKingAttacked()) k++;
						unmakeMove(board.moveBuffer[j]);
					}
					if (!k) mate = true;
				}
			}
			// two same pieces can move to the same square:
			if ((board.moveBuffer[i].moveInt != move.moveInt) && (board.moveBuffer[i].getPiec() == piece) && (board.moveBuffer[i].getTosq() == to))
			{
				ambig = true;
				if (FILES[from] == FILES[board.moveBuffer[i].getFrom()]) ambigfile++;
				if (RANKS[from] == RANKS[board.moveBuffer[i].getFrom()]) ambigrank++;
			}
		}
		unmakeMove(board.moveBuffer[i]);
	}
	//  cleanup:
	board.moveBufLen[ibuf + 1] = 0;
	board.moveBufLen[ibuf + 2] = 0;

	//     construct the SAN string:
	if (!legal)
	{
		strcpy(sanMove, "unknown");
		return false;
	}
	else
	{
		// start building the string
		if (!move.isPawnmove())
		{
			sprintf(sanMove, "%s", PIECECHARS[piece]);
			if (ambig)
			{
				if (ambigfile)
				{
					if (ambigrank) sprintf(sanMove, "%s%c%d", sanMove, FILES[from] + asciiShift - 1, RANKS[from]);
					else sprintf(sanMove, "%s%d", sanMove, RANKS[from]);
				}
				else
				{
					sprintf(sanMove, "%s%c", sanMove, FILES[from] + asciiShift - 1);
				}
			}
		}
		else
		{
			if (move.isCapture())
			{
				sprintf(sanMove, "%s%c", sanMove, FILES[from] + asciiShift - 1);
			}
		}
		if (move.isCapture()) sprintf(sanMove, "%sx", sanMove);
		sprintf(sanMove, "%s%c%d", sanMove, FILES[to] + asciiShift - 1, RANKS[to]);
		if (move.isEnpassant()) sprintf(sanMove, "%s e.p.", sanMove);
		if (move.isPromotion()) sprintf(sanMove, "%s=%s", sanMove, PIECECHARS[prom]);
		if (check)
		{
			if (mate) sprintf(sanMove, "%s#", sanMove);
			else sprintf(sanMove, "%s+", sanMove);
		}
		return true;
	}
}