#include <cstdio>
#include <algorithm>

const int MAX_N = 1000, UNKNOWN = -1;
int a[MAX_N + 1], b[MAX_N + 1], cost[MAX_N + 1][MAX_N + 1];

void initCost(int n) // init cost where 
{
    // Initialize
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            cost[i][j] = UNKNOWN;
    
    int sum_a = 0, sum_b = 0;
    for (int i = 1; i <= n; i++) // accelerate boundary cases
    {
        sum_a += a[i];
        sum_b += b[i];
        cost[1][i] = a[1] * sum_b;
        cost[i][1] = b[1] * sum_a;
    }
}

int getCost(int n_a, int n_b)
{
    if (cost[n_a][n_b] != UNKNOWN)
        return cost[n_a][n_b];
    
    int pick_a = getCost(n_a - 1, n_b); // for (k, 1), k > 1
    int pick_b = getCost(n_a, n_b - 1); // for (1, k), k > 1
    int pick_both = getCost(n_a - 1, n_b - 1); // for (1, 1)
    return cost[n_a][n_b] = a[n_a] * b[n_b] + std::min( pick_both, std::min( pick_a, pick_b ));
}

void testcase()
{
    int n;
    scanf("%d", &n);

    for (int i = n; i >= 1; i--) // inpute & reverse a
    {
        scanf("%d", &a[i]);
        a[i]--; // S_a - K_a = (a_i - 1)(a_i+1 - 1)...(a_j - 1)
    }
    for (int i = n; i >= 1; i--)
    {
        scanf("%d", &b[i]);
        b[i]--;
    }
    initCost(n);
    printf("%d\n", getCost(n, n));
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}

/**
 * I got core idea of this n^2 solution from Moodle Forum.
 * In my understandings, the key message of this problem is to reorganize the mathematical form
 * In this case:
 * 1. optimal solutions contains exclusively "loading" with one item from 'A'/'B' and k item(s) from the other
 * 2. a * (b1 + b2 + ... bn) = a * b1 + a * b2 + ... a * bn. (product of sum --> sum of product)
 *    This makes one state rely on less states
 */
