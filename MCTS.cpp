#include <cmath>
#include "Strategy.h"
#include "Judge.h"
#include "MCTS.h"

int Total_Situation = 0; // 总局面数

int Next_Situation[MAXSITUATION][MAXSIZE]; // Next_Situation[S][next] 为在 S 局面下放棋子于第 next 列后的局面编号

int Win_Times[MAXSITUATION][MAXSIZE]; // 每个分支局面最后走向胜利的次数

int Choose_Times[MAXSITUATION][MAXSIZE]; // 每个分支局面被探索的次数

int Total_Times[MAXSITUATION]; // 局面被探索的次数

// 初始化

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

// 选择某一分支局面作为探索的方向

inline int BestAction(const int nowSituation)
{
	// 若有分支局面未探索过，则采取随机选择决策

	for(int i = 0; i < N; i++) if (!board.full(i) && Choose_Times[nowSituation][i] == 0)
	{
		int x = rand() % N;
		while (board.full(x)) x = rand() % N;
		return x;
	}

	// 若所有分支局面皆探索过，则使用 UCT 公式选择最优局面

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

// 蒙特卡洛搜索

int LastAction;

int MCTS(const int now, const int player)
{
	if (now && board.GameOver(LastAction)) return 0;

	// 选取分支局面

	int Action = BestAction(now);
	if (Action == -1) return rand()&1; // 无法下棋子（平局）则随机确定胜负

	WinPlace();

	if (WIN[player]) Action = WIN[player] % (N+1) - 1;
	else if (WIN[3 - player] >= N+1) return 0;
	else if (WIN[3 - player]) Action = WIN[3 - player] - 1;

	// 更新局面

	board.PlaceChess(Action, player);
	LastAction = Action;

	// 进行探索

	Choose_Times[now][Action]++;
	Total_Times[now]++;
	if (Next_Situation[now][Action] == 0)
		Next_Situation[now][Action] = ++Total_Situation;
	int winorlost = MCTS(Next_Situation[now][Action], 3-player);
	if (winorlost == 0)
		Win_Times[now][Action]++;

	// 还原局面

	board.RemoveChess(Action, player);

	// 反向传播

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
int GAME[409], GAMETOT = 0; // 初始化棋局历史信息记录数组
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