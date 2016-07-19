
class user_input
{
	int count = 0, number, i, j;
	char input_buf[255], next_char, userinput[80], sanMove[12];
	Move move, dummy;

public:
	void read_command(const char* gbuf);
	bool do_command(const char* buf);
};

void user_input::read_command(const char* gbuf)
{
	int k = 0;
	if (board.get_nextMove() == WHITE_MOVE)
	{
		cout << "white> ";
	}
	else
	{
		cout << "black> ";
	}
	cout << gbuf;
	do_command(gbuf);
	/*while ((next_char = getc(stdin)) != EOF)
	{
		if (next_char == '\n')
		{
			input_buf[count] = 0;
			while (count)
			{
				if (!do_command(input_buf)) return;
			}
			if (board.get_nextMove() == WHITE_MOVE)
			{
				cout << "white> ";
			}
			else
			{
				cout << "black> ";
			}

		}
		else
		{
			if (count >= 255)
			{
				cout << "what are you typing?? " << endl;
				count = 0;
			}
			input_buf[count++] = next_char;
		}
	}*/
}

bool user_input::do_command(const char *buf)
{
	//Return when buffer is empty
	if (!strcmp(buf, ""))
	{
		count = 0;
		return true;
	}

	//Help entries: h or help
	if ((!strcmp(buf, "help")) || (!strcmp(buf, "h")))
	{
		cout << endl << "help:" << endl;
		cout << "black               : BLACK to move" << endl;
		cout << "cc                  : play computer-to-computer " << endl;
		cout << "d                   : display board " << endl;
		cout << "exit                : exit program " << endl;
		cout << "eval                : show static evaluation of this position" << endl;
		cout << "go                  : computer next move " << endl;
		cout << "help		         : show this help " << endl;
		cout << "move e2e4, or h7h8q : enter a move (use this format)" << endl;
		cout << "moves               : show all legal moves" << endl;
		cout << "new                 : start new game" << endl;
		cout << "quit                : exit program " << endl;
		cout << "r                   : rotate board " << endl;
		cout << "sd n                : set the search depth to n" << endl;
		cout << "undo                : take back last move" << endl;
		cout << "white               : WHITE to move" << endl;
		cout << endl;
		count=0;
		return true;
	}


	//black to move
	if (!strcmp(buf, "black"))
	{
		if (board.get_nextMove() == WHITE_MOVE)
		{
			board.endOfSearch = 0;
			board.endOfGame = 0;
		}
		board.set_nextMove(BLACK_MOVE);
		count = 0;
		return true;
	}

	//play computer vs computer
	if (!strcmp(buf, "cc"))
	{
		cout << endl;

		while (/*!_kbhit() &&*/ !board.isEndOfgame(i, dummy))
		{
			move = board.think();
			if (move.moveInt)
			{
				makeMove(move);
//				display_blank_board();
//				populate_board();
			//	loadTexture();
			//	freeTexture();
				board.endOfGame++;
				board.endOfSearch = board.endOfGame;
				board.display();
				display_board();
				whose_turn_display();
				mySleep2(3);
			}
			else
			{
				count=0;
				return true;
			}
		}
		count=0;
		return true;
	}



	//display board
	if (!strcmp(buf, "d"))
	{
		board.display();
		count = 0;
		return true;
	}

	//exit the game
	if ((!strcmp(buf, "exit")) || (!strcmp(buf, "quit")))
	{
		count = 0;
		return false;
	}

	//game: show game moves
	if (!strcmp(buf, "game"))
	{
		if (board.endOfGame)
		{
			// make a temporary copy of board.gameLine[];
			number = board.endOfGame;
			move_tree *tmp = new move_tree[number];
			memcpy(tmp, board.move_tree_object, number * sizeof(move_tree));

			// unmake all moves:
			for (i = number - 1; i >= 0; i--)
			{
				unmakeMove(tmp[i].move);
				board.endOfSearch = --board.endOfGame;
			}

			// redo all moves:
			board.set_nextMove(j);
			for (i = 0; i < number; i++)
			{
				// move numbering:
				if (!((i + j + 2) % 2)) cout << (i + 2 * j + 2) / 2 << ". ";
				else if (!i) cout << "1. ... ";

				// construct the move string
				toSan(tmp[i].move, sanMove);
				cout << sanMove;

				// output space:
				if (!((i + j + 1) % 2)) cout << endl;
				else cout << " ";

				// make the move:
				makeMove(tmp[i].move);
				board.endOfSearch = ++board.endOfGame;
			}
			cout << endl;

			// delete the temporary copy:
			delete[] tmp;
		}
		else
		{
			cout << "there are no game moves" << endl;
		}

		count = 0;
		return true;
	}

	//go: computer's next move
	if (!strcmp(buf, "go"))
	{
		cout << endl;

		if (!board.isEndOfgame(i, dummy))
		{
			move = board.think();
			if (move.moveInt)
			{
				makeMove(move);
	
				board.endOfGame++;
				board.endOfSearch = board.endOfGame;
			}
			display_board();
			whose_turn_display();
			board.display();
			board.isEndOfgame(i, dummy);
			count = 0;
			return true;
		}
		else
		{

			board.display();
			count = 0;
			return true;
		}
	}

	//set the search depth
	if (!strncmp(buf, "sd", 2))
	{
		sscanf(buf + 2, "%d", &board.searchDepth);
		cout << "search depth " << board.searchDepth << endl;
		count = 0;
		return true;
	}

	//moves: show all the legal moves.
	if (!strcmp(buf, "moves"))
	{
		board.moveBufLen[0] = 0;
		board.moveBufLen[1] = movegen(board.moveBufLen[0]);
		cout << endl << "moves from this position:" << endl;
		number = 0;
		for (i = board.moveBufLen[0]; i < board.moveBufLen[1]; i++)
		{
			makeMove(board.moveBuffer[i]);
			if (isOtherKingAttacked())
			{
				unmakeMove(board.moveBuffer[i]);
			}
			else
			{
				unmakeMove(board.moveBuffer[i]);
				toSan(board.moveBuffer[i], sanMove);
				cout << ++number << ". " << sanMove << endl;
			}
		}
		count = 0;
		return true;
	}

	//move: make a move in the console
	if (!strncmp(buf, "move", 4))
	{
		sscanf(buf + 4, "%s", userinput);

		// generate the pseudo-legal move list
		board.moveBufLen[0] = 0;
		board.moveBufLen[1] = movegen(board.moveBufLen[0]);
	/*	int legalmoves;
		Move singlemove;*/
		if (!board.isEndOfgame(i, dummy))
		{
			if (isValidTextMove(userinput, move))        // check to see if the user move is also found in the pseudo-legal move list
			{
				makeMove(move);
				if (isOtherKingAttacked())              // post-move check to see if we are leaving our king in check
				{
					unmakeMove(move);
					cout << "    invalid move, leaving king in check: " << userinput << endl;
				}
				else
				{
					board.endOfGame++;
					board.endOfSearch = board.endOfGame;
					board.display();
					display_board();
					whose_turn_display();
					board.isEndOfgame(i, dummy);
				}
			}
			else
			{
				cout << "    move is invalid or not recognized: " << userinput << endl;

			}
		}

		/*else
		{
			cout << "    move is invalid or not recognized: " << userinput << endl;
		}*/
		count = 0;
		return true;
	}

	//new game
	if (!strcmp(buf, "new"))
	{
		board.endOfGame = 0;
		dataInit();
		board.init();
		board.display();
		display_board();
		whose_turn_display();
		count = 0;
		return true;
	}

	//rotate the board
	if (!strcmp(buf, "r"))
	{
		board.set_viewRotated(!(board.is_viewRotated()));
		board.display();
		count = 0;
		return true;
	}


	//undo the last move
	if (!strcmp(buf, "undo"))
	{
		if (board.endOfGame)
		{
			unmakeMove(board.move_tree_object[--board.endOfGame].move);
			board.endOfSearch = board.endOfGame;
			board.display();
			display_board();
		}
		else
		{
			cout << "\nAlready at start of game\n" << endl;
		}
		count = 0;
		return true;
	}

	//white to move
	if (!strcmp(buf, "white"))
	{
		if (board.get_nextMove() == BLACK_MOVE)
		{
			board.endOfSearch = 0;
			board.endOfGame = 0;
		}
		board.set_nextMove(WHITE_MOVE);
		count = 0;
		return true;
	}

	//do a static evaluation of the board
	if (!strcmp(buf, "eval"))
	{
		number = board.eval();
		cout << "eval score = " << number << endl;
		count = 0;
		return true;
	}

	//Exit the program
	if ((!strcmp(buf, "exit")) || (!strcmp(buf, "quit")))
	{
		count = 0;
		return false;
	}

	//Unknown command
	cout << endl << "Unknown Command!!" << endl;
	count = 0;
	return true;
}