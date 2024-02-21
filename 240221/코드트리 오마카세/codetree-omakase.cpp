#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct Customer
{
    long long t;
    long long x;
    string name;
    long long n;
    long long exitTime;
};

struct Sushi
{
    string name;
    long long x;
    long long t;
};

map<string, Customer> customer;   // 이름, 손님 정보
map<string, vector<Sushi>> sushi; // 이름, 이름의 초밥 벡터
vector<pair<int, int>> ans;
long long l, q;

void Eat()
{
    for (auto c = customer.begin(); c != customer.end(); c++)
    {
        for (Sushi s : sushi[c->second.name])
        { // 손님과 이름이 같은 초밥들만 탐색

            long long stPos, customerPos, ansTime = 0;
            customerPos = c->second.x; // 손님 위치

            if (c->second.t < s.t) // 손님보다 초밥이 나중에 온 경우
            {
                ansTime = (customerPos - s.x + l) % l; // 손님이 기다려야 하는 시간
                ansTime += s.t;
            }
            else // 손님보다 초밥이 먼저 온 경우
            {
                stPos = (c->second.t - s.t + s.x) % l;   // 손님 도착 시 초밥 위치
                ansTime = (customerPos - stPos + l) % l; // 손님이 기다려야 하는 시간
                ansTime += c->second.t;
            }

            c->second.exitTime = max(c->second.exitTime, ansTime); // 손님이 나갈 시간
            // 손님의 나갈 시간의 최대값이 그 손님의 퇴장 시간이 됨
            //cout << ansTime << '\n';
            ans.push_back({ansTime, 101}); // 초밥 사라지는 쿼리
        }
    }
}
void Cook(long long t, long long x, string name)
{
    ans.push_back({t, 100});

    Sushi s;
    s.name = name;
    s.x = x;
    s.t = t;
    sushi[name].push_back(s);
}

void Enter(long long t, long long x, string name, long long n)
{
    ans.push_back({t, 200});

    Customer c;
    c.exitTime = 0;
    c.t = t;
    c.x = x;
    c.name = name;
    c.n = n;
    customer[name] = c;
}

void Print(long long t)
{
    ans.push_back({t, 300});
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> l >> q;
    for (int i = 0; i < q; i++)
    {
        int k, t, x, n;
        string name;
        cin >> k;
        if (k == 100)
        {
            cin >> t >> x >> name;
            Cook(t, x, name);
        }
        else if (k == 200)
        {
            cin >> t >> x >> name >> n;
            Enter(t, x, name, n);
        }
        else
        {
            cin >> t;
            Print(t);
        }
    }

    Eat();

    for (auto c = customer.begin(); c != customer.end(); c++)
    {
        // 손님 퇴장 처리
        ans.push_back({c->second.exitTime, 201});
    }

    sort(ans.begin(), ans.end());

    int sushiCnt = 0, customerCnt = 0;
    for (auto a : ans)
    {
        //cout << a.second << ' ';
        if (a.second == 100)
        {
            sushiCnt++;
        }
        else if (a.second == 101)
        {
            sushiCnt--;
        }
        else if (a.second == 200)
        {
            customerCnt++;
        }
        else if (a.second == 201)
        {
            customerCnt--;
        }
        else
        {
            cout << customerCnt << ' ' << sushiCnt << '\n';
        }
    }

    return 0;
}