#include <iostream>
#include <vector>
#include <map>
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
    long long x;
    long long t;
};

multimap<string, Sushi> sushi;
long long l, q;
vector<Customer> customer;
long long customerCnt;

void Eat(long long t)
{
    for (auto c = customer.begin(); c != customer.end(); c++)
    {
        if (c->n == 0)
            continue;                           // 이미 떠난 손님
        auto iter = sushi.equal_range(c->name); // 손님에 해당하는 초밥들 탐색

        for (auto s = iter.first; s != iter.second && (s->second).name == c->name;)
        {
            long long stPos, customerPos, waitingTime, ansTime;
            customerPos = c->x;       // 손님 위치
            if (c->t < (s->second).t)
            { // 손님보다 초밥이 나중에 온 경우
                //cout << t << "?";
                stPos = (s->second).x;
                //stPos -= (c->t - (s->second).t);
                waitingTime = t - (s->second).t;        // 손님이 기다린 시간
                ansTime = customerPos - stPos; // 초밥이 손님 앞까지 걸리는 시간
                //cout << waitingTime << ' ' << ansTime << '\n';
            }
            else
            {
                stPos = (c->t - (s->second).t + (s->second).x) % l; // 손님 도착 시 초밥 위치
                waitingTime = t - c->t;        // 손님이 기다린 시간
                ansTime = customerPos - stPos; // 초밥이 손님 앞까지 걸리는 시간
            }
            
            //stPos = (c->t - (s->second).t + (s->second).x) % l; // 손님 도착 시 초밥 위치
            
            if (ansTime < 0)
                ansTime += l;

            if (waitingTime >= ansTime)
            { // 초밥 먹기
                // 초밥을 만난 경우 or 초밥을 이미 지나친 경우
                // cout<< t << ' ' << waitingTime << ' ' << ansTime << '\n';
                s = sushi.erase(s);
                c->n--;

                if (c->n == 0)
                {
                    customerCnt--;
                    break;
                }
            }
            else
            {
                s++;
            }
        }
    }
}
void Cook(long long t, long long x, string name)
{
    // 시각 t에 위치x앞에 있는 벨트에 name이 부착된 초밥을 올려놓음
    // t에 초밥 회전이 일어난 직후에 발생
    // 같은 위치에 여러 초밥이 올라갈 수 있음
    // 같은 name의 초밥이 같은 위치에 올라갈 수 있음
    Sushi s;
    s.name = name;
    s.x = x;
    s.t = t;
    sushi.insert({name, s});
    //Eat(t);
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
    customer.push_back(c);

   // Eat(t);
}

void Print(long long t)
{
    Eat(t);
    // 초밥 회전 -> 손님이 먹음 -> 촬영
    // 현재 사람 수와 남은 초밥 수 출력

    cout << customerCnt << ' ' << sushi.size() << '\n';
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