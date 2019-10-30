/*
30 / 80
Name	Result	Points	CPU Time
1	test3	timelimit	0	1s
2	test2	timelimit	0	1s
3	test1	correct	30	0.011s
*/

#include <cstdio>
#include <vector>
#include <algorithm>

bool comparator(const std::pair<int, int>& p1, const std::pair<int, int>& p2)
{ return p1.second < p2.second; }

bool isInInterval(int s, const std::pair<int, int>& interval)
{
    if (interval.first <= interval.second)
        return interval.first <= s && s <= interval.second;
    else
        return interval.first <= s || s <= interval.second;
}

// !! 1-base to 0-base
inline int getOffset(int x, int x0, int m)
{
    if (x >= x0) return x - x0;
    else return x + m - x0;
}

// compute only the offset
void normalize(std::vector<std::pair<int, int> >& intv, int n, int m, int s)
{
    for (int i = 0; i < n; i++)
    {
        intv[i].first = getOffset(intv[i].first, s, m);
        intv[i].second = getOffset(intv[i].second, s, m);
    }
}

void testcase()
{
    int n, m;
    scanf("%d%d", &n, &m);
    
    // find s will take lots of time
    std::vector<bool> prot(m + 1, false);
    std::vector<std::pair<int, int> > intv;

    for (int i = 0; i < n; i++)
    {
        int a, b;
        scanf("%d%d", &a, &b);

        intv.push_back(std::make_pair(a, b));
        if (a <= b)
            for (int j = a; j <= b; j++) prot[j] = true; // [a, b]
        else
        {
            for (int j = a; j <= m; j++) prot[j] = true; // [a, m] &
            for (int j = 1; j <= b; j++) prot[j] = true; // [1, b]
        }
    }

    // find the intv that is protected by no one
    int s = -1;
    for (int i = 1; i <= m; i++)
        if (!prot[i])
        {
            s = i;
            break;
        }
    if (s == -1) printf("I give up!\n");

    normalize(intv, n, m, s);
    std::sort(intv.begin(), intv.end(), comparator);

    // greedy
    int count = 0, pos = 0, idx = 0;
    while (idx < n && intv[idx].second <= m)
    {        
        count++;
        pos = intv[idx].second + 1;

        if (pos == m - 1)
            break;
        
        // next best option
        idx++;
        while (idx < n && intv[idx].first < pos)
            idx++;
    }

    printf("%d\n", count);
}


int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}