#include "Strategy.h"
#include "Judge.h"


// 检查双方是否有制胜位

int WIN[3]; // 记录制胜位的信息

void WinPlace()
{
	for(int player = 1; player <= 2; player++)
	{
		WIN[player] = 0;
		for(int i = 0; i < N; i++) if (!board.full(i))
		{
			board.PlaceChess(i, player);
			if (board.GameOver(i)) WIN[player] = WIN[player] * (N+1) + i + 1;
			board.RemoveChess(i, player);
		}
	}
}
