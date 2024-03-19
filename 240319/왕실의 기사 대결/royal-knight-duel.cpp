#include <iostream>
#include <stack>
using namespace std;

int l, n, q;
int board[41][41];
int knights[41][41];
int hp[31];
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};
int sum[31];
bool isChecked[31];
pair<int, int> pos[31];
pair<int, int> hw[31];

void print_knights()
{

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < l; j++)
        {
            cout << knights[i][j] << ' ';
        }
        cout << '\n';
    }
}

bool Check(int num, int d)
{
    if (hp[num] <= 0)
        return true;
    // cout << num << ' ' << d << '\n';
    isChecked[num] = 1;
    int r = pos[num].first;
    int c = pos[num].second;
    int h = hw[num].first;
    int w = hw[num].second;

    if (d == 0)
    { // 위
        int nx = r + dx[d];
        if (nx < 0) // 벽 (가장자리)
            return false;

        for (int i = c; i < c + w; i++)
        {
            if (board[nx][i] == 2) // 벽
                return false;
        }

        for (int i = c; i < c + w; i++)
        {
            if (knights[nx][i] != 0 && !isChecked[knights[nx][i]])
            { // 다른 기사
                if (!Check(knights[nx][i], d))
                    return false;
            }
        }
    }
    else if (d == 1)
    { // 오른쪽
        int ny = c + w;
        if (ny >= l) // 벽 (가장자리)
            return false;

        for (int i = r; i < r + h; i++)
        {
            if (board[i][ny] == 2) // 벽
                return false;
        }

        for (int i = r; i < r + h; i++)
        {
            if (knights[i][ny] != 0 && !isChecked[knights[i][ny]])
            { // 다른 기사
                if (!Check(knights[i][ny], d))
                    return false;
            }
        }
    }
    else if (d == 2)
    { // 아래
        int nx = r + h;
        if (nx >= l) // 벽 (가장자리)
            return false;

        for (int i = c; i < c + w; i++)
        {
            if (board[nx][i] == 2) // 벽
                return false;
        }

        for (int i = c; i < c + w; i++)
        {
            if (knights[nx][i] != 0 && !isChecked[knights[nx][i]])
            { // 다른 기사
                if (!Check(knights[nx][i], d))
                    return false;
            }
        }
    }
    else
    { // 왼쪽
        int ny = c + dy[d];
        if (ny < 0) // 벽 (가장자리)
            return false;

        for (int i = r; i < r + h; i++)
        {
            if (board[i][ny] == 2) // 벽
                return false;
        }

        for (int i = r; i < r + h; i++)
        {
            if (knights[i][ny] != 0 && !isChecked[knights[i][ny]])
            { // 다른 기사
                if (!Check(knights[i][ny], d))
                    return false;
            }
        }
    }

    return true;
}

void GetDamage(int num)
{
    int r = pos[num].first;
    int c = pos[num].second;
    int h = hw[num].first;
    int w = hw[num].second;
    int damage = 0;
    for (int i = r; i < r + h; i++)
    {
        for (int j = c; j < c + w; j++)
        {
            damage += board[i][j];
        }
    }
    sum[num] += damage;
    hp[num] -= damage;
    if (hp[num] <= 0)
    {
        for (int i = r; i < r + h; i++)
        {
            for (int j = c; j < c + w; j++)
            {
                knights[i][j] = 0;
            }
        }
    }
}

void Move(int num, int d) // num번 기사가 d로 한 칸이동
{
    for (int i = 1; i <= n; i++)
        isChecked[i] = 0;

    // d: 위, 오른쪽, 아래쪽, 왼쪽
    if (hp[num] <= 0)
        return;

    if (!Check(num, d))
        return;

    for (int i = 1; i <= n; i++)
    {
        if (isChecked[i])
        {
            pos[i].first += dx[d];
            pos[i].second += dy[d];
        }
    }

    if (d == 0 || d == 3) // 위, 왼쪽
    {
        for (int i = 0; i < l; i++)
        {
            for (int j = 0; j < l; j++)
            {
                if (isChecked[knights[i][j]])
                { // 이동
                    knights[i + dx[d]][j + dy[d]] = knights[i][j];
                    knights[i][j] = 0;
                }
            }
        }
    }
    else if (d == 1 || d == 2) // 오른쪽, 아래
    {
        for (int i = l - 1; i >= 0; i--)
        {
            for (int j = l - 1; j >= 0; j--)
            {
                if (isChecked[knights[i][j]])
                { // 이동
                    knights[i + dx[d]][j + dy[d]] = knights[i][j];
                    knights[i][j] = 0;
                }
            }
        }
    }

    //print_knights();

    // 체력 감소
    for (int i = 1; i <= n; i++)
    {
        if (i != num && isChecked[i])
            GetDamage(i);
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> l >> n >> q;
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < l; j++)
        {
            cin >> board[i][j];
        }
    }
    for (int i = 1; i <= n; i++)
    {
        int r, c, h, w, k;
        cin >> r >> c >> h >> w >> k;
        r--, c--;
        pos[i] = {r, c};
        hw[i] = {h, w};
        hp[i] = k;
        for (int j = r; j < r + h; j++)
        {
            for (int k = c; k < c + w; k++)
            {
                knights[j][k] = i;
            }
        }
    }

    // print_knights();

    for (int i = 0; i < q; i++)
    {
        int j, d;
        cin >> j >> d;
        Move(j, d);
    }

    int ans = 0;

    for (int i = 1; i <= n; i++)
    {
        if (hp[i] > 0)
            ans += sum[i];
    }
    cout << ans;

    return 0;
}