/*
	策略接口文件
*/

#ifndef STRATEGY_H_
#define	STRATEGY_H_

#include "Argument.h"
#include "Function.h"
#include "Point.h"

extern int N, M, top[MAXSIZE], board[MAXSIZE][MAXSIZE], noX, noY, lastX, lastY;


extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board,
	const int lastX, const int lastY, const int noX, const int noY);

extern "C" __declspec(dllexport) void clearPoint(Point* p);

#endif