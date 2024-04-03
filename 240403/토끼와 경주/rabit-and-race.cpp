#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;

struct Rabbit
{
    int x;
    int y;
    int pid;
    int d;
    int jCnt;
    int idx;
};

struct cmp
{
    bool operator()(Rabbit &a, Rabbit &b)
    {
        if (a.jCnt != b.jCnt)
            return a.jCnt > b.jCnt;
        if (a.x + a.y != b.x + b.y)
            return a.x + a.y > b.x + b.y;
        if (a.x != b.x)
            return a.x > b.x;
        if (a.y != b.y)
            return a.y > b.y;
        return a.pid > b.pid;
    }
};

int q, n, m, p, cnt, round;
int board[100001];
int selected[2001];
long long score[2001];
Rabbit rabbit[2001];
map<int, int> pidToRabbit;
priority_queue<Rabbit, vector<Rabbit>, cmp> pq;


bool cmp2(Rabbit &a, Rabbit &b)
{
    if (a.x + a.y != b.x + b.y)
        return a.x + a.y > b.x + b.y;
    if (a.x != b.x)
        return a.x > b.x;
    if (a.y != b.y)
        return a.y > b.y;
    return a.pid > b.pid;
};

bool cmp3(pair<int, int> a, pair<int, int> b)
{
    if (a.first + a.second != b.first + b.second)
        return a.first + a.second > b.first + b.second;
    if (a.first != b.first)
        return a.first > b.first;
    else
        return a.second > b.second;
}

void init(int pid, long long d)
{
    rabbit[cnt].x = 1;
    rabbit[cnt].y = 1;
    rabbit[cnt].pid = pid;
    rabbit[cnt].d = d;
    score[cnt] = 0;
    rabbit[cnt].jCnt = 0;
    rabbit[cnt].idx = cnt;
    pidToRabbit[pid] = cnt;
    pq.push(rabbit[cnt]);
    cnt++;
}

pair<int, int> move(int idx, int dir)
{
    int x = rabbit[idx].x;
    int y = rabbit[idx].y;
    int d = rabbit[idx].d;

     int mod;
    int nx = x, ny = y;
    if (dir == 0)
    { // 상
        mod = d % ((n - 1) * 2);
        nx -= mod;
    }
    else if (dir == 1)
    { // 하
        mod = d % ((n - 1) * 2);
        nx += mod;
    }
    else if (dir == 3)
    { // 좌
        mod = d % ((m - 1) * 2);
        ny -= mod;
    }
    else
    { // 우
        mod = d % ((m - 1) * 2);
        ny += mod;
    }

    while (!(nx >= 1 && nx <= n))
    {
        if (nx > n)
        {
            nx = 2 * n - nx;
        }
        else if (nx <= 0)
        {
            nx = 2 - nx;
        }
    }

    while (!(ny >= 1 && ny <= m))
    {
        if (ny > m)
        {
            ny = 2 * m - ny;
        }
        else if (ny <= 0)
        {
            ny = 2 - ny;
        }
    }

    return {nx, ny};
}

void startRace(int k, int s)
{
    round++;
    for (int i = 0; i < k; i++)
    {
        int idx = pq.top().idx;
        pq.pop();
        vector<pair<int, int>> v;
        v.clear();
        for (int dir = 0; dir < 4; dir++)
            v.push_back(move(idx, dir));
        sort(v.begin(), v.end(), cmp3);

        // 토끼 이동
        rabbit[idx].x = v[0].first;
        rabbit[idx].y = v[0].second;
        rabbit[idx].jCnt++;
        selected[idx] = round;

        // 나머지 토끼 점수 획득
        for (int j = 0; j < cnt; j++)
        {
            if (j == idx)
                continue;
            score[j] += (rabbit[idx].x + rabbit[idx].y);
        }
        pq.push(rabbit[idx]);
    }

    // 우선순위 높은 토끼에게 점수 S 더하기

    vector<Rabbit> v2;
    for (int i = 0; i < cnt; i++)
        v2.push_back(rabbit[i]);
    sort(v2.begin(), v2.end(), cmp2);

    for(auto v : v2){
        if(selected[v.idx] != round) continue;
        score[v.idx] += s;
        break;
    }
}

void changeD(int pid, long long l)
{
    int idx = pidToRabbit[pid];
    rabbit[idx].d *= l;
}

void printMaxScore()
{
    long long maxScore = -1;
    for (int i = 0; i < cnt; i++)
    {
        if (score[i] > maxScore)
            maxScore = score[i];
    }
    cout << maxScore;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> q;
    for (int i = 0; i < q; i++)
    {
        int query;
        cin >> query;
        if (query == 100)
        {
            cin >> n >> m >> p;
            for (int j = 0; j < p; j++)
            {
                int pid, d;
                cin >> pid >> d;
                init(pid, d);
            }
        }
        else if (query == 200)
        {
            int k, s;
            cin >> k >> s;
            startRace(k, s);
        }
        else if (query == 300)
        {
            int pid, l;
            cin >> pid >> l;
            changeD(pid, l);
        }
        else
        {
            printMaxScore();
        }
    }

    return 0;
}