#ifndef _CRT_SECURE_NO_DEPRECATE  // suppress MS security warnings
#define _CRT_SECURE_NO_DEPRECATE 1
#endif
#pragma warning (disable: 4244)
#pragma warning (disable: 4996)

#include<iostream>
#include<iomanip>
#include<conio.h>
#include<string>
#include<sys/timeb.h>
#include<glut.h>
#include<ctime>
#include<Windows.h>

using namespace std;

typedef unsigned long long U64;
typedef unsigned long long BitMap;

#define MAX_MOV_BUFF    4096   // Max number of moves that we can store (all plies)
#define MAX_PLY          64   // Max search depth
#define MAX_MOVES_DEPTH   1024   // Max number of moves in the (game + search) line that we can store

//BIT NUMBERING CONVENTION FOR EACH SQUARE ON THE BOARD
const int A8 = 56, B8 = 57, C8 = 58, D8 = 59, E8 = 60, F8 = 61, G8 = 62, H8 = 63;
const int A7 = 48, B7 = 49, C7 = 50, D7 = 51, E7 = 52, F7 = 53, G7 = 54, H7 = 55;
const int A6 = 40, B6 = 41, C6 = 42, D6 = 43, E6 = 44, F6 = 45, G6 = 46, H6 = 47;
const int A5 = 32, B5 = 33, C5 = 34, D5 = 35, E5 = 36, F5 = 37, G5 = 38, H5 = 39;
const int A4 = 24, B4 = 25, C4 = 26, D4 = 27, E4 = 28, F4 = 29, G4 = 30, H4 = 31;
const int A3 = 16, B3 = 17, C3 = 18, D3 = 19, E3 = 20, F3 = 21, G3 = 22, H3 = 23;
const int A2 = 8, B2 = 9, C2 = 10, D2 = 11, E2 = 12, F2 = 13, G2 = 14, H2 = 15;
const int A1 = 0, B1 = 1, C1 = 2, D1 = 3, E1 = 4, F1 = 5, G1 = 6, H1 = 7;

const char* SQUARENAME[64] = {
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

const int FILES[64] = {
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8
};

const int RANKS[64] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8
};


//Identifier for whose move is next
const int WHITE_MOVE = 0;
const int BLACK_MOVE = 1;

//4-bit place identifiers
//white = 0..., black = 1..., sliding = .1.., nonsliding = .0..
// rank/file sliding pieces = .11., diagonally sliding pieces = .1.1
// pawns and kings (without color bits), are < 3
// major pieces (without color bits set), are > 5
// minor and major pieces (without color bits set), are > 2
const int EMPTY = 0;                //  0000
const int WHITE_PAWN = 1;           //  0001
const int WHITE_KING = 2;           //  0010
const int WHITE_KNIGHT = 3;         //  0011
const int WHITE_BISHOP = 5;         //  0101
const int WHITE_ROOK = 6;           //  0110
const int WHITE_QUEEN = 7;          //  0111
const int BLACK_PAWN = 9;           //  1001
const int BLACK_KING = 10;          //  1010
const int BLACK_KNIGHT = 11;        //  1011
const int BLACK_BISHOP = 13;        //  1101
const int BLACK_ROOK = 14;          //  1110
const int BLACK_QUEEN = 15;         //  1111

const char* PIECENAMES[16] =
{
	"  ", "P ", "K ", "N ", "  ", "B ", "R ", "Q ",
	"  ", "P*", "K*", "N*", "  ", "B*", "R*", "Q*"
};

const char* PIECECHARS[16] = 
{ 
	" ", " ", "K", "N", " ", "B", "R", "Q",
	" ", " ", "K", "N", " ", "B", "R", "Q"
};

BitMap BITSET[64];
unsigned int BOARDINDEX[9][9];

// Value of pieces, in centipawns:
const unsigned int PAWN_VALUE = 100;
const unsigned int KNIGHT_VALUE = 300;
const unsigned int BISHOP_VALUE = 300;
const unsigned int ROOK_VALUE = 500;
const unsigned int QUEEN_VALUE = 900;
const unsigned int KING_VALUE = 9999;
const unsigned int CHECK_MATE = KING_VALUE;

// used in Eugene Nalimov's bitScanReverse
//unsigned int MS1BTABLE[256];

// Attack tables:
BitMap WHITE_PAWN_ATTACKS[64];
BitMap WHITE_PAWN_MOVES[64];
BitMap WHITE_PAWN_DOUBLE_MOVES[64];
BitMap BLACK_PAWN_ATTACKS[64];
BitMap BLACK_PAWN_MOVES[64];
BitMap BLACK_PAWN_DOUBLE_MOVES[64];
BitMap KNIGHT_ATTACKS[64];
BitMap KING_ATTACKS[64];
BitMap RANK_ATTACKS[64][64];      // 32KB
BitMap FILE_ATTACKS[64][64];      // 32KB
BitMap DIAGA8H1_ATTACKS[64][64];  // 32KB
BitMap DIAGA1H8_ATTACKS[64][64];  // 32KB

