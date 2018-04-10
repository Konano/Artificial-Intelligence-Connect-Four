#define _DEBUG

#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

#define TIME 8

// Copy from this

#define MAXSITUATION 2000000

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
	for(int i = 0; i < N; i++) if (top[i] && n[now][i] == 0)
	{
		int x = rand() % N;
		while (!top[x]) x = rand() % N;
		return x;
	}

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

int WIN[3];
int Dx[8] = {-1,-1,0,1,1,1,0,-1};
int Dy[8] = {0,1,1,1,0,-1,-1,-1};
inline int max4 (int a, int b, int c, int d){return max(max(a,b),max(c,d));}
inline bool Check(int x, int y, int player)
{
	if (0<=x && x<N && 0<=y && y<M) return board[x][y]==player;
	return false;
}
inline void CanWin()
{
	for(int player = 1; player <= 2; player++)
	{
		WIN[player] = 0;
		for(int i = 0; i < N; i++) if (top[i])
		{
			int x = i, y = top[i]-1;
			int d[8] = {0};
			for(int dir = 0; dir < 8; dir++)
				while (Check(x+Dx[dir]*(d[dir]+1), y+Dy[dir]*(d[dir]+1), player))
					d[dir]++;
			if (max4(d[0] + d[4], d[1] + d[5], d[2] + d[6], d[3] + d[7]) >= 3)
				WIN[player] = WIN[player] * (N+1) + x + 1;
		}
	}
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

	int st = Choose(now);
	if (st == -1) return 0;

	CanWin();
	if (WIN[player]) st = WIN[player] % (N+1) - 1;
	else if (WIN[3 - player] >= N+1) return 0;
	else if (WIN[3 - player]) st = WIN[3 - player] - 1;

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

// Copy end

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
	while((double)(clock()-startTime)/CLOCKS_PER_SEC < TIME) Search(0,2);

	#ifdef _DEBUG
	printf("Situation: %d\n", tot);
	#endif

	double mx = -1;
	int Ans = -1;
	for(int i = 0; i < N; i++) if (top[i])
		if (mx < 1.0*w[0][i]/max(1,n[0][i]))
			mx = 1.0*w[0][i]/max(1,n[0][i]), Ans = i;

	//printf("%d %d\n", Ans, top[Ans]-1);
	printf("%d %.3lf%%\n", Ans, mx*100);
	return 0;
}
