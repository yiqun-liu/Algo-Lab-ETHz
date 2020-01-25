// idea: range of |k| is small --> warshall
#include <cstdio>
#include <vector>
using std::vector;

void testcase()
{
    int n, remainder, k;
    scanf("%d%d%d", &n, &remainder, &k);
    
    // we need 2 to avoid counting one disks multiple times
    vector<bool> possible1(k, false), possible2(k, false);
    for (int i = 0; i < n; i++)
    {
        int height;
        scanf("%d", &height);
        height %= k;
        
        vector<bool> &curr = i & 1 ? possible1 : possible2;
        vector<bool> &last = i & 1 ? possible2 : possible1;
        if (last[remainder]) // already solved, just read in all inputs
            continue;
        
        // sparser structure is faster?
        for (int j = 0; j < k; j++)
            if (last[j])
            {
                curr[j] = true;// last must be a subset of curr
                curr[ (j + height) % k ] = true;
            }
        curr[ height ] = true;
    }
    if (possible1[remainder] || possible2[remainder])
        printf("yes\n");
    else
        printf("no\n");
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}