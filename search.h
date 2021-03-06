Move Board::think()
{
	// Search entry point
	// The search stops if (whatever comes first):
	// - there is no legal move (checkmate or stalemate)
	// - there is only one legal move (in this case we don't need to search)
	// - time is up
	// - the search is interrupted by the user, or by winboard
	// - the search depth is reached
	int score, legalmoves;
	Move singlemove;

	//  Check if the game has ended, or if there is only one legal move,
	//  because then there's no need to search:
	if (isEndOfgame(legalmoves, singlemove)) return NOMOVE;
	if (legalmoves == 1)
	{
		cout << "forced move: "; displayMove(singlemove); cout << endl;
		return singlemove;
	}

	//     ===========================================================================
	//     There is more than legal 1 move, so prepare to search:
	//     ===========================================================================
	timer.init();
	displaySearchStats(1, 0, 0);  // display console header

	moveBufLen[0] = 0;
	inodes = 0;
	msStart = timer.getms();

	//    score = minimax(0, searchDepth);
	//score = alphabeta(0, searchDepth, -LARGE_NUMBER, LARGE_NUMBER);

   score = alphabetapvs(0, searchDepth, -LARGE_NUMBER, LARGE_NUMBER);
	msStop = timer.getms();
	displaySearchStats(0, searchDepth, score);
	return (triangularArray[0][0]);
}

int Board::alphabetapvs(int ply, int depth, int alpha, int beta)
{
	// PV search

	int i, j, movesfound, pvmovesfound, val;
	triangularLength[ply] = ply;
	if (depth == 0) return board.eval();
	movesfound = 0;
	pvmovesfound = 0;
	moveBufLen[ply + 1] = movegen(moveBufLen[ply]);
	for (i = moveBufLen[ply]; i < moveBufLen[ply + 1]; i++)
	{
		makeMove(moveBuffer[i]);
		if (!isOtherKingAttacked())
		{
			inodes++;
			movesfound++;
			if (!ply) displaySearchStats(3, ply, i);
			if (pvmovesfound)
			{
				val = -alphabetapvs(ply + 1, depth - 1, -alpha - 1, -alpha);
				if ((val > alpha) && (val < beta))
				{
					val = -alphabetapvs(ply + 1, depth - 1, -beta, -alpha);   // In case of failure, proceed with normal alphabeta                
				}
			}
			else val = -alphabetapvs(ply + 1, depth - 1, -beta, -alpha);          // Normal alphabeta
			unmakeMove(moveBuffer[i]);
			if (val >= beta)
			{
				return beta;
			}
			if (val > alpha)
			{
				alpha = val;                                                      // both sides want to maximize from *their* perspective
				pvmovesfound++;
				triangularArray[ply][ply] = moveBuffer[i];                                  // save this move
				for (j = ply + 1; j < triangularLength[ply + 1]; j++)
				{
					triangularArray[ply][j] = triangularArray[ply + 1][j];   // and append the latest best PV from deeper plies
				}
				triangularLength[ply] = triangularLength[ply + 1];
				if (!ply)
				{
					msStop = timer.getms();
					displaySearchStats(2, depth, val);
				}
			}
		}
		else unmakeMove(moveBuffer[i]);
	}

	//     50-move rule:
	if (fiftyMove >= 100) return DRAWSCORE;

	//     Checkmate/stalemate detection:
	if (!movesfound)
	{
		if (isOwnKingAttacked())  return (-CHECKMATESCORE + ply - 1);
		else  return (STALEMATESCORE);
	}

	return alpha;
}

int Board::alphabeta(int ply, int depth, int alpha, int beta)
{
	// Negascout

	int i, j, val;

	triangularLength[ply] = ply;
	if (depth == 0) return board.eval();

	moveBufLen[ply + 1] = movegen(moveBufLen[ply]);
	for (i = moveBufLen[ply]; i < moveBufLen[ply + 1]; i++)
	{
		makeMove(moveBuffer[i]);
		{
			if (!isOtherKingAttacked())
			{
				inodes++;
				if (!ply) displaySearchStats(3, ply, i);
				val = -alphabeta(ply + 1, depth - 1, -beta, -alpha);
				unmakeMove(moveBuffer[i]);
				if (val >= beta)
				{
					return beta;
				}
				if (val > alpha)
				{
					alpha = val;                                                                              // both sides want to maximize from *their* perspective
					triangularArray[ply][ply] = moveBuffer[i];                                  // save this move
					for (j = ply + 1; j < triangularLength[ply + 1]; j++)
					{
						triangularArray[ply][j] = triangularArray[ply + 1][j];   // and append the latest best PV from deeper plies
					}
					triangularLength[ply] = triangularLength[ply + 1];
					if (!ply)
					{
						msStop = timer.getms();
						displaySearchStats(2, depth, val);
					}
				}
			}
			else unmakeMove(moveBuffer[i]);
		}
	}
	return alpha;
}

