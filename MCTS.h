/*
	���ؿ�����������
*/

#ifndef MCTS_H_
#define MCTS_H_

extern int Total_Situation; // �ܾ�����

extern int Next_Situation[MAXSITUATION][MAXSIZE]; // Next_Situation[S][next] Ϊ�� S �����·������ڵ� next �к�ľ�����

extern int Win_Times[MAXSITUATION][MAXSIZE]; // ÿ����֧�����������ʤ���Ĵ���

extern int Choose_Times[MAXSITUATION][MAXSIZE]; // ÿ����֧���汻̽���Ĵ���

extern int Total_Times[MAXSITUATION]; // ���汻̽���Ĵ���

void Situation_Init(); // ��ʼ��

inline int BestAction(int nowSituation);

int MCTS(int now, int player);

int GetFinalAction();

void GetFinalData();

#endif