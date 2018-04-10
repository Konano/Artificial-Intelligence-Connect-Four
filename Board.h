/*
	有关棋盘方面的函数
*/

#ifndef CHESS_H_
#define	CHESS_H_

#include "Argument.h"

class BOARD
{
	int N, M, noX, noY;
	int top[MAXSIZE];
	int boardX[3][MAXSIZE];
	int boardY[3][MAXSIZE];
	int boardVL[3][MAXSIZE*3];
	int boardVR[3][MAXSIZE*3];
	inline void clear();
	inline bool InALine(const int &a);
public:
	void Init(const int _N, const int _M, const int _noX, const int _noY, const int* _top, const int* _board);
	void PlaceChess(const int &X, const int &player);
	void RemoveChess(const int &X, const int &player);
	bool GameOver(const int X, const int player);
	int GetTop(const int &X);
	bool full(const int &X);
	void OutputBoard();
};

#endif