#include <cmath>
#include <conio.h>
#include <atlstr.h>
#include "Strategy.h"
#include "Judge.h"
#include "MCTS.h"

int Total_Situation; // �ܾ�����

int nowSituation;
int _nowSituation;

int Next_Situation[MAXSITUATION][MAXSIZE]; // Next_Situation[S][next] Ϊ�� S �����·������ڵ� next �к�ľ�����

int Win_Times[MAXSITUATION][MAXSIZE]; // ÿ����֧�����������ʤ���Ĵ���

int Choose_Times[MAXSITUATION][MAXSIZE]; // ÿ����֧���汻̽���Ĵ���

int Total_Times[MAXSITUATION]; // ���汻̽���Ĵ���

class STACK
{
	int Total, q[MAXSITUATION];
public:
	STACK() { Total = 0; };
	bool empty() { return Total == 0; }
	void clear() { Total = 0; }
	void push(int v) { q[Total++] = v; }
	int front() { return q[--Total]; }
} ST; // ɾ�������±��Ŵ�

inline void CleanSituation(int i)
{
	clr(Next_Situation[i], 0);
	clr(Win_Times[i], 0);
	clr(Choose_Times[i], 0);
	Total_Times[i] = 0;
	ST.push(i);
}

inline int NewSituation()
{
	if (ST.empty())
		return ++Total_Situation;
	else
		return ST.front();
}

inline void DeleteSituation(int now)
{
	if (now == nowSituation) return;
	for(int i = 0; i < MAXSIZE; i++) if (Next_Situation[now][i]) DeleteSituation(Next_Situation[now][i]);
	CleanSituation(now);
}

// ��ʼ��

void Situation_Init()
{
	if (Start())
	{
		for(int i = 0; i <= Total_Situation; i++) CleanSituation(i);
		ST.clear();
		_nowSituation = nowSituation = Total_Situation = 0;
		return;
	}
	nowSituation = Next_Situation[nowSituation][lastX];

	DeleteSituation(_nowSituation);
	_nowSituation = nowSituation;
}

// ѡ��ĳһ��֧������Ϊ̽���ķ���

inline int BestAction(int nowSituation)
{
	// ���з�֧����δ̽���������ȡ���ѡ�����

	for(int i = 0; i < N; i++) if (top[i] && Choose_Times[nowSituation][i] == 0)
	{
		int x = rand() % N;
		while (!top[x]) x = rand() % N;
		return x;
	}

	// �����з�֧�����̽��������ʹ�� UCT ��ʽѡ�����ž���

	double Possibility = -1;
	int BestAction = -1;
	for(int i = 0; i < N; i++) if (top[i])
		if (Possibility < (double)Win_Times[nowSituation][i]/Choose_Times[nowSituation][i] + sqrt(2*log(Total_Times[nowSituation])/Choose_Times[nowSituation][i]))
		{
			Possibility = (double)Win_Times[nowSituation][i]/Choose_Times[nowSituation][i] + sqrt(2*log(Total_Times[nowSituation])/Choose_Times[nowSituation][i]);
			BestAction = i;
		}
	return BestAction;
}

// ���ؿ�������

int MCTS(int now, int player)
{
	#ifdef _DEBUG
	_cprintf("%d %d %d ---- ", now, player);
	#endif

	if (GameOver()) return 0;

	// ѡȡ��֧����

	int Action = BestAction(now);
	if (Action == -1) return rand()&1; // �޷������ӣ�ƽ�֣������ȷ��ʤ��

	WinPlace();

	if (WIN[player]) Action = WIN[player] % (N+1) - 1;
	else if (WIN[3 - player] >= N+1) return 0;
	else if (WIN[3 - player]) Action = WIN[3 - player] - 1;

	// ���¾���

	board[Action][--top[Action]] = player;
	bool noFlag = false;
	if (top[noX] - 1 == noY)
		top[noX]--, noFlag = true;

	#ifdef _DEBUG
	_cprintf("%d %d %d\n", WIN[1], WIN[2], Action);
	#endif

	// ����̽��

	Choose_Times[now][Action]++;
	Total_Times[now]++;
	if (Next_Situation[now][Action] == 0)
		Next_Situation[now][Action] = NewSituation();
	int winorlost = MCTS(Next_Situation[now][Action], 3-player);
	if (winorlost == 0)
		Win_Times[now][Action]++;

	// ��ԭ����

	if (noFlag)
		top[noX]++;
	board[Action][top[Action]++] = 0;

	// ���򴫲�

	return (1 - winorlost);
}

double Possibility;
int Action;
int GetFinalAction()
{
	Possibility = -1;
	Action = -1;
	for(int i = 0; i < N; i++) if (top[i])
		if (Possibility < (double)Win_Times[nowSituation][i]/max(1,Choose_Times[nowSituation][i]))
			Possibility = (double)Win_Times[nowSituation][i]/max(1,Choose_Times[nowSituation][i]), Action = i;
	nowSituation = Next_Situation[nowSituation][Action];
	return Action;
}

#ifdef _GETLOG
int GAME[409], GAMETOT = 0; // ��ʼ�������ʷ��Ϣ��¼����
void GetFinalData()
{
	for(int i = 0; i < N; i++) if (top[i])
		_cprintf("%d, w = %d, n = %d\n", i, Win_Times[_nowSituation][i], Choose_Times[_nowSituation][i]);
	if (lastX != -1) GAME[GAMETOT++] = lastX;
	_cprintf("\n%d %d %d %d\n%d\n", N, M, noX, noY, GAMETOT);
	for(int i = 0; i < GAMETOT; i++)
		_cprintf("%d ", GAME[i]);
	_cprintf("\n\nAction: %d\n%.2lf%%\n\n====================================================\n\n", Action, Possibility*100);
	GAME[GAMETOT++] = Action;
}
#endif