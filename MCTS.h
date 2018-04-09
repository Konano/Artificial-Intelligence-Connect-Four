/*
	蒙特卡洛搜索函数
*/

#ifndef MCTS_H_
#define MCTS_H_

extern int nowSituation;


void Situation_Init(); // 初始化

inline int BestAction(int nowSituation);

int MCTS(int now, int player);

int GetFinalAction();

void GetFinalData();

#endif