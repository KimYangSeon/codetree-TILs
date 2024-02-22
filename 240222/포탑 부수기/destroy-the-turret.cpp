#include <iostream>
#include <queue>
#define X first
#define Y second
using namespace std;

int n, m, k, aliveCnt, t;
int board[11][11];
int attackTime[11][11];
int attackedTime[11][11];
int vis[11][11];
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};
pair<int, int> attacker;
pair<int, int> target;

void SelectAttacker()
{
    // 공격자: 공격력이 0이 아닌 가장 약한 포탑 => N+M만큼 공격력 증가됨
    // 공격력이 같다면 가장 최근에 공격한 포탑 선택
    // 그것도 같다면 행과 열의 합이 더 큰거
    // 또 같다면 열 값이 큰거
    int minAttack = 5001;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (board[i][j] == 0)
                continue;

            if (minAttack > board[i][j])
            {
                minAttack = board[i][j];
                attacker = {i, j};
            }
            else if (minAttack == board[i][j])
            {
                if (attackTime[i][j] > attackTime[attacker.X][attacker.Y])
                {
                    attacker = {i, j};
                }
                else if (attackTime[i][j] == attackTime[attacker.X][attacker.Y])
                {
                    if (i + j > attacker.X + attacker.Y)
                    {
                        attacker = {i, j};
                    }
                    else if (i + j == attacker.X + attacker.Y)
                    {
                        if (j > attacker.Y)
                        {
                            attacker = {i, j};
                        }
                    }
                }
            }
        }
    }

    board[attacker.X][attacker.Y] += (n + m);
    // cout<< "attacker: "  << attacker.X << ' ' << attacker.Y << '\n';
}

void SelectTarget()
{
    // 공격자를 제외한 가장 강한 포탑 선택
    // 공격력이 같다면 공격한지 가장 오래된 포탑 (0에 모두 공격한 경험이 있다고 가정)
    // 행과 열의 합이 가장 작은 포탑
    // 열 값이 가장 작은 포탑

    int maxAttack = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i == attacker.X && j == attacker.Y)
                continue;

            if (maxAttack < board[i][j])
            {
                maxAttack = board[i][j];
                target = {i, j};
            }
            else if (maxAttack == board[i][j])
            {
                if (attackTime[i][j] < attackTime[target.X][target.Y])
                {
                    target = {i, j};
                }
                else if (attackTime[i][j] == attackTime[target.X][target.Y])
                {
                    if (i + j < target.X + target.Y)
                    {
                        target = {i, j};
                    }
                    else if (i + j == target.X + target.Y)
                    {
                        if (j < target.Y)
                        {
                            target = {i, j};
                        }
                    }
                }
            }
        }
    }

    // cout << "target: " << target.X << ' ' << target.Y << '\n';
}

void Attack(int x, int y, int damage)
{
    attackedTime[x][y]=t;
    board[x][y] -= damage;
    if (board[x][y] < 0)
    {
        board[x][y] = 0;
        aliveCnt--;
    }
}

void Bomb()
{
    // 목표에 포탄을 던짐
    // 주위 8방향 포탑도 공격력/2만큼 피해
    // 가장자리 반대편에도 피해감
    int x = target.X;
    int y = target.Y;

    for(int i=x-1; i<=x+1; i++){
        for(int j=y-1; j<=y+1; j++){
            int tempX = (i + n)%n;
            int tempY = (j+m)%m;

            if(attacker.X == tempX && attacker.Y == tempY) continue;
            if(tempX == x && tempY == y){ // 목표
                Attack(tempX,tempY,board[attacker.X][attacker.Y]);
            }
            else{
                Attack(tempX,tempY,board[attacker.X][attacker.Y]/2);
            }
        }
    }
}

void Laser()
{
    // 목표까지 최단 경로로 공격
    // 경로가 존재하지 않으면 포탄 공격
    // 최단 경로가 2개 이상이면 우/하/좌/상 우선순위대로
    // 경로에 있는 포탑은 공격력의 절반만 공격받음
    // 가장자리에서 막힌 방향으로 진행하고자 한다면, 반대편으로 나옵니다

    queue<pair<int, int>> q;
    attackTime[attacker.X][attacker.Y] = t; // 공격 시간 갱신
    vis[attacker.X][attacker.Y] = t;        // 현재 공격 시간 방문 여부 표시

    q.push({attacker.X, attacker.Y});

    // 최단 경로를 구하고 이 경로에만 공격하기
    pair<int, int> pre[11][11];
    pre[attacker.X][attacker.Y] = {-1, -1};
    bool isFind = false;
    while (!q.empty() && !isFind)
    {
        auto cur = q.front();
        q.pop();

        for (int dir = 0; dir < 4; dir++)
        {
            int nx = cur.X + dx[dir];
            int ny = cur.Y + dy[dir];

            nx = (nx + n) % n;
            ny = (ny + m) % m;

            if (board[nx][ny] == 0)
                continue; // 벽
            if (vis[nx][ny] == t)
                continue; // 이미 방문함

            vis[nx][ny] = t;
            pre[nx][ny] = {cur.X, cur.Y};

            if (nx == target.X && ny == target.Y)
            { // 목표
                isFind = true;
                break;
            }

            q.push({nx, ny});
        }
    }

    if(!isFind) {
        Bomb();
        return;
    }

    int x = target.X, y = target.Y;
    Attack(x, y, board[attacker.X][attacker.Y]);

    while (!(pre[x][y].X == attacker.X && pre[x][y].Y == attacker.Y))
    {
        Attack(pre[x][y].X, pre[x][y].Y, board[attacker.X][attacker.Y] / 2);

        int tempX = pre[x][y].X;
        int tempY = pre[x][y].Y;
        x = tempX;
        y = tempY;
    }
}



void Simulation()
{
    // 포탑이 1개가 되면 즉시 중지
    if (aliveCnt <= 1)
        return; // 공격중일때도?

    //  공격자 선정
    SelectAttacker();

    // 목표 선정
    SelectTarget();

    // 레이저 공격(포탑 공격)
    Laser();
    // 포탑 정비
    // 공격자 아님 & 아무 피해도 입지 않은 포탑은 공격력이 1상승

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(attackedTime[i][j] != t && !(i==attacker.X && j==attacker.Y) && board[i][j]>0){
                board[i][j]++;
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m >> k;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> board[i][j];
            if (board[i][j] > 0)
                aliveCnt++;
        }
    }

    while (k--)
    {
        t++;
        Simulation();
    }
    // 가장 강한 포탑의 공격력 출력
    int ans = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            ans = max(board[i][j], ans);
        }
    }

    cout << ans;

    return 0;
}