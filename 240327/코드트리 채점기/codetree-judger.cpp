#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#include <stack>
#include <map>
using namespace std;

int q, n, cnt, minIdx;
string machine_to_url[50001];
vector<int> grading_machine;
set<string> readyUrl;
set<string> gradingUrl;
map<string, pair<int, int>> endMap;
priority_queue<int, vector<int>, greater<int>> machine_pq;
priority_queue<tuple<int, int, string>, vector<tuple<int, int, string>>, greater<tuple<int, int, string>>> pq;
// 우선순위, 들어온 시간, url

void request(int t, int p, string u) // 200 큐에 추가
{
    if (readyUrl.find(u) != readyUrl.end()){
       // cout << "exist: " << u << '\n';
        return;
    }
        
    cnt++;
    readyUrl.insert(u);
    //pq.push({p, t, u.substr(0, u.find('/'))});
    pq.push({p, t, u});
}

void init(string u0)
{
    for (int i = 1; i <= n; i++)
        machine_pq.push(i);
    request(0, 1, u0);
}

void tryGrade(int t) // 300 채점 시도
{
    // t초에 큐에서 우선순위가 높은 거 채점(우선순위 숫자가 작은거 & 큐에 먼저 온거)
    // 현재 채점중인 도메인이면 채점 X

    if(cnt==0) return;
    if (machine_pq.empty())
        return; // 채점기 없으면 채점 X

    stack<tuple<int, int, string>> temp;
    bool is_find = false;
    while (!pq.empty() && !is_find)
    {
        auto cur = pq.top();
        string url = get<2>(cur);
        string u = url.substr(0, url.find('/'));
        if (gradingUrl.find(u) != gradingUrl.end())
        { // 현재 채점중임
            temp.push(cur);
            pq.pop();
            continue;
        }
        else if (endMap.find(u) != endMap.end())
        { // 최근 채점 기록 중에서
            int st = endMap[u].first;
            int en = endMap[u].second;
            if (en != -1 && t < st + 3 * (en - st))
            { // t < start + 3 * gap
                temp.push(cur);
                pq.pop();
                continue;
            }
        }
        is_find = true;
    }

    if (is_find)
    {
        auto cur = pq.top();
        string url = get<2>(cur);
        string u = url.substr(0, url.find('/'));
        readyUrl.erase(url);
        gradingUrl.insert(u);
        endMap[u] = {t, -1};
        machine_to_url[machine_pq.top()] = u;
        machine_pq.pop();
        pq.pop();
        cnt--;
        //cout << "start: " << url << '\n';
    }

    while (!temp.empty())
    {
        pq.push(temp.top());
        temp.pop();
    }
}

void endGrade(int t, int id) // 400 채점 끝
{
    // id번 채점기의 채점 종료
    string url = machine_to_url[id];
    gradingUrl.erase(url);

    endMap[url].second = t;
    machine_pq.push(id);
}

void printInfo(int t) // 500
{
    // t초에 대기 큐 크기 출력
    cout << cnt << '\n';
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> q;

    for (int i = 0; i < q; i++)
    {
        int query, t, p, id;
        string url, u;
        cin >> query;
        if (query == 100)
        {
            cin >> n >> url;
            init(url);
        }
        else if (query == 200)
        {
            cin >> t >> p >> u;
            request(t, p, u);
        }
        else if (query == 300)
        {
            cin >> t;
            tryGrade(t);
        }
        else if (query == 400)
        {
            cin >> t >> id;
            endGrade(t, id);
        }
        else
        {
            cin >> t;
            printInfo(t);
        }
    }

    return 0;
}