#include <iostream>
#include <vector>
#include <map>
#include <set>
using namespace std;

int n, m;
map<int, int> idToBox;
set<int> beltSet[11];
int beltHead[11];
int beltTail[11];
int nxt[100001];
int pre[100001];
int w[100001];
int boxToId[100001];
int merged[11];

void init()
{
    int perBelt = n / m;
    int beltCnt = 0;
    int cnt = 0;
    for (int i = 0; i < m; i++)
        merged[i] = -1;
    // for (int i = 0; i < m; i++)
    // beltSize[i] = perBelt;
    for (int i = 0; i < n; i++)
    {
        int id;
        cin >> id;
        boxToId[i] = id;
        idToBox[id] = i;
        beltSet[beltCnt].insert(i);
        if (i % perBelt == 0)
        {
            beltHead[beltCnt] = i;
        }
        else if (i % perBelt == perBelt - 1)
        {
            beltTail[beltCnt++] = i;
        }
    }

    for (int i = 0; i < n; i++)
    {
        int weight;
        cin >> weight;
        w[i] = weight;
    }

    for (int i = 0; i < n; i++)
    {
        if (i % perBelt < perBelt - 1)
            nxt[i] = i+1;
        if (i % perBelt > 0)
            pre[i] = i-1;
    }

    for (int i = 0; i < n; i++)
    {
        if (i % perBelt < perBelt - 1)
            nxt[i] = i+1;
        else
            nxt[i] = -1;

        if (i % perBelt > 0)
            pre[i] = i-1;
        else
            pre[i] = -1;
    }
}

int PopBox(int belt, int idx, bool isErase)
{ // idx번 박스 제거 & 한칸씩 앞으로 내리기
    int id = boxToId[idx];
    if(isErase){
        idToBox.erase(id);
        beltSet[belt].erase(idx);
    }
    
    if (nxt[idx] != -1 && pre[idx] != -1)
    {
        int p = pre[idx];
        int nx = nxt[idx];
        nxt[p] = nx;
        pre[nx] = p;
    }
    else if (nxt[idx] != -1 && pre[idx] == -1)
    {
        int nx = nxt[idx];
        beltHead[belt] = nx;
        pre[nx] = -1;
    }
    else if (nxt[idx] == -1 && pre[idx] != -1)
    {
        int p = pre[idx];
        beltTail[belt] = p;
        nxt[p] = -1;
    }
    else
    {
        beltTail[belt] = -1;
        beltHead[belt] = -1;
    }

    nxt[idx] = -1;
    pre[idx] = -1;

    return idx;
}

void PushBack(int belt, int idx) // 뒤에 한개 push
{
    if (beltHead[belt] == -1) // 벨트가 빈 경우
    {
        beltHead[belt] = idx; // head 지정
    }
    else // 앞 상자와 연결
    {
        int p = beltTail[belt];
        pre[idx] = p;
        nxt[p] = idx;
    }

    beltTail[belt] = idx;
    beltSet[belt].insert(idx);
    
    //idToBox[boxToId[idx]] = idx;

   // cout << "push:"<< belt << ' ' << boxToId[idx] << '\n';
}

void BoxRemove(int w_max) // 앞에 있는 물건 하차
{
    long long sum = 0;
    for (int i = 0; i < m; i++)
    {
        if (beltHead[i] == -1)
            continue;

        if (w[beltHead[i]] <= w_max)
        {
            sum += w[beltHead[i]];
            PopBox(i, beltHead[i], true);
        }
        else
        {
            int idx = PopBox(i, beltHead[i], false);
            PushBack(i, idx);
        }
    }

    cout << sum << '\n';
}

void BoxRemove2(int r_id)
{ // 물건 제거
    if (idToBox.find(r_id) == idToBox.end())
    {
        cout << -1 << '\n';
        return;
    }
    int idx = idToBox[r_id];

    for (int i = 0; i < m; i++) // 벨트에서 물건 찾기
    {
        //if (beltHead[i] == -1)
        //    continue;

        if (beltSet[i].find(idx) != beltSet[i].end())
        { // 속한 벨트 번호 찾았음
            int belt = i;
            if (merged[belt] != -1)
                belt = merged[belt];
            PopBox(belt, idx, true);
            break;
        }
    }
    cout << r_id << '\n';
}

void MoveToHead(int belt, int idx)
{ // idx부터 끝까지 밸트 맨 앞으로 옮김
    int originHead = beltHead[belt];
    int originTail = beltTail[belt];
    if (originHead == idx)
        return;

    beltHead[belt] = idx;
    beltTail[belt] = pre[idx];
    nxt[originTail] = originHead;
    pre[originHead] = originTail;
    pre[idx] = -1;
    nxt[beltTail[belt]] = -1;
}

void FindBox(int f_id)
{ // 물건 확인
    if (idToBox.find(f_id) == idToBox.end())
    {
        cout << -1 << '\n';
        return;
    }
    int idx = idToBox[f_id];
    for (int i = 0; i < m; i++)
    {
        //cout << i << ' ';
        //if (beltHead[i] == -1)
        //    continue;

        //if(beltSet[i].find(idx) == beltSet[i].end()) cout << "false\n";

        if (beltSet[i].find(idx) != beltSet[i].end())
        {
            // 해당 상자 위에 있는 모든 상자를 전부 앞으로
            int belt = i;
            if (merged[belt] != -1)
                belt = merged[belt];
            //cout << belt + 1 << "?\n";
            MoveToHead(belt, idx);
            cout << belt + 1 << '\n';
            break;
        }
    }

}

void BeltRemove(int b_num)
{                            // 벨트 고장
    if (merged[b_num] != -1) // 이미 고장남
    {
        cout << -1 << '\n';
        return;
    }

    int cnt = 0;
    for (int i = b_num + 1; cnt < m - 1; i++, cnt++)
    {
        if (i >= m)
            i = 0;
        if (merged[i] != -1) // 이미 고장남
            continue;

        merged[b_num] = i; // b_num이 i로 옮겨짐
        //cout << b_num+1 << " -> "<< i+1 << '\n';

        for (int j = 0; j < m; j++)
        {
            if (merged[j] == b_num)
            {
                // b_num으로 합쳐졌던 벨트를 i로
                merged[j] = i;
                // cout <<j+1 << " -> "<< i+1 << '\n';
            }
        }

        if (beltHead[i] == -1) // i에 아무것도 없음
        {
            beltHead[i] = beltHead[b_num];
        }
        else
        {
            int originTail = beltTail[i];
            nxt[originTail] = beltHead[b_num];
            pre[beltHead[b_num]] = originTail;
        }
        beltTail[i] = beltTail[b_num];

        beltHead[b_num] = -1; // 고장난 벨트 비우기
        beltTail[b_num] = -1;

        break;
    }
    cout << b_num + 1 << '\n';
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
            init();
        }
        else if (query == 200)
        {
            int w_max;
            cin >> w_max;
            BoxRemove(w_max);
        }
        else if (query == 300)
        {
            int r_id;
            cin >> r_id;
            BoxRemove2(r_id);
        }
        else if (query == 400)
        {
            int f_id;
            cin >> f_id;
            FindBox(f_id);
        }
        else
        {
            int b_num;
            cin >> b_num;
            BeltRemove(b_num - 1);
        }

        /*
        for (int i = 0; i < m; i++)
        {
            if (merged[i] == -1)
            {
                //cout << i + 1 << ": " << boxToId[beltHead[i]]  << ' ' << boxToId[beltTail[i]] << '\n';
            }
        }
        */
        
    }

    return 0;
}