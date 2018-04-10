/*
	���ؿ�����������
*/

#ifndef MCTS_H_
#define MCTS_H_

#include "Argument.h"

extern int Total_Situation; // �ܾ�����

void Situation_Init(); // ��ʼ��

inline int BestAction(const int nowSituation);

int MCTS(const int now, const int player);

int GetFinalAction();

void GetFinalData();

#endif