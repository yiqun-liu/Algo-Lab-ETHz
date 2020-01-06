#include <cstdio>
#include <algorithm> // max & min

const int MAX_N = 2500;
int v[MAX_N], gain[MAX_N][MAX_N];

void testcase()
{
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        scanf("%d", &v[i]);
    
    // We always init the players last move, so that we need not to do index check during regular iterations
    int width; // interval width = number of remaining cards - 1
    if (n & 1) // player picks the last coins
    {
        for (int i = 0; i < n; i++)
            gain[i][i] = v[i];
        width = 1;
    }
    else // i.e. n >= 2
    {
        for (int i = 0; i < n - 1; i++)
            gain[i][i + 1] = std::max(v[i], v[i + 1]);
        width = 2;
    }
    
    while( width < n )
    {
        // opponent's turn
        for (int i = 0; i + width < n; i++)
            gain[i][i + width] = std::min(gain[i + 1][i + width], gain[i][i + width - 1]);
        width++;

        // player's turn
        for (int i = 0; i + width < n; i++)
            gain[i][i + width] = std::max(v[i] + gain[i + 1][i + width], v[i + width] +  gain[i][i + width - 1]);
        width++;
    }

    printf("%d\n", gain[0][n - 1]);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}