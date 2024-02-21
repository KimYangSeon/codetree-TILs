#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

struct Customer
{
    long long t;
    long long x;
    string name;
    long long n;
};

struct Sushi
{
    string name;
    bool eaten;
    long long x;
    long long t;
};

map<string, Customer> customer; // 이름, 손님 정보
map<string, vector<Sushi>> sushi; // 이름, 이름의 초밥 벡터

long long l, q;
long long customerCnt;
long long sushiCnt;

void Eat(long long t)
{
    for (auto c = customer.begin(); c != customer.end(); c++)
    {
        if (c->second.n == 0)
            continue;                           // 이미 떠난 손님
        //cout << t << ' ' << sushi[c->first].size() << ' ';
        for(Sushi &s : sushi[c->first]){ // 손님과 이름이 같은 초밥들만 탐색
            if(s.eaten) continue;
            //cout << "time: " <<  s.t << ' ';
            long long stPos, customerPos, waitingTime, ansTime;
            customerPos = c->second.x;       // 손님 위치

            if (c->second.t < s.t)
            { // 손님보다 초밥이 나중에 온 경우
                stPos = s.x;
                waitingTime = t - s.t;        // 손님이 기다린 시간
                ansTime = customerPos - stPos; // 초밥이 손님 앞까지 걸리는 시간
            }
            else
            {
                stPos = (c->second.t - s.t + s.x) % l; // 손님 도착 시 초밥 위치
                waitingTime = t - c->second.t;        // 손님이 기다린 시간
                ansTime = customerPos - stPos; // 초밥이 손님 앞까지 걸리는 시간
            }

            if (ansTime < 0)
                ansTime += l;

            if (waitingTime >= ansTime)
            { // 초밥 먹기
                //cout << t << ' ' << s.eaten << '\n';
                s.eaten = true;
                sushiCnt--;
                
                c->second.n--;
                if (c->second.n == 0)
                {
                    customerCnt--;
                    break;
                }
                
            }

        }
    }
}
void Cook(long long t, long long x, string name)
{
    sushiCnt++;
    Sushi s;
    s.name = name;
    s.x = x;
    s.t = t;
    s.eaten = false;
    sushi[name].push_back(s);
}

void Enter(long long t, long long x, string name, long long n)
{
    // 동일한 손님이 방문하는 경우는 없음
    // t에 x로 가서 name손님이 앉음
    // t에 초밥 회전이 일어난 직후에 발생
    // 위치 x로 오는 초밥들 중 자신의 name이 적힌 초밥 n개 먹고 떠남 (시각 t에 있으면 즉시 먹음)
    // 위치 x에 다른 사람은 없음
    // 초밥 먹을때 시간 소요 X
    customerCnt++;

    Customer c;
    c.t = t;
    c.x = x;
    c.name = name;
    c.n = n;
    customer[name] = c;
}

void Print(long long t)
{
    Eat(t);

    cout << customerCnt << ' ' << sushiCnt << '\n';
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    // 여기에 코드를 작성해주세요.
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

    return 0;
}