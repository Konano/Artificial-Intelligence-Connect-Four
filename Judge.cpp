#include <atlstr.h>
#include "Strategy.h"
#include "Judge.h"

inline int max4(int a, int b, int c, int d){return max(max(a,b),max(c,d));}

// 八方位，0 为向左，之后为逆时针旋转

int Dx[8] = {-1,-1,0,1,1,1,0,-1};
int Dy[8] = {0,1,1,1,0,-1,-1,-1};


// 检查 board[x][y] 是否为 player 的棋子

inline bool CheckBoard(int x, int y, int player)
{
    if (0<=x && x<N && 0<=y && y<M) return board[x][y]==player;
    return false;
}


// 检查双方是否有制胜位

int WIN[3]; // 记录制胜位的信息

void WinPlace()
{
    for(int player = 1; player <= 2; player++)
    {
        WIN[player] = 0;
        for(int i = 0; i < N; i++) if (top[i])
        {
            int x = i, y = top[i]-1; // 枚举每一列的顶端
            int d[8]; // 记录某一格往八方向能延伸多少个同色棋
            for(int dir = 0; dir < 8; dir++)
            {
                d[dir] = 0;
                while (CheckBoard(x+Dx[dir]*(d[dir]+1), y+Dy[dir]*(d[dir]+1), player))
                    d[dir]++;
            }
            if (max4(d[0] + d[4], d[1] + d[5], d[2] + d[6], d[3] + d[7]) >= 3) // 相对方向延伸棋数加起来达到 3 的则为制胜位
            {
                WIN[player] = WIN[player] * (N+1) + x + 1; // 使用 N+1 进制储存多种可能的制胜位
            }
        }
    }
}


// 检测局面是否结束

bool GameOver()
{
    int Count = 0, player = 0;

    // 检查纵向有没有四子相连

    for(int i = 0; i < N; i++)
    {
        Count = player = 0;
        for(int j = 0; j < M; j++)
        {
            if (board[i][j])
                if (!player || player == board[i][j])
                    player = board[i][j], Count++;
                else
                    player = board[i][j], Count = 1;
            else
                Count = player = 0;
            if (Count == 4) return true;
        }
    }

    // 检查横向有没有四子相连

    for(int j = 0; j < M; j++)
    {
        Count = player = 0;
        for(int i = 0; i < N; i++)
        {
            if (board[i][j])
                if (!player || player == board[i][j])
                    player = board[i][j], Count++;
                else
                    player = board[i][j], Count = 1;
            else
                Count = player = 0;
            if (Count == 4) return true;
        }
    }

    // 检查「左上-右下」方向有没有四子相连

    for(int o = -M; o < N; o++)
    {
        Count = player = 0;
        for(int i = max(o,0), j = i-o; i < N && j < M; i++, j++)
        {
            if (board[i][j])
                if (!player || player == board[i][j])
                    player = board[i][j], Count++;
                else
                    player = board[i][j], Count = 1;
            else
                Count = player = 0;
            if (Count == 4) return true;
        }
    }

    // 检查「左下-右上」方向有没有四子相连

    for(int o = 0; o < N+M; o++)
    {
        Count = player = 0;
        for(int i = min(N-1,o), j = o-i; i >= 0 && j < M; i--, j++)
        {
            if (board[i][j])
                if (!player || player == board[i][j])
                    player = board[i][j], Count++;
                else
                    player = board[i][j], Count = 1;
            else
                Count = player = 0;
            if (Count == 4) return true;
        }
    }

    // 若都没有则返回 false

    return false;
}

bool Start()
{
    static int sum;
    sum = 0;
    for(int i = 0; i < N; i++) for(int j = 0; j < M; j++)
    {
        sum += (board[i][j] > 0);
        if (sum > 1) return false;
    }
    return true;
}
