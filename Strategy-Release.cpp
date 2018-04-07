#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <conio.h>
#include <atlstr.h>
#include "Point.h"
#include "Strategy.h"

#define _DEBUG

#ifdef _DEBUG
int GAME[409], GAMETOT = 0;
#endif

using namespace std;

#define TIME 1

#define clr(x, c) memset(x,c,sizeof(x))

/*
	���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������

	input:
		Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
		M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
		top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
		_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
				��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
				board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
				board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
		lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
				����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
		noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
				������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
				��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
				��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
		���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
	output:
		������ӵ�Point
*/

#define MAXSITUATION 2000000

int N, M, top[20], board[20][20], noX, noY;

int tot = 0;
int nx[MAXSITUATION][20];
int w[MAXSITUATION][20], n[MAXSITUATION][20], t[MAXSITUATION];

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
		return i;
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

extern "C" __declspec(dllexport) Point* getPoint(const int _M, const int _N, const int* _top, const int* _board,
	const int lastX, const int lastY, const int _noX, const int _noY){

	N = _N, M = _M, noX = _noY, noY = _noX;
	for(int i = 0; i < N; i++) top[i] = _top[i];
	for(int j = 0; j < M; j++) for(int i = 0; i < N; i++)
		board[i][j] = _board[j*N+i];
	Init();

	/*
		��Ҫ������δ���
	*/

	srand(1);

    clock_t startTime = clock();
	while((double)(clock()-startTime)/CLOCKS_PER_SEC < TIME) Search(0,2);
	// while(tot < 100000) Search(0,2);

	double mx = 0;
	int Ans = 0;
	for(int i = 0; i < N; i++) if (top[i])
		if (mx < 1.0*w[0][i]/n[0][i])
			mx = 1.0*w[0][i]/n[0][i], Ans = i;

	#ifdef _DEBUG
	AllocConsole();
	for(int i = 0; i < N; i++) if (top[i])
		_cprintf("%d, w = %d, n = %d\n", i, w[0][i], n[0][i]);
	if (lastY != -1) GAME[GAMETOT++] = lastY;
	_cprintf("\n%d %d %d %d\n%d\n", N, M, noX, noY, GAMETOT);
	for(int i = 0; i < GAMETOT; i++)
		_cprintf("%d ", GAME[i]);
	_cprintf("\n\nAns: %d\n\n====================================================\n\n", Ans);
	GAME[GAMETOT++] = Ans;
	#endif

	/*
		��Ҫ������δ���
	*/

	return new Point(top[Ans]-1, Ans);
}


/*
	getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
	�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	���top��board����
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	������Լ��ĸ�������������������Լ����ࡢ����������µ�.h .cpp�ļ�������ʵ������뷨
*/