int Board::minimax(int ply, int depth)
{
	// Negamax

	unsigned int i, j, val, best;
	best = -LARGE_NUMBER;
	triangularLength[ply] = ply;
	if (depth == 0) return board.eval();
	moveBufLen[ply + 1] = movegen(moveBufLen[ply]);
	for (i = moveBufLen[ply]; i < moveBufLen[ply + 1]; i++)
	{
		makeMove(moveBuffer[i]);
		{
			if (!isOtherKingAttacked())
			{
				inodes++;
				if (!ply) displaySearchStats(3, ply, i);
				val = -minimax(ply + 1, depth - 1);                                 // note the minus sign
				unmakeMove(moveBuffer[i]);
				if (val > best)                                                 // both sides want to maximize from *their* perspective
				{
					best = val;
					triangularArray[ply][ply] = moveBuffer[i];                                  // save this move
					for (j = ply + 1; j < triangularLength[ply + 1]; j++)
					{
						triangularArray[ply][j] = triangularArray[ply + 1][j];   // and append the latest best PV from deeper plies
					}
					triangularLength[ply] = triangularLength[ply + 1];
					if (!ply)
					{
						msStop = timer.getms();
						displaySearchStats(2, depth, val);
					}
				}
			}
			else unmakeMove(moveBuffer[i]);
		}
	}
	return best;
}


void Board::displaySearchStats(int mode, int depth, int score)
{
	// displays various search statistics
	// only to be used when ply = 0
	// mode = 1 : display header
	// mode = 2 : display full stats, including score and latest PV
	// mode = 3 : display current root move that is being searched
	//                     depth = ply, score = loop counter in the search move list

	char sanMove[12];
	U64 dt, hh, mm, ss;

	switch (mode)
	{
	case 1:
		cout << "depth  score   nodes     time  knps PV" << endl;
		break;

	case 2:
		dt = msStop - msStart;

		// depth
		printf("%5d ", depth);

		// score
		printf("%+6.2f ", float(score / 100.0));

		// nodes searched
		if (inodes > 100000000) printf("%6.0f%c ", float(inodes / 1000000.0), 'M');
		else if (inodes > 10000000) printf("%6.2f%c ", float(inodes / 1000000.0), 'M');
		else if (inodes > 1000000) printf("%6.0f%c ", float(inodes / 1000.0), 'K');
		else if (inodes > 100000)  printf("%6.1f%c ", float(inodes / 1000.0), 'K');
		else if (inodes > 10000)   printf("%6.2f%c ", float(inodes / 1000.0), 'K');
		else                                     printf("%7d ", inodes);

		// search time
		if (dt > 3600000)
		{
			hh = dt / 3600000;
			mm = (dt - 3600000 * hh) / 60000;
			ss = (dt - 3600000 * hh - 60000 * mm) / 1000;
			printf("%02d%c", hh, ':');
			printf("%02d%c", mm, ':');
			printf("%02d ", ss);
		}
		else if (dt > 60000)
		{
			mm = dt / 60000;
			ss = (dt - 60000 * mm) / 1000;
			printf("   %02d%c", mm, ':');
			printf("%02d ", ss);
		}
		else if (dt > 10000)       printf(" %6.1f%c ", float(dt / 1000.0), 's');
		else if (dt > 1000)               printf(" %6.2f%c ", float(dt / 1000.0), 's');
		else if (dt > 0)                  printf(" %5dms ", dt);
		else                                     printf("     0ms ");

		// search speed
		if (dt > 0) cout << setw(5) << (inodes / dt) << " ";
		else          cout << "    - ";

		// PV
		displayPV();
		break;

	case 3: //numbers refer to pseudo-legal moves:
		printf("%12s (%2d/%2d)%16s", " ", score + 1, moveBufLen[depth + 1] - moveBufLen[depth], " ");
		unmakeMove(moveBuffer[score]);
		toSan(moveBuffer[score], sanMove);
		cout << sanMove;
		makeMove(moveBuffer[score]);
		printf("...    \r");
		cout.flush();
		break;

	default: break;
	}

	return;
}

