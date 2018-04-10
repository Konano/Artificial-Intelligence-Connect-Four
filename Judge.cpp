#include "Strategy.h"
#include "Judge.h"


// ���˫���Ƿ�����ʤλ

int WIN[3]; // ��¼��ʤλ����Ϣ

void WinPlace()
{
	for(int player = 1; player <= 2; player++)
	{
		WIN[player] = 0;
		for(int i = 0; i < N; i++) if (!board.full(i))
		{
			board.PlaceChess(i, player);
			if (board.GameOver(i, player)) WIN[player] = WIN[player] * (N+1) + i + 1;
			board.RemoveChess(i, player);
		}
	}
}
