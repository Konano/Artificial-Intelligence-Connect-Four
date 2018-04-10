#include "Board.h"
#include "Function.h"

void BOARD::Init(const int _N, const int _M, const int _noX, const int _noY, const int* _top, const int* _board)
{
	clear();
	N = _N, M = _M, noX = _noX, noY = _noY;
	for(int i = 0; i < N; i++) top[i] = M;
	if (noY == M-1) top[noX] = M-1;
	for(int i = 0; i < N; i++) for(int j = M-1; j >= 0; j--) if (_board[j*N+i])
		PlaceChess(i, _board[j*N+i]);

	#ifdef ___DEBUG
	OutputBoard();
	#endif
}

inline void BOARD::clear()
{
	clr(boardX, 0);
	clr(boardY, 0);
	clr(boardVL, 0);
	clr(boardVR, 0);
}

void BOARD::PlaceChess(const int &X, const int &player)
{
	int Y = top[X] - 1;

	boardX[X] |= (1 << (Y*2+player));

	boardY[Y] |= (1 << (X*2+player));

	boardVL[X-Y+M] |= (1 << (X*2+player));

	boardVR[X+Y] |= (1 << (X*2+player));

	top[X]--;

	if (X == noX && top[X]-1 == noY) top[X]--;
}

void BOARD::RemoveChess(const int &X, const int &player)
{
	if (X == noX && top[X] == noY) top[X]++;

	int Y = top[X];

	boardX[X] -= (1 << (Y*2+player));

	boardY[Y] -= (1 << (X*2+player));

	boardVL[X-Y+M] -= (1 << (X*2+player));

	boardVR[X+Y] -= (1 << (X*2+player));

	top[X]++;
}

bool BOARD::GameOver(const int &X)
{
	int Y = top[X];
	if (X == noX && Y == noY) Y++;
	if (Y == M) return false;

	return (InALine(boardX[X]) || InALine(boardY[Y]) ||
			InALine(boardVL[X-Y+M]) || InALine(boardVR[X+Y]));
}

inline bool BOARD::InALine(const int &a)
{
	return (a & (a >> 2) & (a >> 4) & (a >> 6)) ? true : false;
}

int BOARD::GetTop(const int &X) { return top[X]; }

bool BOARD::full(const int &X) { return top[X] == 0; }

void BOARD::OutputBoard()
{
	for(int j = 0; j < M; j++)
	{
		for(int i = 0; i < N; i++) if (boardX[i] & (3 << (j*2+1)))
			_cprintf("%d", (boardX[i] & (1 << (j*2+1))) ? 1 : 2);
		else
			_cprintf("0");
		_cprintf("\n");
	}
	for(int i = 0; i < N; i++) _cprintf("%d ", top[i]);
	_cprintf("\n\n");
}