bool Board::isEndOfgame(int &legalmoves, Move &singlemove)
{
	// Checks if the current position is end-of-game due to:
	// checkmate, stalemate, 50-move rule, or insufficient material

	int whiteknights, whitebishops, whiterooks, whitequeens, whitetotalmat;
	int blackknights, blackbishops, blackrooks, blackqueens, blacktotalmat;

	// checkmating the other side?
	unsigned int i;
	if (isOtherKingAttacked())
	{
		if (nextMove)
		{
			cout << "0-1 {White mates}" << endl;
//			side_display_clear();
			draw_button(475, 586, 10, 220, 420);

			drawstringg(500, 300, "0-1");
			drawstringg(500, 270, "Black");
			drawstringg(500, 240, "Wins");
		}
		else
		{
			cout << "1-0 {Black mates}" << endl;
	//		side_display_clear();
			draw_button(475, 586, 11, 220, 420);

			drawstringg(500, 300, "1-0");
			drawstringg(500, 270, "White");
			drawstringg(500, 240, "Wins");
			
		}
		return true;
	}

	// how many legal moves?
	legalmoves = 0;
	moveBufLen[0] = 0;
	moveBufLen[1] = movegen(moveBufLen[0]);
	for (i = moveBufLen[0]; i < moveBufLen[1]; i++)
	{
		makeMove(moveBuffer[i]);
		if (!isOtherKingAttacked())
		{
			legalmoves++;
			singlemove = moveBuffer[i];
		}
		unmakeMove(moveBuffer[i]);
	}

	// checkmate or stalemate?
	if (!legalmoves)
	{
		if (isOwnKingAttacked())
		{
			if (nextMove)
			{
				cout << "1-0 {Black mates}" << endl;
//				side_display_clear();
				draw_button(475, 586, 11, 220, 420);

				drawstringg(500, 300, "1-0");
				drawstringg(500, 270, "White");
				drawstringg(500, 240, "Wins");
				
			}
			else
			{
				cout << "0-1 {White mates}" << endl;
//				side_display_clear();
				draw_button(475, 586, 10, 220, 420);

				drawstringg(500, 300, "0-1");
				drawstringg(500, 270, "Black");
				drawstringg(500, 240, "Wins");
			}
		}
		else
		{
			cout << "1/2-1/2 {stalemate}" << endl;
//			side_display_clear();
			draw_button(475, 586, 12, 220, 420);
			drawstringg(500, 300, "1/2 - 1/2");
			drawstringg(500, 270, "Stalemate");
			//drawstring(500, 240, "Mates");
		}
		return true;
	}

	// draw due to insufficient material:
	if (!whitePawns && !blackPawns)
	{
		whiteknights = bitCnt(whiteKnights);
		whitebishops = bitCnt(whiteBishops);
		whiterooks = bitCnt(whiteRooks);
		whitequeens = bitCnt(whiteQueens);
		whitetotalmat = 3 * whiteknights + 3 * whitebishops + 5 * whiterooks + 10 * whitequeens;
		blackknights = bitCnt(blackKnights);
		blackbishops = bitCnt(blackBishops);
		blackrooks = bitCnt(blackRooks);
		blackqueens = bitCnt(blackQueens);
		blacktotalmat = 3 * blackknights + 3 * blackbishops + 5 * blackrooks + 10 * blackqueens;

		// king versus king:
		if ((whitetotalmat == 0) && (blacktotalmat == 0))
		{
			draw_button(475, 586, 12, 220, 420);
			cout << "1/2-1/2 {material}" << endl;
			return true;
		}

		// king and knight versus king:
		if (((whitetotalmat == 3) && (whiteknights == 1) && (blacktotalmat == 0)) ||
			((blacktotalmat == 3) && (blackknights == 1) && (whitetotalmat == 0)))
		{
			draw_button(475, 586, 12, 220, 420);
			cout << "1/2-1/2 {material}" << endl;
			return true;
		}

		// 2 kings with one or more bishops, all bishops on the same colour:
		if ((whitebishops + blackbishops) > 0)
		{
			if ((whiteknights == 0) && (whiterooks == 0) && (whitequeens == 0) &&
				(blackknights == 0) && (blackrooks == 0) && (blackqueens == 0))
			{
				if (!((whiteBishops | blackBishops) & WHITE_SQUARES) ||
					!((whiteBishops | blackBishops) & BLACK_SQUARES))
				{
					draw_button(475, 586, 12, 220, 420);
					cout << "1/2-1/2 {material}" << endl;
					return true;
				}
			}
		}
	}

	if (fiftyMove >= 100)
	{
		draw_button(475, 586, 12, 220, 420);
		cout << "1/2-1/2 {50-move rule}" << endl;
		return true;
	}

	return false;
}