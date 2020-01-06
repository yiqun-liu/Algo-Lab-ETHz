#include <cstdio>
#include <queue>
// Another implementation with std::queue, should be slower but more space-efficient

const int MAX_K = 1 << 30;

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
    std::queue<int> Q;
    // Here I keep the right end as 'pivot', it's equally fine to keep the left end as 'pivot'
    // Or treat none as pivot
    for (int i = 0, j = 0; j < n; j++) // sliding window: closed interval [i, j]
    {
        int value;
        scanf("%d", &value);

        Q.push(value);
        sum += value;

        int sum_prime = sum - Q.front();
        int diff = absDiff(sum, k), diff_prime = absDiff(sum_prime, k);
        while (!Q.empty() && diff_prime < diff) // lt: maintain lexicographical order
        {
            sum = sum_prime;
            diff = diff_prime;
            i++;
            Q.pop();

            sum_prime = sum - Q.front();
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