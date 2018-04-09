/*
	蒙特卡洛搜索函数
*/

#ifndef MCTS_H_
#define MCTS_H_

extern int Total_Situation; // 总局面数

extern int Next_Situation[MAXSITUATION][MAXSIZE]; // Next_Situation[S][next] 为在 S 局面下放棋子于第 next 列后的局面编号

extern int Win_Times[MAXSITUATION][MAXSIZE]; // 每个分支局面最后走向胜利的次数

extern int Choose_Times[MAXSITUATION][MAXSIZE]; // 每个分支局面被探索的次数

extern int Total_Times[MAXSITUATION]; // 局面被探索的次数

void Situation_Init(); // 初始化

inline int BestAction(int nowSituation);

int MCTS(int now, int player);

int GetFinalAction();

void GetFinalData();

#endif