#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <conio.h>
#include <atlstr.h>
#include "Point.h"
#include "Strategy.h"

//#define _GETLOG
//#define _DEBUG

#ifdef _GETLOG
int GAME[409], GAMETOT = 0;
#endif

using namespace std;

#define TIME 1

#define clr(x, c) memset(x,c,sizeof(x))

/*
	策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误

	input:
		为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
		M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
		top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
		_board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
				你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
				board[x][y]表示第x行、第y列的点(从0开始计)
				board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
		lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
				落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
		noX, noY : 棋盘上的不可落子点(注:其实这里给出的top已经替你处理了不可落子点，也就是说如果某一步
				所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
				所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
				当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
		以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
	output:
		你的落子点Point
*/

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
inline int max4(int a, int b, int c, int d){return max(max(a,b),max(c,d));}
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
			int d[8];
			for(int dir = 0; dir < 8; dir++)
			{
				d[dir] = 0;
				while (Check(x+Dx[dir]*(d[dir]+1), y+Dy[dir]*(d[dir]+1), player))
					d[dir]++;
			}
			if (max4(d[0] + d[4], d[1] + d[5], d[2] + d[6], d[3] + d[7]) >= 3)
			{
				WIN[player] = WIN[player] * (N+1) + x + 1;

				#ifdef _DEBUG
				for(int dir = 0; dir < 8; dir++) _cprintf("%d ", d[dir]);
				_cprintf("==== ");
				#endif
			}
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

inline int Search(int now, int player, int DDD)
{
	if (CheckLost()) return 0;

	int st = Choose(now);
	if (st == -1) return 0;

	#ifdef _DEBUG
	_cprintf("%d %d %d ---- ", now, player, DDD);
	#endif

	CanWin();

	if (WIN[player]) st = WIN[player] % (N+1) - 1;
	else if (WIN[3 - player] >= N+1) return 0;
	else if (WIN[3 - player]) st = WIN[3 - player] - 1;

	board[st][--top[st]] = player;
	bool noFlag = false;
	if (top[noX] - 1 == noY) top[noX]--, noFlag = true;

	#ifdef _DEBUG
	_cprintf("%d %d %d\n", WIN[1], WIN[2], st);
	if (DDD > N*M)
	{
		while(true);
	}
	#endif

	n[now][st]++; t[now]++;
	if (nx[now][st] == 0) nx[now][st] = ++tot;
	int winorlost = Search(nx[now][st], 3-player, DDD+1);
	if (winorlost == 0)
		w[now][st]++;

	if (noFlag) top[noX]++;
	board[st][top[st]++] = 0;

	return (1 - winorlost);
}

// Copy end

inline void Init()
{
	for(int i = 0; i <= tot; i++)
	{
		clr(nx[i], 0);
		clr(w[i], 0);
		clr(n[i], 0);
		t[i] = 0;
	}
	tot = 0;
}

extern "C" __declspec(dllexport) Point* getPoint(const int _M, const int _N, const int* _top, const int* _board,
	const int lastX, const int lastY, const int _noX, const int _noY){

	N = _N, M = _M, noX = _noY, noY = _noX;
	for(int i = 0; i < N; i++) top[i] = _top[i];
	for(int j = 0; j < M; j++) for(int i = 0; i < N; i++)
		board[i][j] = _board[j*N+i];
	Init();

	/*
		不要更改这段代码
	*/

	srand(1);

	#ifdef _GETLOG
	AllocConsole();
	#endif

    clock_t startTime = clock();
	while((double)(clock()-startTime)/CLOCKS_PER_SEC < TIME) Search(0,2,0);
	// while(tot < 100000) Search(0,2);

	double mx = -1;
	int Ans = -1;
	for(int i = 0; i < N; i++) if (top[i])
		if (mx < 1.0*w[0][i]/max(1,n[0][i]))
			mx = 1.0*w[0][i]/max(1,n[0][i]), Ans = i;

	#ifdef _GETLOG
	for(int i = 0; i < N; i++) if (top[i])
		_cprintf("%d, w = %d, n = %d\n", i, w[0][i], n[0][i]);
	if (lastY != -1) GAME[GAMETOT++] = lastY;
	_cprintf("\n%d %d %d %d\n%d\n", N, M, noX, noY, GAMETOT);
	for(int i = 0; i < GAMETOT; i++)
		_cprintf("%d ", GAME[i]);
	_cprintf("\n\nAns: %d\n%.2lf%%\n\n====================================================\n\n", Ans, mx*100);
	GAME[GAMETOT++] = Ans;
	#endif

	/*
		不要更改这段代码
	*/

	return new Point(top[Ans]-1, Ans);
}


/*
	getPoint函数返回的Point指针是在本dll模块中声明的，为避免产生堆错误，应在外部调用本dll中的
	函数来释放空间，而不应该在外部直接delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	清除top和board数组
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	添加你自己的辅助函数，你可以声明自己的类、函数，添加新的.h .cpp文件来辅助实现你的想法
*/
