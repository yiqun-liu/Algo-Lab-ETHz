#include <cstdio>
#include <cstring>
#include <vector>

const int MAX_N = 50000, MAX_M = 50000, UNDEFINED = -1;
int fast[MAX_N + 1], slow[MAX_N + 1]; // number of rounds needed to reach the target according to strategies
std::vector<int> adjList[MAX_N + 1];

int getSlow(int pos, int target);

int getFast(int pos, int target)
{
    if (fast[pos] != UNDEFINED)
        return fast[pos];
    if (pos == target)
        return 0;
    
    fast[pos] = MAX_N;
    for (int i = 0; i < adjList[pos].size(); i++)
    {
        const int& to = adjList[pos][i];
        int rounds = getSlow(to, target) + 1;
        if (rounds < fast[pos])
            fast[pos] = rounds;
    }
    return fast[pos];
}

int getSlow(int pos, int target)
{
    if (slow[pos] != UNDEFINED)
        return slow[pos];
    if (pos == target)
        return 0;
    
    slow[pos] = 0;
    for (int i = 0; i < adjList[pos].size(); i++)
    {
        const int& to = adjList[pos][i];
        int rounds = getFast(to, target) + 1;
        if (rounds > slow[pos])
            slow[pos] = rounds;
    }
    return slow[pos];
}

void testcase()
{
    int n, m, r, b;
    scanf("%d%d%d%d", &n, &m, &r, &b);

    for (int i = 1; i <= n; i++)
        adjList[i].clear();
    for (int i = 0; i < m; i++)
    {
        int from, to;
        scanf("%d%d", &from, &to);
        adjList[from].push_back(to);
    }

    memset(fast, UNDEFINED, sizeof(int) * (n + 1));
    memset(slow, UNDEFINED, sizeof(int) * (n + 1));

    int red   = getFast(r, n);
    int black = getFast(b, n);

    int winner = red > black;
    if (red == black) // which color moves first depending on the rounds
        winner = (red & 1) ? 0 : 1;
    
    printf("%d\n", winner);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}