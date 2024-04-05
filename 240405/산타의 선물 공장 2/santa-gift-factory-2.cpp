#include <iostream>
#include <list>
using namespace std;

int n, m, q;
list<int> belt[100001];
int present_front[100001];
int present_back[100001];
int cnt[100001];
// int belt_back[100001];

void init(int b, int p)
{
    if (cnt[b] != 0) // 벨트 위에 선물이 한개 이상 있을 때
    {
        present_back[belt[b].back()] = p;  // 이전 선물의 뒤 = 새로 들어온 선물
        present_front[p] = belt[b].back(); // 새로 온 선물의 앞 = 이전 선물
    }

    belt[b].push_back(p);
    cnt[b]++; // 벨트의 선물 개수
    // belt_back[b] = p; // 벨트의 맨 뒤
}

void MoveAll(int src, int dst)
{ // src -> dst

    if (cnt[src] != 0)
    {
        int sp = belt[src].back();
        if (cnt[dst] > 0)
        {
            int dp = belt[dst].front();
            //cout << dp << "??????";
            present_back[sp] = dp;  // src 맨뒤 선물의 뒤 = dst 맨앞 선물
            present_front[dp] = sp; // dst 맨앞 선물의 앞 = src 맨뒤 선물
        }
    }
    belt[dst].splice(belt[dst].begin(), belt[src]);

    cnt[dst] += cnt[src];
    cnt[src] = 0;
    // belt_back[src] = 0;
    belt[src].clear();

    cout << cnt[dst] << '\n';
}

void ChangeFront(int src, int dst)
{
    int sp, dp;

    if (cnt[src] > 0)
    {
        sp = belt[src].front();
        belt[src].pop_front();
    }

    if (cnt[dst] > 0)
    {
        dp = belt[dst].front();
        belt[dst].pop_front();
    }

    if (cnt[src] > 0 && cnt[dst] > 0)
    {
        if (cnt[src] > 1 && cnt[dst] > 1) // dst, src에 선물이 2개 이상 있었음
        {
            present_front[belt[src].front()] = dp;
            present_front[belt[dst].front()] = sp;

            present_back[sp] = belt[dst].front();
            present_back[dp] = belt[src].front();
        }
        else if (cnt[src] > 1 && cnt[dst] == 1)
        { // dst에 한개밖에 없었을 때
            present_front[belt[src].front()] = dp;
            present_back[dp] = belt[src].front();
            present_back[sp] = 0;
        }
        else if (cnt[src] == 1 && cnt[dst] > 1)
        { // src에 한개밖에 없었을 때
            present_front[belt[dst].front()] = sp;
            present_back[sp] = belt[dst].front();
            present_back[dp] = 0;
        }

        belt[dst].push_front(sp);
        belt[src].push_front(dp);
    }
    else if (cnt[src] > 0 && cnt[dst] == 0)
    { // dst 벨트가 비어있던 경우
        cnt[src]--;
        cnt[dst]++;

        present_back[sp] = 0;
        if (cnt[src] > 1)
            present_front[belt[src].front()] = 0;

        belt[dst].push_front(sp);
    }
    else if (cnt[dst] > 0 && cnt[src] == 0)
    { // src 벨트가 비어있던 경우
        cnt[src]++;
        cnt[dst]--;

        present_back[dp] = 0;
        if (cnt[dst] > 1)
            present_front[belt[dst].front()] = 0;

        belt[src].push_front(dp);
    }

    cout << cnt[dst] << '\n';
}

void DivedePresent(int src, int dst)
{
    if (cnt[src] > 1)
    {
        auto iter = belt[src].begin();
        advance(iter, cnt[src] / 2-1); // n/2 번째 선물 가리킴

        int sp = *iter;
        //cout << cnt[src] / 2 << "?????\n";
        if (cnt[dst] > 0)
        {
            int dp = belt[dst].front();
            present_back[sp] = dp;  // src 선물의 뒤 = dst 앞 선물
            present_front[dp] = sp; // dst 선물의 앞 = src 선물
        }
        else{
            present_back[sp] = 0; // src 선물의 뒤에 아무것도 없음
        }

        advance(iter, 1);
        belt[dst].splice(belt[dst].begin(), belt[src], belt[src].begin(), iter);
        belt[src].erase(belt[src].begin(), iter);

        cnt[dst] += cnt[src] / 2;
        cnt[src] -= cnt[src] / 2;
        //cout << belt[dst].front()  << ' ' << belt[dst].back()<<"???";
        if (cnt[src] > 0)
            present_front[belt[src].front()] = 0; // src에 남아있는 맨앞 선물의 앞 선물 바꿈
        // belt_back[src] = 0;
        // belt[src].clear();
    }


    cout << cnt[dst] << '\n';
}

void PresentInfo(int p)
{
    int a = present_front[p] == 0 ? -1 : present_front[p];
    int b = present_back[p] == 0 ? -1 : present_back[p];
    //cout << p << " : " << a << ' ' << b << '\n';
    cout << a + 2 * b << '\n';
}

void BeltInfo(int b_num)
{
    int a = cnt[b_num] > 0 ? belt[b_num].front() : -1;
    int b = cnt[b_num] > 0 ? belt[b_num].back() : -1;
    //cout << b_num << " : " << a << ' ' << b << '\n';
    cout << a + 2 * b + 3 * cnt[b_num] << '\n';
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> q;
    while (q--)
    {
        int query;
        cin >> query;
        if (query == 100)
        {
            cin >> n >> m;
            for (int i = 1; i <= m; i++)
            {
                int b;
                cin >> b;
                init(b, i);
            }
        }
        else if (query == 200)
        {
            int m_src, m_dst;
            cin >> m_src >> m_dst;
            MoveAll(m_src, m_dst);
        }
        else if (query == 300)
        {
            int m_src, m_dst;
            cin >> m_src >> m_dst;
            ChangeFront(m_src, m_dst);
        }
        else if (query == 400)
        {
            int m_src, m_dst;
            cin >> m_src >> m_dst;
            DivedePresent(m_src,m_dst);
        }
        else if (query == 500)
        {
            int p_num;
            cin >> p_num;
            PresentInfo(p_num);
        }
        else if (query == 600)
        {
            int b_num;
            cin >> b_num;
            BeltInfo(b_num);
        }

        //for(int j=1; j<=m; j++) PresentInfo(j);
    }

    // for(int i=1; i<=n; i++) cout << cnt[i] << ' ';

    return 0;
}