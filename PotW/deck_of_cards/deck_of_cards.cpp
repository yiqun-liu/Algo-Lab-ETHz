#include <cstdio>

const int MAX_N = 100000;
const int MAX_K = 1 << 30;
int v[MAX_N];

int absDiff(int a, int b)
{
    int ret = a - b;
    if (ret < 0) return -ret;
    return ret;
}

void testcase()
{
    int n, k;
    scanf("%d%d", &n, &k);
    
    int sum = 0;
    int minDiff = MAX_K, l = 0, r = 0;
    // Here I keep the right end as 'pivot', it's equally fine to keep the left end as 'pivot'
    // Or treat none as pivot
    for (int i = 0, j = 0; j < n; j++) // sliding window: closed interval [i, j]
    {
        scanf("%d", &v[j]);
        sum += v[j];

        int sum_prime = sum - v[i];
        int diff = absDiff(sum, k), diff_prime = absDiff(sum_prime, k);
        while (i < j && diff_prime < diff) // lt: maintain lexicographical order
        {
            sum = sum_prime;
            diff = diff_prime;
            i++;

            sum_prime = sum - v[i];
            diff_prime = absDiff(sum_prime, k);
        }

        if (diff < minDiff) // Update min. strict lt: maintain lexicographical order
        {
            minDiff = diff;
            l = i;
            r = j;
        }
    }

    printf("%d %d\n", l, r);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}