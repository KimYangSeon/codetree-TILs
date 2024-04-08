#include <iostream>
#include <queue>
using namespace std;

priority_queue<int> gunPq[21][21];
int pBoard[21][21];
int score[31];
int playerDir[31];
int playerStat[31];
int gun[31];
pair<int, int> playerPos[31];
int n, m, k;
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};
int round;

bool isWin(int p1, int p2)
{

    int p1Stat = playerStat[p1] + gun[p1];
    int p2Stat = playerStat[p2] + gun[p2];

    if (p1Stat > p2Stat)
    {
        return true;
    }
    else if (p1Stat == p2Stat && playerStat[p1] > playerStat[p2])
    {
        return true;
    }
    return false;
}

void CheckGun(int p, int x, int y)
{
    if (gunPq[x][y].empty())
        return;

    if (gun[p] == 0)
    {
        gun[p] = gunPq[x][y].top();
        gunPq[x][y].pop();
    }
    else if (gun[p] < gunPq[x][y].top())
    {
        int temp = gun[p];          // 들고 있던 총
        gun[p] = gunPq[x][y].top(); // 새 총
        gunPq[x][y].pop();
        gunPq[x][y].push(temp);
    }

    // cout << "gun: " << p << ' ' << gun[p] << '\n';
}

void Move2(int p)
{
    auto cur = playerPos[p];
    int nx;
    int ny;
    // cout << p << " dir: " << playerDir[p] << '\n';
    for (int dir = 0; dir < 4; dir++)
    { // 오른쪽으로 90도씩 회전하여 빈 칸이 보이는 순간 이동
        playerDir[p] = (playerDir[p] + dir) % 4;
        nx = cur.first + dx[playerDir[p]];
        ny = cur.second + dy[playerDir[p]];
        if (nx < 0 || nx >= n || ny < 0 || ny >= n)
            continue;
        if (pBoard[nx][ny] != 0)
        {
            // cout << nx << ' ' << ny << ' ' << pBoard[nx][ny] << "?\n";
            continue;
        }

        break;
    }
    pBoard[cur.first][cur.second] = 0;
    pBoard[nx][ny] = p;
    playerPos[p] = {nx, ny};

    // cout << p << " lose move: " << nx << ' ' << ny << '\n';

    CheckGun(p, nx, ny);
}
void Move(int p)
{
    auto cur = playerPos[p];
    pBoard[cur.first][cur.second] = 0;
    int nx = cur.first + dx[playerDir[p]];
    int ny = cur.second + dy[playerDir[p]];
    if (nx < 0 || nx >= n || ny < 0 || ny >= n)
    {
        playerDir[p] = (playerDir[p] + 2) % 4;
        nx = cur.first + dx[playerDir[p]];
        ny = cur.second + dy[playerDir[p]];
    }

    // cout << "move: " << p << ' ' << nx << ' ' << ny << '\n';

    if (pBoard[nx][ny] == 0)
    { // 이동한 방향에 플레이어가 없다면
        // cout << "!";
        CheckGun(p, nx, ny);
        pBoard[nx][ny] = p;
        playerPos[p] = {nx, ny};
    }
    else
    { // 싸움
        int win, lose;
        if (isWin(p, pBoard[nx][ny]))
        {
            win = p;
            lose = pBoard[nx][ny];
        }
        else
        {
            win = pBoard[nx][ny];
            lose = p;
        }

        // cout << "win: " << win << '\n';

        score[win] += (playerStat[win] + gun[win]) - (playerStat[lose] + gun[lose]);
        // 원래 놓여있던 총이랑 진 사람이 내려놓은 총 중에 쎈거 골라야 함
        playerPos[lose] = {nx, ny};
        gunPq[nx][ny].push(gun[lose]); // 진 플레이어 총 내려놓음
        gun[lose] = 0;
        Move2(lose);

        pBoard[nx][ny] = p;
        playerPos[p] = {nx, ny};

        CheckGun(win, nx, ny);
    }
}

void Simulate()
{
    // 플레이어 이동
    for (int i = 1; i <= m; i++)
    {
        Move(i);
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m >> k;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int g;
            cin >> g;
            if (g != 0)
                gunPq[i][j].push(g);
        }
    }

    for (int i = 1; i <= m; i++)
    {
        int x, y, d, s;
        cin >> x >> y >> d >> s;
        pBoard[x - 1][y - 1] = i;
        playerDir[i] = d;
        playerStat[i] = s;
        playerPos[i] = {x - 1, y - 1};
    }
    while (round < k)
    {
        Simulate();
        round++;
    }
    for (int p = 1; p <= m; p++)
        cout << score[p] << ' ';

    cout << '\n';

    return 0;
}