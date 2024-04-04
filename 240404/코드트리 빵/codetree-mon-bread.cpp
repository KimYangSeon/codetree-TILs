#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

int n, m, t, cnt;
int board[16][16]; // 베이스캠프: 100, 편의점: 번호
int vis[16][16][31];
int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, -1, 1, 0};
vector<pair<int, int>> curPos;
vector<pair<int, int>> storePos;
vector<bool> isExit;
queue<pair<int,int>> wall;

void BuildWall(){
    // 모든 이동이 끝난 후 이동할 수 없는 칸으로 변경 
    while(!wall.empty()){
        auto cur = wall.front(); wall.pop();
        board[cur.first][cur.second] = -1;
        //cout << "wall: " << cur.first << ' ' << cur.second << '\n';
    }
}

void FindBase(int p)
{
    if(p > m) return;
    // p번 편의점과 가장 가까운 베이스캠프 찾기

    queue<pair<int, int>> q;
    vector<pair<int, int>> basePos;
    pair<int, int> closest;
    q.push(storePos[p]); // 시작지점: p번 편의점
    vis[storePos[p].first][storePos[p].second][p] = t;
    bool isFound = false;
    while (!isFound)
    {
        auto cur = q.front();
        q.pop();
        for (int dir = 0; dir < 4; dir++)
        {
            int nx = cur.first + dx[dir];
            int ny = cur.second + dy[dir];
            if (nx < 1 || nx > n || ny < 1 || ny > n)
                continue;
            if (vis[nx][ny][p] == t || board[nx][ny] == -1)
                continue;

            vis[nx][ny][p] = t;
            if (board[nx][ny] == 100)
            { // 베이스캠프 찾음
                isFound = true;
                basePos.push_back({nx, ny});
            }
            else
            {
                q.push({nx, ny});
            }
        }
    }
    closest = basePos[0];
    for (auto b : basePos)
    {
        if (closest.first > b.first)
        {
            closest = b;
        }
        else if (closest.first == b.first && closest.second > b.second)
        {
            closest = b;
        }
    }
    curPos[p] = closest; // 베이스캠프로 이동
    board[closest.first][closest.second] = -1; // 해당 칸을 지나갈 수 없어짐
    //cout << "base: " << closest.first << ' ' << closest.second << '\n';
}

void FindStore(int p)
{
    // p번 편의점으로 가는 최단경로 찾기 

    queue<pair<int, int>> q;
    pair<int,int> path[16][16]; // 경로

    q.push(curPos[p]); // 시작지점: p번 편의점
    vis[curPos[p].first][curPos[p].second][p] = t;
    bool isFound = false;

    while (!isFound)
    {
        auto cur = q.front();
        q.pop();
        
        for (int dir = 0; dir < 4; dir++)
        {
            int nx = cur.first + dx[dir];
            int ny = cur.second + dy[dir];
            if (nx < 1 || nx > n || ny < 1 || ny > n)
                continue;
            if (vis[nx][ny][p] == t || board[nx][ny] == -1)
                continue;
            //cout << nx << ' ' << ny << '\n';
            vis[nx][ny][p] = t;
            path[nx][ny] = cur;
            if (board[nx][ny] == p)
            { // 편의점 찾음
                isFound = true;
                break;
            }

            q.push({nx, ny});
        }
    }

    pair<int,int> temp = storePos[p];
    while(true){
        auto temp2 = path[temp.first][temp.second];
        if(temp2 == curPos[p]) break;
        temp = temp2;
    }
    //cout << temp.first << ' ' << temp.second << '\n';

    curPos[p] = temp; // 이동
    if(curPos[p] == storePos[p]){
        isExit[p]=1;
        cnt--;
        wall.push(storePos[p]);
    }
}

void MoveToStore(int p)
{
    p = min(m,t-1);

    for (int i = 1; i <= p; i++)
    { // 편의점을 향해 한 칸 이동
        if(isExit[i]) continue;
        FindStore(i);
    }

    BuildWall();
}

int main()
{
    //ios::sync_with_stdio(0);
    //cin.tie(0);

    cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cin >> board[i][j];
            if (board[i][j] == 1)
                board[i][j] = 100;
        }
    }

    curPos.push_back({0, 0});
    storePos.push_back({0, 0});
    isExit.push_back(1);
    cnt = m;

    for (int i = 1; i <= m; i++)
    {
        int x, y;
        cin >> x >> y;
        board[x][y] = i;
        curPos.push_back({0, 0});
        storePos.push_back({x, y});
        isExit.push_back(0);
    }

    t++;
    FindBase(t);

    while (cnt > 0)
    {
        t++;
        MoveToStore(t);
        FindBase(t);
    }
    cout << t;

    return 0;
}