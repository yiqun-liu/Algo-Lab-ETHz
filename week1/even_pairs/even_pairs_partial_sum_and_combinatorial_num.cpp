#include <cstdio>

void testcase()
{
    int n;
    scanf("%d", &n);

    // pSum stands for partial sum, can be replaced by a even/odd boolean
    // nEven initialized to 1 because paritial sum to x[0] is 0
    int nOdd = 0, nEven = 1, x, pSum = 0;
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &x);
        pSum += x;
        if (pSum & 1)
            nOdd++;
        else
            nEven++;
    }
    
    // possible (i, j) pairs = C(nEven, 2) + C(nOdd, 2)
    int evenPairs = (nEven * (nEven - 1) + nOdd * (nOdd - 1)) / 2;
    printf("%d\n", evenPairs);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}