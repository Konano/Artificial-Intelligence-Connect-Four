/*
	蒙特卡洛搜索函数
*/

#ifndef MCTS_H_
#define MCTS_H_

#include "Argument.h"

extern int Total_Situation; // 总局面数

void Situation_Init(); // 初始化

inline int BestAction(const int nowSituation);

int MCTS(const int now, const int player);

int GetFinalAction();

void GetFinalData();

#endif