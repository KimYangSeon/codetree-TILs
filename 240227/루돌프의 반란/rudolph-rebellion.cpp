#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

int n, m, p, c, d, rx, ry, aliveCnt;
bool isGameover;
map<int, pair<int, int>> santa;
int board[51][51];
int dx[8] = {-1, 0, 1, 0, -1, -1, 1, 1};
int dy[8] = {0, 1, 0, -1, -1, 1, 1, -1};
int santaStat[31]; // -1: 죽음 0: 기절 1: 보통
int score[31];
int stunCnt[31];

int getDist(int x1, int y1, int x2, int y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

void Collide(int num, int dir, int dist)
{
    // 밀려난 칸에 다른 산타가 있으면 연쇄적으로 밀림
    int x = santa[num].first;
    int y = santa[num].second;

    int nx = x + dx[dir] * dist;
    int ny = y + dy[dir] * dist;

    if (nx < 1 || nx > n || ny < 1 || ny > n)
    { // 산타 죽음
        santaStat[num] = -1;
        aliveCnt--;
        //cout << "dead" << num << '\n';
        if (aliveCnt == 0) isGameover = true;
        return;
    }
    else if (board[nx][ny] > 0)
    { // 다른 산타와 충돌
        Collide(board[nx][ny], dir, 1);
    }

    santa[num] = {nx, ny};
    board[nx][ny] = num;
}

void SantaMove(int num, int dir)
{
    // 산타 좌표 변경
    int x = santa[num].first;
    int y = santa[num].second;
    board[x][y] = 0;
    x += dx[dir];
    y += dy[dir];
    santa[num] = {x, y}; // 산타가 이동할 곳
    //cout << "santa " << num << " -> " << x << ' ' << y << '\n';

    // 루돌프와 충돌 체크
    if (board[x][y] == 100)
    {
        score[num] += d; // +d점
        santaStat[num] = 0; // 기절
        stunCnt[num] = 2;
        if (dir < 4)
        {
            dir = (dir + 2) % 4;
        }
        else
        {
            dir = (dir + 2) % 4 + 4;
        }

        Collide(num, dir, d); // 산타 번호 & 방향 & 이동거리
    }
    else
    {
        board[x][y] = num;
    }
}

void Santa()
{
    int minDist = 51 * 51 + 51 * 51;
    int minDir = 100;

    for (auto s : santa)
    {
        int pn, x, y;
        pn = s.first;
        x = s.second.first;
        y = s.second.second;

        if (santaStat[pn] != 1)
            continue; // 기절 or 탈락한 산타 제외

        minDist = getDist(x, y, rx, ry);
        minDir = 100;

        for (int dir = 0; dir < 4; dir++) // 루돌프에게 가까워지는 방향 구함
        {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (nx < 1 || nx > n || ny < 1 || ny > n)
                continue; // 갈 수 없는 칸
            if (board[nx][ny] > 0 && board[nx][ny] <= 30)
                continue; // 다른 산타가 있음
            int curDist = getDist(nx, ny, rx, ry);
            if (curDist < minDist)
            {
                minDist = curDist;
                minDir = dir;
            }
        }
        // 루돌프에게 가까워지는 방향으로 1칸 이동

        if (minDir == 100)
        {
            continue; //  갈 수 없음
        }

        SantaMove(pn, minDir);
    }
}

void RudolphRush(int dir)
{
    // 루돌프 좌표 변경
    board[rx][ry] = 0;
    rx += dx[dir];
    ry += dy[dir];

    // 충돌 체크
    if (board[rx][ry] != 0)
    {
        int santaNum = board[rx][ry];
        score[santaNum] += c; // +c점
        santaStat[santaNum] = 0; // 기절
        stunCnt[santaNum] = 2;
        Collide(santaNum, dir, c); // 산타 번호 & 방향 & 이동거리
    }

    board[rx][ry] = 100;
}

void Rudolph()
{
    int pn, x, y, targetNum, minDir, targetX, targetY;
    int minDist1 = 51 * 51 + 51 * 51;
    int minDist2 = minDist1;

    // 가장 가까운 산타로 향하는 방향 알아냄
    for (auto s : santa)
    {
        pn = s.first;
        x = s.second.first;
        y = s.second.second;

        if (santaStat[pn] == -1)
            continue; // 탈락한 산타 제외

        int curDist = getDist(rx, ry, x, y);
        if (curDist < minDist1)
        {
            minDist1 = curDist;
            targetNum = pn;
            targetX = x;
            targetY = y;
        }
        else if (curDist == minDist1)
        {
            if (x > targetX)
            {
                targetNum = pn;
                targetX = x;
                targetY = y;
            }
            else if (x == targetX && y > targetY)
            {
                targetNum = pn;
                targetX = x;
                targetY = y;
            }
        }
    }

    for (int dir = 0; dir < 8; dir++)
    {
        int nx = rx + dx[dir];
        int ny = ry + dy[dir];
        int curDist = getDist(nx, ny, targetX, targetY);
        if (curDist < minDist2)
        {
            minDist2 = curDist;
            minDir = dir;
        }
    }
    // 돌진
    //cout << "rudolph target: " << targetNum << '\n';
    RudolphRush(minDir);
}

void Simulate()
{
    // 기절했던 산타 원래대로 되돌리기
    for (auto s : santa)
    {
        if(santaStat[s.first] == -1) continue; // 죽은 산타 제외
        if (stunCnt[s.first] > 0)
        {
            stunCnt[s.first]--;
            if (stunCnt[s.first] == 0)
                santaStat[s.first] = 1;
        }
    }

    // 루돌프
    Rudolph();

    if (isGameover)
    {
        return;
    }

    // 산타
    Santa();

    // 산타가 모두 탈락하면 즉시 게임 종료
    if (isGameover)
    {
        return;
    }

    // 아직 탈락하지 않은 산타한테 1점 추가 부여
    for (auto s : santa)
    {
        if (santaStat[s.first] != -1)
        {
            score[s.first]++;

        }
    }
    
    /*
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cout << board[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << "------------------------\n";
*/
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m >> p >> c >> d;
    cin >> rx >> ry;
    board[rx][ry] = 100;
    aliveCnt = p;
    for (int i = 0; i < p; i++)
    {
        int x, y, pn;
        cin >> pn >> x >> y;
        board[x][y] = pn;
        santaStat[pn] = 1;
        santa[pn] = {x, y};
    }

    for (int i = 0; i < m && !isGameover; i++)
        Simulate();

    for(int i=1; i<=p; i++){
        cout << score[i] << ' ';
    }


    return 0;
}