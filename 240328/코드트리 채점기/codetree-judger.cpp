#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <tuple>
using namespace std;

int q, n, domainCnt, ans;
int machine_to_domainIdx[50001];
bool machineStat[50001];
//set<string> readyUrl;    
set<int> readyDomain[50001]; // readyDomain[도메인idx].insert(id)   
int en[50001];
int st[50001];
priority_queue<int, vector<int>, greater<int>> machine_pq;
// 도메인별 우선순위 큐
map<int, priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>>> readyQueue;
map<string, int> domainToIdx; // 도메인 문자열 -> int로 변환
set<int> domains[50001]; // 도메인별로 관리


void request(int t, int p, string url) // 200 큐에 추가
{
    int idx = url.find('/');
    string d = url.substr(0, idx);
    int id = stoi(url.substr(idx + 1));

    if(!domainToIdx[d]){ // 처음 나온 도메인
        domainCnt++;
        domainToIdx[d] = domainCnt;
    }
    int domainIdx = domainToIdx[d];

    if(readyDomain[domainIdx].find(id) != readyDomain[domainIdx].end()) return; // 이미 존재하는 url
    readyDomain[domainIdx].insert(id);
    readyQueue[domainIdx].push({p, t, id});
    ans++;
}

void init(string url)
{
    for (int i = 1; i <= n; i++)
        machine_pq.push(i);
    request(0, 1, url);
}

void tryGrade(int t) // 300 채점 시도
{
    if (ans == 0) // 채점 가능한 task가 없음
        return;
    if (machine_pq.empty())
        return; // 채점기 없음

    bool is_find = false;

    int minP = n+1;
    int minT = 1000001;
    int id, domainIdx;

    for (auto r : readyQueue)
    { // 우선순위가 높은 task 찾기

        //string cur_d = r;
        int curDomainIdx = r.first;
        int cur_p, cur_t, cur_id;
        tie(cur_p, cur_t, cur_id) = r.second.top();

        // 부적절한 채점
        if(en[curDomainIdx] == -1 ||  t < st[curDomainIdx] + 3 * (en[curDomainIdx] - st[curDomainIdx])) continue;

        if (minP > cur_p || (minP == cur_p && minT > cur_t))
        {
            domainIdx = curDomainIdx;
            id = cur_id;
            minP = cur_p;
            minT = cur_t;
            //url = d + '/' + to_string(id);
            is_find = true;
        }
    }

    if (is_find)
    { // 채점 시작
        readyDomain[domainIdx].erase(id);
        readyQueue[domainIdx].pop();
        if(readyQueue[domainIdx].empty()) readyQueue.erase(domainIdx);
        //endMap[d] = {t, -1};
        st[domainIdx] = t;
        en[domainIdx] = -1;
        machine_to_domainIdx[machine_pq.top()] = domainIdx;
        machineStat[machine_pq.top()] = true;
        machine_pq.pop();
        ans--;
    }
}

void endGrade(int t, int id)
{
    if (!machineStat[id]) // 채점기가 채점 중이 아님
        return;

    machineStat[id] = false;
    int domainIdx = machine_to_domainIdx[id];
    en[domainIdx] = t;
    machine_pq.push(id);
}

void printInfo(int t)
{
    cout << ans << '\n';
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