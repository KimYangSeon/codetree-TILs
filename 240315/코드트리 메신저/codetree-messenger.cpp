#include <iostream>
using namespace std;

int n, q;
int parents[100001];
int authority[100001];
bool isOff[100001];
int cnt[100001];
int upcnt[100001][21]; // n번 노드가 위로 올릴 수 있는 알림 수 (권한 숫자 별로 셈)

void Init()
{
    for (int i = 1; i <= n; i++)
    {
        int cur = i;
        int a = authority[i];
        upcnt[cur][a]++;
        while (parents[cur] != 0 && a > 0)
        {
            cur = parents[cur];
            cnt[cur]++;
            a--;
            upcnt[cur][a]++;
        }
    }
}

void Change(int c)
{ // 알림망 설정 ON / OFF
    // c번 채팅방의 알림 설정을 반대로 변경
    if (isOff[c])
    { // OFF -> ON
        int cur = parents[c];
        int depth = 1;
        while (cur != 0)
        {
            for (int i = depth; i <= 20; i++)
            {
                cnt[cur] += upcnt[c][i];
                if (i - depth > 0)
                    upcnt[cur][i - depth] += upcnt[c][i];
            }
            if (isOff[cur])
                break;
            cur = parents[cur];
            depth++;
        }
    }
    else
    { // ON -> OFF
        int cur = parents[c];
        int depth = 1;
        while (cur != 0)
        {
            for (int i = depth; i <= 20; i++)
            {
                cnt[cur] -= upcnt[c][i];
                if (i - depth > 0)
                    upcnt[cur][i - depth] -= upcnt[c][i];
            }
            if (isOff[cur])
                break;
            cur = parents[cur];
            depth++;
        }
    }
    isOff[c] = !isOff[c];

    /*
    for(int i=1; i<=n; i++){
        for(int j=1; j<=20; j++){
            if(upcnt[i][j]>0) cout << i << ' ' << j << ' '<< upcnt[i][j] << '\n';
        }
    }
    */
}

void ChangeAutority(int c, int power)
{
    //  c번 채팅방의 권한 세기를 power로 변경
    if (power > 20)
        power = 20;
    int a = authority[c];
    authority[c] = power;
    upcnt[c][a]--;
    int cur = c;
    int depth = 1;
    if (isOff[cur]){
        upcnt[c][power]++;
        return;
    }
        
    while (parents[cur] != 0)
    {
        cur = parents[cur];
        if (isOff[cur])
            break;

        if (a - depth >= 0)
            cnt[cur]--;
        if (a - depth > 0)
            upcnt[cur][a - depth]--;

        depth++;
    }

    upcnt[c][power]++;
    cur = c;
    depth=1;
    while (parents[cur] != 0)
    {
        cur = parents[cur];
        if (isOff[cur])
            break;

        if (power - depth >= 0)
            cnt[cur]++;
        if (power - depth > 0)
            upcnt[cur][power - depth]++;

        depth++;
    }
}

void ChangeParent(int c1, int c2)
{
    bool before_isOff1 = isOff[c1];
    bool before_isOff2 = isOff[c2];

    if (!isOff[c1])
        Change(c1);
    if (!isOff[c2])
        Change(c2);

    swap(parents[c1], parents[c2]);

    if (!before_isOff1)
        Change(c1);
    if (!before_isOff2)
        Change(c2);
}

void Print(int c)
{
    cout << cnt[c] << '\n';
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> q;
    for (int i = 0; i < q; i++)
    {
        int query;
        cin >> query;
        if (query == 100)
        {
            for (int j = 1; j <= n; j++)
                cin >> parents[j];
            for (int j = 1; j <= n; j++)
            {
                cin >> authority[j];
                if (authority[j] > 20)
                    authority[j] = 20;
            }

            Init();
        }
        else if (query == 200)
        {
            int c;
            cin >> c;
            Change(c);
        }
        else if (query == 300)
        {
            int c, power;
            cin >> c >> power;
            ChangeAutority(c, power);
        }
        else if (query == 400)
        {
            int c1, c2;
            cin >> c1 >> c2;
            ChangeParent(c1, c2);
        }
        else
        {
            int c;
            cin >> c;
            Print(c);
        }
    }

    return 0;
}