#include <iostream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
int board[11][11];
set<int> pBoard[11][11];
int n, m, k, ans, exitX, exitY, cnt;
vector<pair<int, int>> p;
vector<bool> pStat;

void printPos()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cout << board[i][j] << ' ';
        }
        cout << '\n';
    }

    for (int i = 0; i < m; i++)
    {
        if (pStat[i])
        {
            cout << i << ": " << p[i].first << ' ' << p[i].second << '\n';
        }
    }
}

void tryMove()
{
    for (int i = 0; i < m; i++)
    {
        if (!pStat[i])
            continue;
        int x = p[i].first;
        int y = p[i].second;
        int dist = abs(exitX - x) + abs(exitY - y); // 출구까지의 거리
        int minDist = dist;
        int minDir = 0;
        vector<pair<int, int>> nPosList; // 움직일 수 있는 좌표들 목록

        for (int dir = 0; dir < 4; dir++)
        {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (nx < 1 || nx > n || ny < 1 || ny > n)
                continue;
            int nDist = abs(exitX - nx) + abs(exitY - ny);
            if (nDist > minDist)
                continue;
            minDist = nDist;
            nPosList.push_back({nx, ny});
        }

        for (auto d : nPosList)
        {
            if (board[d.first][d.second] != 0 && board[d.first][d.second] != 100)
                continue; // 빈칸이 아니면 못움직임

            pBoard[x][y].erase(i);
            p[i] = d; // 움직임
            pBoard[d.first][d.second].insert(i);

            //cout << i << ": " << p[i].first << ' ' << p[i].second << '\n';
            ans++;
            if (d.first == exitX && d.second == exitY)
            { // 탈출
                pStat[i] = 0;
                pBoard[d.first][d.second].erase(i);
                //cout << i << "escape\n";
                cnt--;
            }
            break;
        }
    }
}

pair<int, int> getMinSquare(int px, int py, int width)
{
    pair<int, int> curSquarePos;
    if (exitX >= px && exitY >= py)
    { // 왼쪽 위
        curSquarePos = {max(1, exitX - width), max(1, exitY - width)};
    }
    else if (exitX >= px && exitY < py)
    { // 오른쪽 위
        if (exitX - px == width)
            curSquarePos = {px, max(1, py - width)};
        else
            curSquarePos = {max(1, exitX - width), exitY};
    }
    else if (exitX < px && exitY <= py)
    { // 오른쪽 아래
        curSquarePos = {max(1, px - width), max(1, py - width)};
    }
    else
    { // 왼쪽 아래
        if (exitY - py == width)
            curSquarePos = {max(1, px - width), py};
        else
            curSquarePos = {exitX, max(1, exitY - width)};
    }

    return curSquarePos;
}

void rotate()
{
    // 출구를 포함한 가장 작은 정사각형 찾기
    int minWidth = 11;
    pair<int, int> squarePos = {11, 11};

    for (int i = 0; i < m; i++)
    {
        if (!pStat[i])
            continue;
        pair<int, int> curSquarePos;
        int width = max(abs(exitX - p[i].first), abs(exitY - p[i].second));

        if (minWidth > width)
        {
            minWidth = width;
            squarePos = getMinSquare(p[i].first, p[i].second, width);
        }
        else if (minWidth == width)
        {
            curSquarePos = getMinSquare(p[i].first, p[i].second, width);

            if (curSquarePos.first < squarePos.first && curSquarePos.second < squarePos.second)
                squarePos = curSquarePos;
        }
    }

    // cout << "square: " << squarePos.first << ' ' << squarePos.second << ' ' << minWidth << '\n';

    int temp[11][11];
    set<int> tempSet[11][11];
    int maxY = squarePos.second + minWidth;
    int maxX = squarePos.first + minWidth;
    int tempCntX = 0;
    int tempCntY = 0;

    for (int i = squarePos.first; i <= maxX; i++)
    {
        tempCntX = 0;
        for (int j = squarePos.second; j <= maxY; j++)
        {
            temp[squarePos.first + tempCntX][maxY - tempCntY] = board[i][j];
            tempSet[squarePos.first + tempCntX][maxY - tempCntY] = pBoard[i][j];
            pBoard[i][j].clear();
            tempCntX++;
        }
        tempCntY++;
    }

    for (int i = squarePos.first; i <= squarePos.first + minWidth; i++)
    {
        for (int j = squarePos.second; j <= maxY; j++)
        {
            if (temp[i][j] > 0 && temp[i][j] != 100)
                board[i][j] = temp[i][j] - 1;
            else
                board[i][j] = temp[i][j];

            pBoard[i][j] = tempSet[i][j];
            for (auto t : pBoard[i][j]) p[t] = {i, j}; 
                
            

  

            if (board[i][j] == 100)
                exitX = i, exitY = j;

            // cout << board[i][j] << ' ';
        }
        // cout << '\n';
    }
}

void simulate()
{
    // 참가자 이동
    tryMove();

    if(cnt==0) return;

    // 회전
    rotate();
}

int main()
{
    //ios::sync_with_stdio(0);
    //cin.tie(0);

    cin >> n >> m >> k;
    cnt = m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            cin >> board[i][j];

    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;
        pBoard[x][y].insert(i);
        p.push_back({x, y});
        pStat.push_back(1);
    }

    cin >> exitX >> exitY;
    board[exitX][exitY] = 100;

    for (int i = 0; i < k && cnt > 0; i++)
    {
        simulate();
        //printPos();
    }

    cout << ans << '\n'
         << exitX << ' ' << exitY;

    return 0;
}