// Move generator shift for ranks:
const int RANKSHIFT[64] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	9, 9, 9, 9, 9, 9, 9, 9,
	17, 17, 17, 17, 17, 17, 17, 17,
	25, 25, 25, 25, 25, 25, 25, 25,
	33, 33, 33, 33, 33, 33, 33, 33,
	41, 41, 41, 41, 41, 41, 41, 41,
	49, 49, 49, 49, 49, 49, 49, 49,
	57, 57, 57, 57, 57, 57, 57, 57
};

// Move generator magic multiplication numbers for files:
const BitMap _FILEMAGICS[8] = {
	0x8040201008040200,
	0x4020100804020100,
	0x2010080402010080,
	0x1008040201008040,
	0x0804020100804020,
	0x0402010080402010,
	0x0201008040201008,
	0x0100804020100804
};

// Move generator magic multiplication numbers for diagonals:
const BitMap _DIAGA8H1MAGICS[15] = {
	0x0,
	0x0,
	0x0101010101010100,
	0x0101010101010100,
	0x0101010101010100,
	0x0101010101010100,
	0x0101010101010100,
	0x0101010101010100,
	0x0080808080808080,
	0x0040404040404040,
	0x0020202020202020,
	0x0010101010101010,
	0x0008080808080808,
	0x0,
	0x0
};

// Move generator magic multiplication numbers for diagonals:
const BitMap _DIAGA1H8MAGICS[15] = {
	0x0,
	0x0,
	0x0101010101010100,
	0x0101010101010100,
	0x0101010101010100,
	0x0101010101010100,
	0x0101010101010100,
	0x0101010101010100,
	0x8080808080808000,
	0x4040404040400000,
	0x2020202020000000,
	0x1010101000000000,
	0x0808080000000000,
	0x0,
	0x0
};

// Move generator 6-bit masking and magic multiplication numbers:
BitMap RANKMASK[64];
BitMap FILEMASK[64];
BitMap FILEMAGIC[64];
BitMap DIAGA8H1MASK[64];
BitMap DIAGA8H1MAGIC[64];
BitMap DIAGA1H8MASK[64];
BitMap DIAGA1H8MAGIC[64];

//generalized sliding attacks array: [8 squares][64 states]
// the unsigned int (=8 bits) contains the attacks for a rank, file or diagonal
unsigned int GEN_SLIDING_ATTACKS[8][64];

// Used for castling:
unsigned int CANCASTLEOO = 1;
unsigned int CANCASTLEOOO = 2;
unsigned int WHITE_OOO_CASTL;
unsigned int BLACK_OOO_CASTL;
unsigned int WHITE_OO_CASTL;
unsigned int BLACK_OO_CASTL;

BitMap maskEG[2];
BitMap maskFG[2];
BitMap maskBD[2];
BitMap maskCE[2];

int ICAPT;
int IEP;
int IPROM;
int ICASTLOO;
int ICASTLOOO;
int ICHECK;

/*
Evaluation scores start here, all scores are in centipawns.
If there are scoring data that is supposed to be symmetrical (i.e. same for BLACK & WHITE),
then only the data for WHITE is supplied, and data for BLACK is calculated in dataInit().

MIRRORED:
Some scoring arrays are supplied MIRRORED, i.e. starting with the last rank.
They are mirrored back in the right order in dataInit().
*/

int PENALTY_DOUBLED_PAWN = 10;
int PENALTY_ISOLATED_PAWN = 20;
int PENALTY_BACKWARD_PAWN = 8;
int BONUS_PASSED_PAWN = 20;
int BONUS_BISHOP_PAIR = 10;
int BONUS_ROOK_BEHIND_PASSED_PAWN = 20;
int BONUS_ROOK_ON_OPEN_FILE = 20;
int BONUS_TWO_ROOKS_ON_OPEN_FILE = 20;

int BONUS_PAWN_SHIELD_STRONG = 9;
int BONUS_PAWN_SHIELD_WEAK = 4;

int PAWN_OWN_DISTANCE[8] = { 0, 8, 4, 2, 0, 0, 0, 0 };
int PAWN_OPPONENT_DISTANCE[8] = { 0, 2, 1, 0, 0, 0, 0, 0 };
int KNIGHT_DISTANCE[8] = { 0, 4, 4, 0, 0, 0, 0, 0 };
int BISHOP_DISTANCE[8] = { 0, 5, 4, 3, 2, 1, 0, 0 };
int ROOK_DISTANCE[8] = { 0, 7, 5, 4, 3, 0, 0, 0 };
int QUEEN_DISTANCE[8] = { 0, 10, 8, 5, 4, 0, 0, 0 };

