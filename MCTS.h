/*
	���ؿ�����������
*/

#ifndef MCTS_H_
#define MCTS_H_

extern int nowSituation;


void Situation_Init(); // ��ʼ��

inline int BestAction(int nowSituation);

int MCTS(int now, int player);

int GetFinalAction();

void GetFinalData();

#endif