#define _debug

#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

#define MAXSITUATION 2000000
#define TIME 2.8

int N, M, top[20], board[20][20], noX, noY;

int tot = 0;
int nx[MAXSITUATION][20];
int w[MAXSITUATION][20], n[MAXSITUATION][20], t[MAXSITUATION];

inline double Rand(double d)
{
	double x = (double)rand() / RAND_MAX;
	double y = (double)rand() / RAND_MAX;
	return (x + y / RAND_MAX) * d;
}

inline int Choose(int now)
{
	double mx = -1;
	int Choose = -1;
	for(int i = 0; i < N; i++) if (top[i])
		if (mx < (double)w[now][i]/n[now][i] + sqrt(2*log(t[now])/n[now][i]))
		{
			mx = (double)w[now][i]/n[now][i] + sqrt(2*log(t[now])/n[now][i]);
			Choose = i;
		}
	return Choose;
}

inline bool CheckLost()
{
	int Count = 0, now = 0;
	for(int i = 0; i < N; i++)
	{
		Count = now = 0;
		for(int j = 0; j < M; j++)
		{
			if (board[i][j])
				if (!now || now == board[i][j])
					now = board[i][j], Count++;
				else
					now = board[i][j], Count = 1;
			else
				Count = now = 0;
			if (Count == 4) return true;
		}
	}
	for(int j = 0; j < M; j++)
	{
		Count = now = 0;
		for(int i = 0; i < N; i++)
		{
			if (board[i][j])
				if (!now || now == board[i][j])
					now = board[i][j], Count++;
				else
					now = board[i][j], Count = 1;
			else
				Count = now = 0;
			if (Count == 4) return true;
		}
	}
	for(int o = -M; o < N; o++)
	{
		Count = now = 0;
		for(int i = max(o,0), j = i-o; i < N && j < M; i++, j++)
		{
			if (board[i][j])
				if (!now || now == board[i][j])
					now = board[i][j], Count++;
				else
					now = board[i][j], Count = 1;
			else
				Count = now = 0;
			if (Count == 4) return true;
		}
	}
	for(int o = 0; o < N+M; o++)
	{
		Count = now = 0;
		for(int i = min(N-1,o), j = o-i; i >= 0 && j < M; i--, j++)
		{
			if (board[i][j])
				if (!now || now == board[i][j])
					now = board[i][j], Count++;
				else
					now = board[i][j], Count = 1;
			else
				Count = now = 0;
			if (Count == 4) return true;
		}
	}
	return false;
}

inline int Search(int now, int player)
{
	if (CheckLost()) return 0;

	if (t[now] == 0)
		for(int i = 0; i < N; i++) if (top[i])
			n[now][i] = 1, t[now]++;
	int st = Choose(now);
	if (st == -1) return 0;

	board[st][--top[st]] = player;
	bool noFlag = false;
	if (top[noX] - 1 == noY) top[noX]--, noFlag = true;

	n[now][st]++; t[now]++;
	if (nx[now][st] == 0) nx[now][st] = ++tot;
	int winorlost = Search(nx[now][st], 3-player);
	if (winorlost == 0)
		w[now][st]++;

	if (noFlag) top[noX]++;
	board[st][top[st]++] = 0;

	return (1 - winorlost);
}

inline void GetData()
{
	scanf("%d%d%d%d", &N, &M, &noX, &noY);
	for(int i = 0; i < N; i++) top[i] = M;
	int L, player;
	scanf("%d", &L);
	while (L--)
	{
		player = (L&1 ? 2 : 1);
		int st;
		scanf("%d", &st);
		board[st][--top[st]] = player;
		if (top[noX] - 1 == noY) top[noX]--;
	}
}

int main()
{
	GetData();

	srand(1);

	clock_t startTime = clock();
	// while((double)(clock()-startTime)/CLOCKS_PER_SEC < TIME) Search(0,2);
	while(tot < 100000) Search(0,2);

	#ifdef _debug
	printf("Situation: %d\n", tot);
	#endif

	double mx = 0;
	int Ans = 0;
	for(int i = 0; i < N; i++) if (top[i])
		if (mx < 1.0*w[0][i]/n[0][i])
			mx = 1.0*w[0][i]/n[0][i], Ans = i;

	//printf("%d %d\n", Ans, top[Ans]-1);
	printf("%d\n", Ans);
	return 0;
}
