#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>

using std::vector; using std::make_pair;
typedef std::pair<int, int> Segment;

const int MAX_N = 100000;
int partSum[MAX_N + 1]; // 1-based index

void testcase()
{
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);

    vector<Segment> targets;
    partSum[0] = 0;
    int left = 0;
    for (int i = 1; i <= n; i++) // sliding window: (left, i]
    {
        int value;
        scanf("%d", &value);
        partSum[i] = partSum[i - 1] + value;

        while (left < i && partSum[i] - partSum[left] > k)
            left++;
        
        if ( partSum[i] - partSum[left] == k)
            targets.push_back(make_pair(left, i)); // left is NOT contained
    }
    
    vector<int> v1(n), v2(n);
    vector<int>::iterator lastEnd = v1.begin() + 1; // [0] is the padding
    int maxValue = 0;
    for (int i = 0; i < targets.size(); i++) // base case: m=1
    {
        vector<int>::iterator thisEnd = v1.begin() + targets[i].second;
        std::fill( lastEnd, thisEnd, maxValue);
        lastEnd = thisEnd;

        int length = targets[i].second -  targets[i].first;
        maxValue = std::max(maxValue, length);
    }

    for (int i = 2; i <= m; i++) // number of segments kept
    {
        vector<int> &last = i & 1 ? v2 : v1;
        vector<int> &curr = i & 1 ? v1 : v2;

        maxValue = 0;
        vector<int>::iterator lastEnd = curr.begin() + 1; // [0] is the padding
        for (int j = i - 1; j < targets.size(); j++) // the segment
        {
            vector<int>::iterator thisEnd = curr.begin() + targets[j].second;
            std::fill( lastEnd, thisEnd, maxValue);
            lastEnd = thisEnd;

            int value = last[ targets[j].first ] ? 
                last[ targets[j].first ] + targets[j].second - targets[j].first : 
                0;
            maxValue = std::max(maxValue, value);
        }
    }

    if (maxValue == 0)
        printf("fail\n");
    else
        printf("%d\n", maxValue);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}