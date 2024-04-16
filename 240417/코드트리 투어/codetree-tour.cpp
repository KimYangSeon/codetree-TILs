#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

struct cmp
{
    bool operator()(pair<int, int> &a, pair<int, int> &b)
    {
        if (a.first != b.first)
            return a.first < b.first;
        return a.second > b.second;
    }
};

const int MAX_DIST = 10000 * 100 + 1;
int n, m;
int st;
vector<pair<int, int>> adj[2001];
int d[2001];
int p[30001];
int pr[30001];
int pd[30001];
priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> bestProduct;

void init()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    for (int i = 0; i < n; i++)
        d[i] = MAX_DIST;

    d[st] = 0;
    pq.push({d[st], st});
    while (!pq.empty())
    {
        auto cur = pq.top();
        pq.pop();
        int curCity = cur.second;
        int curDist = cur.first;

        if (d[curCity] != curDist)
            continue;

        for (auto nxt : adj[curCity])
        {
            int nCity = nxt.second;
            int nDist = nxt.first;
            if (d[nCity] <= d[curCity] + nDist)
                continue;
            d[nCity] = d[curCity] + nDist;
            pq.push({d[nCity], nCity});
        }
    }

    // for(int i=0; i<n; i++) cout << d[i] << ' ';
}

void Create(int id, int revenue, int dest)
{
    p[id] = 1;
    pr[id] = revenue;
    pd[id] = dest;
    if (d[dest] == MAX_DIST || revenue - d[dest] < 0)
    {
        bestProduct.push({-1, id});
       // cout << -1 << ' ' << id << '\n';
    }
    else
    {
        bestProduct.push({revenue - d[dest], id});
        //cout << revenue - d[dest] << ' ' << id << '\n';
    }
}

void Cancle(int id)
{
    p[id] = 0;
    // isCanceled[id] = 1;
}

void Sell()
{
    while (!bestProduct.empty())
    {
        auto cur = bestProduct.top();
        bestProduct.pop();
        int id = cur.second;
        int c = cur.first;
        if (p[id] == 0 || c < 0)
            continue;

        p[id]=0;
        cout << id << '\n';
        return;
    }

    cout << -1 << '\n';
}

void ChangeStart(int s)
{
    st = s;
    while (!bestProduct.empty())
        bestProduct.pop();
    init();

    for (int i = 1; i <= 30000; i++)
    {
        if (p[i] != 0)
            Create(i, pr[i], pd[i]);
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int q;
    cin >> q;
    while (q--)
    {
        int query;
        cin >> query;
        if (query == 100)
        {
            cin >> n >> m;
            for (int i = 0; i < m; i++)
            {
                int v, u, w;
                cin >> v >> u >> w;
                adj[v].push_back({w, u});
                adj[u].push_back({w, v});
            }
            init();
        }
        else if (query == 200)
        {
            int id, revenue, dest;
            cin >> id >> revenue >> dest;
            Create(id, revenue, dest);
        }
        else if (query == 300)
        {
            int id;
            cin >> id;
            Cancle(id);
        }
        else if (query == 400)
        {
            Sell();
        }
        else
        {
            int s;
            cin >> s;
            ChangeStart(s);
        }
    }

    return 0;
}