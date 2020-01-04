#include <cstdio>

const int MAX_N = 50000;

// arrays are not necessary
int x[MAX_N];
int odd[MAX_N], even[MAX_N];

void testcase()
{
    int n;
    
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", &x[i]);
    
    odd[0] = x[0] & 1;
    even[0] = !odd[0];
    
    int evenPairs = even[0];
    for (int i = 1; i < n; i++)
    {
        if (x[i] & 1)
        {
            odd[i] = even[i - 1] + 1;
            even[i] = odd[i - 1];
        }
        else
        {
            odd[i] = odd[i - 1];
            even[i] = even[i - 1] + 1;
        }
        evenPairs += even[i];
    }

    printf("%d\n", evenPairs);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}