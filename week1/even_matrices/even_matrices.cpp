#include <cstdio>

// Adapted from "even pairs" problem
int calcEvenPairs(int n, const int* x)
{
    int lastOdd, lastEven, odd, even;
    lastOdd = x[0] & 1;
    lastEven = !lastOdd;
    
    int evenPairs = lastEven;
    for (int i = 1; i < n; i++)
    {
        bool isOdd = x[i] & 1;
        odd  = isOdd ? lastEven + 1 : lastOdd;
        even = isOdd ? lastOdd : lastEven + 1;

        lastOdd = odd;
        lastEven = even;
        evenPairs += even;
    }

    return evenPairs;
}

const int MAX_N = 200;
// `bool` type suffices, I used `int` only for convinience
//  pSum: partial sum along column dimension, usage: pSum[r2][c] - pSum[r1][c]
int pSum[MAX_N + 1][MAX_N]; // zero-padding on pSum[0][c]
int temp[MAX_N];

void testcase()
{
    int n;
    scanf("%d", &n);

    int element;
    for (int j = 0; j < n; j++) // pSum[0][*] left as 0
        scanf("%d", &pSum[1][j]);
    for (int i = 2; i <= n; i++)
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &element);
            pSum[i][j] = pSum[i - 1][j] + element;
        }
    
    // Iterate through all i1, i2 combinations. written in interval (i1, i2]
    int evenMatrices = 0;
    for (int i1 = 0; i1 < n; i1++)
        for (int i2 = i1 + 1; i2 <= n; i2++)
        {
            for (int j = 0; j < n; j++)
                temp[j] = pSum[i2][j] - pSum[i1][j];
            
            evenMatrices += calcEvenPairs(n, temp);
        }
    
    printf("%d\n", evenMatrices);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}