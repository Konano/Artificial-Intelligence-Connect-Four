#include <cmath>
#include "Strategy.h"
#include "Judge.h"
#include "MCTS.h"

int Total_Situation = 0; // �ܾ�����

int Next_Situation[MAXSITUATION][MAXSIZE]; // Next_Situation[S][next] Ϊ�� S �����·������ڵ� next �к�ľ�����

int Win_Times[MAXSITUATION][MAXSIZE]; // ÿ����֧�����������ʤ���Ĵ���

int Choose_Times[MAXSITUATION][MAXSIZE]; // ÿ����֧���汻̽���Ĵ���

int Total_Times[MAXSITUATION]; // ���汻̽���Ĵ���

// ��ʼ��

void Situation_Init()
{
	for(int i = 0; i <= Total_Situation; i++)
	{
		clr(Next_Situation[i], 0);
		clr(Win_Times[i], 0);
		clr(Choose_Times[i], 0);
		Total_Times[i] = 0;
	}
	Total_Situation = 0;
}

// ѡ��ĳһ��֧������Ϊ̽���ķ���

inline int BestAction(const int nowSituation)
{
	// ���з�֧����δ̽���������ȡ���ѡ�����

	for(int i = 0; i < N; i++) if (!board.full(i) && Choose_Times[nowSituation][i] == 0)
	{
		int x = rand() % N;
		while (board.full(x)) x = rand() % N;
		return x;
	}

	// �����з�֧�����̽��������ʹ�� UCT ��ʽѡ�����ž���

	double Possibility = -1;
	int Action = -1;
	for(int i = 0; i < N; i++) if (!board.full(i))
		if (Possibility < (double)Win_Times[nowSituation][i]/Choose_Times[nowSituation][i] + sqrt(2*log(Total_Times[nowSituation])/Choose_Times[nowSituation][i]))
		{
			Possibility = (double)Win_Times[nowSituation][i]/Choose_Times[nowSituation][i] + sqrt(2*log(Total_Times[nowSituation])/Choose_Times[nowSituation][i]);
			Action = i;
		}

	return Action;
}

// ���ؿ�������

int LastAction;

int MCTS(const int now, const int player)
{
	if (now && board.GameOver(LastAction)) return 0;

	// ѡȡ��֧����

	int Action = BestAction(now);
	if (Action == -1) return rand()&1; // �޷������ӣ�ƽ�֣������ȷ��ʤ��

	WinPlace();

	if (WIN[player]) Action = WIN[player] % (N+1) - 1;
	else if (WIN[3 - player] >= N+1) return 0;
	else if (WIN[3 - player]) Action = WIN[3 - player] - 1;

	// ���¾���

	board.PlaceChess(Action, player);
	LastAction = Action;

	// ����̽��

	Choose_Times[now][Action]++;
	Total_Times[now]++;
	if (Next_Situation[now][Action] == 0)
		Next_Situation[now][Action] = ++Total_Situation;
	int winorlost = MCTS(Next_Situation[now][Action], 3-player);
	if (winorlost == 0)
		Win_Times[now][Action]++;

	// ��ԭ����

	board.RemoveChess(Action, player);

	// ���򴫲�

	return (1 - winorlost);
}

double Possibility;
int Action;
int GetFinalAction()
{
	Possibility = -1;
	Action = -1;
	for(int i = 0; i < N; i++) if (!board.full(i))
		if (Possibility < (double)Win_Times[0][i]/max(1,Choose_Times[0][i]))
			Possibility = (double)Win_Times[0][i]/max(1,Choose_Times[0][i]), Action = i;
	return Action;
}

#ifdef _GETLOG
int GAME[409], GAMETOT = 0; // ��ʼ�������ʷ��Ϣ��¼����
void GetFinalData()
{
	for(int i = 0; i < N; i++) if (!board.full(i))
		_cprintf("%d, w = %d, n = %d\n", i, Win_Times[0][i], Choose_Times[0][i]);
	if (lastX != -1) GAME[GAMETOT++] = lastX;
	_cprintf("\n%d %d %d %d\n%d\n", N, M, noX, noY, GAMETOT);
	for(int i = 0; i < GAMETOT; i++)
		_cprintf("%d ", GAME[i]);
	_cprintf("\n\nAction: %d\n%.2lf%%\n%d\n\n====================================================\n\n", Action, Possibility*100, Total_Situation);
	GAME[GAMETOT++] = Action;
}
#endif