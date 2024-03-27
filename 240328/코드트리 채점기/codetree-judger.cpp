#include <iostream>
#include <queue>
#include <unordered_set>
#include <unordered_map>

#include <tuple>
using namespace std;

int q, n, cnt;
string machine_to_url[50001];
bool machineStat[50001];
vector<int> grading_machine;
unordered_set<string> readyUrl;                   
priority_queue<int, vector<int>, greater<int>> machine_pq;
unordered_map<string, pair<int, int>> endMap;
unordered_map<string, priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>>> readyQueue;

void request(int t, int p, string url) // 200 큐에 추가
{
    if (!readyUrl.empty() && readyUrl.find(url) != readyUrl.end()) // url이 큐에 존재
        return;

    int idx = url.find('/');
    string d = url.substr(0, idx);
    int id = stoi(url.substr(idx + 1));

    cnt++;
    readyUrl.insert(url);
    readyQueue[d].push({p, t, id}); // 도메인을 기준으로 우선순위, 시간, id 저장
}

void init(string url)
{
    for (int i = 1; i <= n; i++)
        machine_pq.push(i);
    request(0, 1, url);
}

void tryGrade(int t) // 300 채점 시도
{
    if (cnt == 0)
        return;
    if (machine_pq.empty())
        return; // 채점기 없으면 채점 X

    bool is_find = false;

    int minP = n+1;
    int minT = 1000001;
    int id;
    string url, d;

    for (auto r : readyQueue)
    { // 우선순위가 높은 task 찾기
        if(r.second.empty()) continue;
        string cur_d = r.first;
        int cur_p, cur_t, cur_id;
        tie(cur_p, cur_t, cur_id) = r.second.top();

        if (!endMap.empty() && endMap.find(cur_d) != endMap.end())
        {
            int st = endMap[cur_d].first;
            int en = endMap[cur_d].second;
            if (en == -1 || t < st + 3 * (en - st))
                continue; // 부적절한 채점
        }

        if (minP > cur_p || (minP == cur_p && minT > cur_t))
        {
            d = cur_d;
            id = cur_id;
            minP = cur_p;
            minT = cur_t;
            url = d + '/' + to_string(id);
            is_find = true;
        }
    }

    if (is_find)
    { // 채점 시작
        readyUrl.erase(url);
        readyQueue[d].pop();
        endMap[d] = {t, -1};
        machine_to_url[machine_pq.top()] = d;
        machineStat[machine_pq.top()] = true;
        machine_pq.pop();
        cnt--;
    }
}

void endGrade(int t, int id)
{
    if (!machineStat[id]) // 채점기가 채점 중이 아님
        return;

    machineStat[id] = false;
    string d = machine_to_url[id];
    endMap[d].second = t;
    machine_pq.push(id);
}

void printInfo(int t)
{
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
        cin >> query;
        if (query == 100)
        {
            string url;
            cin >> n >> url;
            init(url);
        }
        else if (query == 200)
        {
            string url;
            cin >> t >> p >> url;
            request(t, p, url);
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
        else if (query == 500)
        {
            cin >> t;
            printInfo(t);
        }
    }

    return 0;
}