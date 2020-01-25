#include <cstdio>
#include <algorithm>

const int MAX_N = 1000, UNKNOWN = -1;
int a[MAX_N + 1], b[MAX_N + 1], sum_a[MAX_N + 1], sum_b[MAX_N + 1]; 
int cost[MAX_N + 1][MAX_N + 1];

int getCost(int n_a, int n_b)
{
    if (cost[n_a][n_b] != UNKNOWN)
        return cost[n_a][n_b];
    
    if (n_a == 1)
        return cost[n_a][n_b] = a[1] * sum_b[n_b];
    if (n_b == 1)
        return cost[n_a][n_b] = sum_a[n_a] * b[1];
    
    // Simple analysis show that to be a optimal solution, we have either pick 
    // (1, k) or (k, 1) - k 'A' parcel and 1 'B' parcel
    // Load (1, i)
    int minCost = 2147483647;
    for (int i = 1; i < n_b; i++) // avoid being fired: i != n_b unless n_a = 1, which is already handled
        minCost = std::min(minCost, 
            getCost(n_a - 1, n_b - i) + a[n_a] * (sum_b[n_b] - sum_b[n_b - i]) );
    
    // Load (i, 1). (1, 1) already considered so we start at 2
    for (int i = 2; i < n_a; i++)
        minCost = std::min(minCost, 
            getCost(n_a - i, n_b - 1) + b[n_b] * (sum_a[n_a] - sum_a[n_a - i]) );
    
    return cost[n_a][n_b] = minCost;
}

void testcase()
{
    int n;
    scanf("%d", &n);

    for (int i = n; i >= 1; i--) // inpute & reverse a
        scanf("%d", &a[i]);
    for (int i = n; i >= 1; i--)
        scanf("%d", &b[i]);

    for (int i = 1; i <= n; i++) 
    {
        a[i]--; // S_a - K_a = (a_i - 1)(a_i+1 - 1)...(a_j - 1)
        b[i]--;
        sum_a[i] = sum_a[i - 1] + a[i]; // precompute partial sum
        sum_b[i] = sum_b[i - 1] + b[i];
    }

    // Initialize
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            cost[i][j] = UNKNOWN;
    
    getCost(n, n);
    printf("%d\n", cost[n][n]);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}