//Mirrored Array
int PAWNPOS_W[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	5, 10, 15, 20, 20, 15, 10, 5,
	4, 8, 12, 16, 16, 12, 8, 4,
	3, 6, 9, 12, 12, 9, 6, 3,
	2, 4, 6, 8, 8, 6, 4, 2,
	1, 2, 3, -10, -10, 3, 2, 1,
	0, 0, 0, -40, -40, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

//Mirrored Array
int KNIGHTPOS_W[64] = {
	-10, -10, -10, -10, -10, -10, -10, -10,
	-10, 0, 0, 0, 0, 0, 0, -10,
	-10, 0, 5, 5, 5, 5, 0, -10,
	-10, 0, 5, 10, 10, 5, 0, -10,
	-10, 0, 5, 10, 10, 5, 0, -10,
	-10, 0, 5, 5, 5, 5, 0, -10,
	-10, 0, 0, 0, 0, 0, 0, -10,
	-10, -30, -10, -10, -10, -10, -30, -10
};

//Mirrored Array
int BISHOPPOS_W[64] = {
	-10, -10, -10, -10, -10, -10, -10, -10,
	-10, 0, 0, 0, 0, 0, 0, -10,
	-10, 0, 5, 5, 5, 5, 0, -10,
	-10, 0, 5, 10, 10, 5, 0, -10,
	-10, 0, 5, 10, 10, 5, 0, -10,
	-10, 0, 5, 5, 5, 5, 0, -10,
	-10, 0, 0, 0, 0, 0, 0, -10,
	-10, -10, -20, -10, -10, -20, -10, -10
};

//Mirrored Array
int ROOKPOS_W[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	15, 15, 15, 15, 15, 15, 15, 15,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	-10, 0, 0, 10, 10, 0, 0, -10
};

//Mirrored Array
int QUEENPOS_W[64] = {
	-10, -10, -10, -10, -10, -10, -10, -10,
	-10, 0, 0, 0, 0, 0, 0, -10,
	-10, 0, 5, 5, 5, 5, 0, -10,
	-10, 0, 5, 10, 10, 5, 0, -10,
	-10, 0, 5, 10, 10, 5, 0, -10,
	-10, 0, 5, 5, 5, 5, 0, -10,
	-10, 0, 0, 0, 0, 0, 0, -10,
	-10, -10, -20, -10, -10, -20, -10, -10
};

//Mirorred Array
int KINGPOS_W[64] = {
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-20, -20, -20, -20, -20, -20, -20, -20,
	0, 20, 40, -20, 0, -20, 40, 20
};

//Mirrored Array
int KINGPOS_ENDGAME_W[64] = {
	0, 10, 20, 30, 30, 20, 10, 0,
	10, 20, 30, 40, 40, 30, 20, 10,
	20, 30, 40, 50, 50, 40, 30, 20,
	30, 40, 50, 60, 60, 50, 40, 30,
	30, 40, 50, 60, 60, 50, 40, 30,
	20, 30, 40, 50, 50, 40, 30, 20,
	10, 20, 30, 40, 40, 30, 20, 10,
	0, 10, 20, 30, 30, 20, 10, 0
};

int MIRROR[64] = {
	56, 57, 58, 59, 60, 61, 62, 63,
	48, 49, 50, 51, 52, 53, 54, 55,
	40, 41, 42, 43, 44, 45, 46, 47,
	32, 33, 34, 35, 36, 37, 38, 39,
	24, 25, 26, 27, 28, 29, 30, 31,
	16, 17, 18, 19, 20, 21, 22, 23,
	8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7
};

int DISTANCE[64][64];
int PAWNPOS_B[64];
int KNIGHTPOS_B[64];
int BISHOPPOS_B[64];
int ROOKPOS_B[64];
int QUEENPOS_B[64];
int KINGPOS_B[64];
int KINGPOS_ENDGAME_B[64];
BitMap PASSED_WHITE[64];
BitMap PASSED_BLACK[64];
BitMap ISOLATED_WHITE[64];
BitMap ISOLATED_BLACK[64];
BitMap BACKWARD_WHITE[64];
BitMap BACKWARD_BLACK[64];
BitMap KINGSHIELD_STRONG_W[64];
BitMap KINGSHIELD_STRONG_B[64];
BitMap KINGSHIELD_WEAK_W[64];
BitMap KINGSHIELD_WEAK_B[64];
BitMap WHITE_SQUARES;
BitMap BLACK_SQUARES;

// Search parameters:
int LARGE_NUMBER = KING_VALUE + 1;
int CHECKMATESCORE = KING_VALUE;
int STALEMATESCORE = 0;
int DRAWSCORE = 0;


//GUI STUFF

#define maxx 9
#define maxy 9
#define dx 50
#define dy 50
int x[maxx] = { 0 }, y[maxy] = { 0 };
char buf[2][2];
float x0 = 50, yk = 50;
int i, j;
int gw, gh;
int flag = 0;
char gui_ip[255];
bool onep = false;
bool main_menu = true;
bool about_menu = false;
bool game = false;
