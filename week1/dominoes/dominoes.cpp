#include <cstdio>

void testcase()
{
    int n;
    scanf("%d", &n);

    int cover = 2; // least int which is greater than 1
    for (int i = 1; i <= n; i++)
    {
        int h;
        scanf("%d", &h);
        
        // 1st cond: will this domino be toppled? 
        if (i < cover && i + h > cover)
            cover = i + h;
    }
    
    if (cover > n) // normalize
        cover = n + 1;
    printf("%d\n", cover - 1);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}