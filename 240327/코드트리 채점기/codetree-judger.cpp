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
unordered_set<string> gradingUrl;
priority_queue<int, vector<int>, greater<int>> machine_pq;
unordered_map<string, pair<int, int>> endMap;
unordered_map<string, pair<int, int>> readyQueue; // 도메인, {우선순위, id}
unordered_map<string, int> readyUrl;              // url, id
unordered_map<string, priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>>> readyDomain;

void request(int t, int p, string url) // 200 큐에 추가
{
    if (readyUrl.find(url) != readyUrl.end()) // url이 큐에 존재
        return;

    int idx = url.find('/');
    string d = url.substr(0, idx);
    int id = stoi(url.substr(idx + 1));
    
    cnt++;
    readyUrl[url] = id;
    readyDomain[d].push({p, t, id}); // 도메인을 기준으로 우선순위, 시간, id 저장
    tie(p, t, id) = readyDomain[d].top();
    readyQueue[d] = { p, id }; // 우선순위가 높다면 레디큐 갱신
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

    int minP = 50001;
    int minT = 50001;
    int id,p;
    string url;
    string d;
    for (auto r : readyQueue)
    { // 우선순위가 높은 task 찾기
        d = r.first;
        p = r.second.first;
        if (gradingUrl.find(d) != gradingUrl.end())
            continue; // 채점중인 도메인
        if (endMap.find(d) != endMap.end())
        {
            int st = endMap[d].first;
            int en = endMap[d].second;
            if (en != -1 && t < st + 3 * (en - st))
                continue; // 부적절한 채점
        }

        if (minP > p || (minP == p && minT > get<1>(readyDomain[d].top())))
        {
            id = r.second.second;
            url = d + '/' + to_string(id);
            minP = p;
            minT = get<1>(readyDomain[d].top());
            is_find = true;
        }
    }

    if (is_find)
    { // 채점 시작
        //cout << url << '\n';
       
        readyUrl.erase(url);
        readyQueue.erase(d);
        readyDomain[d].pop();
        gradingUrl.insert(d);
        if(!readyDomain.empty()) readyQueue[d] = {get<0>(readyDomain[d].top()), id}; // 도메인의 다음 우선순위에 해당하는 task의 {우선순위, id} 저장
        endMap[d] = {t, -1};
        machine_to_url[machine_pq.top()] = d;
        machineStat[machine_pq.top()] = true;
        machine_pq.pop();
        cnt--;
    }
}

void endGrade(int t, int id) // 400 채점 끝
{
    // id번 채점기의 채점 종료
    if(id>n || !machineStat[id]) return;

    string url = machine_to_url[id];
    machine_to_url[id] = "";
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
        cin >> query;
        if (query == 100)
        {
            string url;
            cin >> n >> url;
            init(url);
        }
        else if (query == 200)
        {
            string u;
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
        else if (query == 500)
        {
            cin >> t;
            printInfo(t);
        }
    }

    return 0;
}