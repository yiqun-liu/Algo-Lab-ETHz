/*
80 / 80
	Name	Result	Points	CPU Time
1	test3	correct	20	0.595s
2	test2	correct	30	0.652s
3	test1	correct	30	0.008s
 */

#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>

typedef std::pair<int, int> Interval;

// create or update counter
void increment(std::map<int, int>& diffArray, int key, int delta)
{
    std::map<int, int>::iterator it = diffArray.find(key);
    if (it == diffArray.end())
        diffArray.insert(std::make_pair(key, delta));
    else
        it->second += delta;
}

void getMinCoveredPos(int& minPos, int& minCover, const std::map<int, int>& diffArray, int m)
{
    if (diffArray.begin()->first != 1)
    {
        minPos = 1;
        minCover = 0;
        return;
    }

    minCover = diffArray.begin()->second;
    minPos = diffArray.begin()->first;

    int curCover = 0;
    for (std::map<int, int>::const_iterator it = diffArray.begin(); 
         it != diffArray.end() && it->first <= m; it++)
    {
        curCover += it->second; // computer "prefix sum"
        if (curCover < minCover)
        {
            minCover = curCover;
            minPos = it->first;
        }
    }
}

// sort by ending time
bool compRightEnd(const Interval& intv1, const Interval& intv2)
{ return intv1.second < intv2.second; }

bool isInside(int pos, int start, int end)
{
    if (start <= end)
        return start <= pos && pos <= end;
    else
        return start <= pos || pos <= end;
}

int greedy(int start, int end, int n, const std::vector<Interval>& intv, int firstIdx = 0)
{
    int count = 0, pos = start;
    
    for (int i = firstIdx; i < 2 * n && intv[i].second < end; i++)
    {
        // ignore inconsistent notaion (they've already been converted to the form we need)
        if (intv[i].first > intv[i].second) continue; 
        // compatible with already selected ones
        if (intv[i].first > pos)
        {
            pos = intv[i].second;
            count++;
        }
    }

    return count;
}

void testcase()
{
    int n, m;
    scanf("%d%d", &n, &m);
    std::vector<Interval> intv(n);
    std::map<int, int> diffArray;
    
    for (int i = 0; i < n; i++)
    {
        int a, b;
        scanf("%d%d", &a, &b);
        intv[i].first = a;
        intv[i].second = b;

        if (a <= b) // normal
        {
            // [a, b]
            increment(diffArray, a, 1);
            increment(diffArray, b + 1, -1);
        }
        else // cross boundary
        {
            // [a, m]
            increment(diffArray, a, 1);
            // [1, b]
            increment(diffArray, 1, 1);
            increment(diffArray, b + 1, -1);
        }
    }

    // now order of all intv are fixed; we refer jedis by index in this vector
    std::sort(intv.begin(), intv.end(), compRightEnd);

    // copy & expand, note the newly generated vector are still in order
    for (int i = 0; i < n; i++)
    {
        if (intv[i].first <= intv[i].second)
            intv.push_back(std::make_pair( intv[i].first + m,  intv[i].second + m ));
        else
            intv.push_back(std::make_pair( intv[i].first    ,  intv[i].second + m ));
    }

    // find the weak point (covered by <= 10 Jedi)
    int minPos, minCover;
    getMinCoveredPos(minPos, minCover, diffArray, m);

    // use binary search to skip irrelevant "jedi" efficiently
    std::vector<Interval>::iterator it;
    Interval temp = std::make_pair(0, minPos);
    it = std::lower_bound(intv.begin(), intv.end(), temp, compRightEnd);

    // FINAL STEP, run greedy algorithm in all possible cases, optimal one is one of them
    int start, end, maxCount, count;
    
    // case 1, not covered
    start = minPos;
    end = minPos + m;
    maxCount = greedy(start, end, n, intv, it - intv.begin());

    // case 2, covered by some "jedi"
    int i = it - intv.begin();
    for (int j = 0; j < minCover; j++)
    {
        // find the one that protects weak spot
        while (!isInside(minPos, intv[i].first, intv[i].second))
            i++;
        
        // pick this jedi, consider the remaining segments
        start = intv[i].second;
        end = intv[i].first <= intv[i].second ? intv[i].first + m : intv[i].first;
        count = 1 + greedy(start, end, n, intv, i); // this jedi + combination of others
        
        if (count > maxCount)
            maxCount = count;
        
        // Check next Jedi
        i++;
    }

    printf("%d\n", maxCount);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}