/*
	用于棋局评判的辅助函数
*/

#ifndef JUDGE_H_
#define JUDGE_H_

extern int WIN[3]; // 记录制胜位的信息


inline bool CheckBoard(int x, int y, int player);

void WinPlace();

bool GameOver();

#endif