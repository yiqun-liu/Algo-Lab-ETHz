#include <cstdio>

void testcase()
{
    int n;
    scanf("%d", &n);

    int x, sum = 0;
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &x);
        sum += x;
    }

    printf("%d\n", sum);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}