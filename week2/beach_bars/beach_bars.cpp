#include <cstdio>
#include <vector>
#include <algorithm>

void addLocation(std::vector<int>& v, int l, int r)
{
    int radius = (r - l) / 2;

    v.push_back(l + radius);
    if ((r - l) & 1) // interval length: odd number --> two midpoints
        v.push_back(l + radius + 1);
}

void testcase()
{
    int n;
    scanf("%d", &n);

    std::vector<int> x(n);
    for (int i = 0; i < n; i++)
        scanf("%d", &x[i]);
    
    std::sort(x.begin(), x.end());

    // Sliding window
    int maxCount = 0, minDist = 0, l = 0, r = 0;
    std::vector<int> center(1, 0);
    for (int i = 0, j = 0; i < n; i++)
    {
        if (j < i)
            j = i;
        
        while (j + 1 < n && x[j + 1] - x[i] <= 200)
            j++;
        
        int count = j - i + 1, dist = (x[j] - x[i] + 1) / 2;
        if (count > maxCount || count == maxCount && dist < minDist)
        {
            maxCount = count;
            minDist = dist;
            center.clear();
            addLocation(center, x[i], x[j]);
        }
        else if (count == maxCount && dist == minDist)
            addLocation(center, x[i], x[j]);
    }

    printf("%d %d\n%d", maxCount, minDist, center[0]);
    for (int i = 1; i < center.size(); i++)
        printf(" %d", center[i]);
    putchar('\n');
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}