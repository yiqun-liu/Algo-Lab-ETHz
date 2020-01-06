#include <cstdio>

const int MAX_N = 100000;
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
    
    for (int i = 0; i < n; i++)
        scanf("%d", &v[i]);
    
    int i = 0, j = 0, l = 0, r = 0;
    int sum = v[0], minDiff = absDiff(k, sum);

    while (j < n && minDiff)
    {
        // Slide the window
        if (sum < k || i == j)
        {
            if (j + 1 < n)
                sum += v[++j];
            else
                break;
        }
        else // actual condition: sum > k && i < j. We use else to ensure it always 'slides'.
            sum -= v[i++];
        
        // Get Min
        int diff = absDiff(k, sum);
        if (diff < minDiff)
        {
            l = i;
            r = j;
            minDiff = diff;
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