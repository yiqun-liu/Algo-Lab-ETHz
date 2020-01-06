#include <cstdio>
#include <cstring>
#include <algorithm>

const int MAX_N = 2500, UNDEFINIED = -1;
int v[MAX_N];
int gain[MAX_N][MAX_N];

// parameters: endpoints of closed interval
int opponent(int l, int r);

int player(int l, int r)
{
    if (gain[l][r] != UNDEFINIED) // memoization
        return gain[l][r];
    if (l == r) // base case
        return gain[l][r] = v[l];
    
    int pickFirst = v[l] + opponent(l + 1, r);
    int pickLast  = v[r] + opponent(l, r - 1);
    return gain[l][r] = std::max(pickFirst, pickLast); // play optimally
}

int opponent(int l, int r)
{
    if (gain[l][r] != UNDEFINIED) // memoization
        return gain[l][r];
    if (l == r) // base case
        return gain[l][r] = 0;
    
    int pickFirst = player(l + 1, r);
    int pickLast  = player(l, r - 1);
    return gain[l][r] = std::min(pickFirst, pickLast); // gurantee in worst case
}

void testcase()
{
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        scanf("%d", &v[i]);
    
    memset(gain, UNDEFINIED, sizeof(int) * MAX_N * n);

    printf("%d\n", player(0, n - 1));
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}