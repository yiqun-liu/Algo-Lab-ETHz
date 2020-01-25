#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>

using std::vector;

void testcase()
{
    int nFriends, nBoxes;
    scanf("%d%d", &nFriends, &nBoxes);

    vector<int> strengths, weights;
    strengths.reserve(nFriends);
    weights.reserve(nBoxes);

    int maxStrength = 0;
    for (int i = 0; i < nFriends; i++)
    {
        int strength;
        scanf("%d", &strength);
        strengths.push_back(strength);
        maxStrength = std::max(maxStrength, strength);
    }

    int maxWeight = 0;
    for (int i = 0; i < nBoxes; i++)
    {
        int weight;
        scanf("%d", &weight);
        weights.push_back(weight);
        maxWeight = std::max(maxWeight, weight);
    }

    if (maxWeight > maxStrength)
    {
        printf("impossible\n");
        return;
    }

    // Idea: minimize maximum boxes assigned to all friends
    std::sort(strengths.begin(), strengths.end());
    std::sort(weights.begin(), weights.end());

    long long nEmpty = 0; // nEmpty: free list.  CAUTION: possibility to overflow!!!
    int maxRounds = 0, nAssigned = 0, toAssign = nBoxes;
    for (int i = 1; toAssign && i <= nFriends; i++) // number of friends under discussion
    {
        int iFriend = nFriends - i; // reverse order
        int round = 0; // boxes we must assign to "sb at strong as iFriend" (index: iF, iF+1, ... ,nF -1)
        
        if (iFriend == 0) // the last one
        {
            round = toAssign;
            toAssign = 0;
        }
        else
            while (toAssign > 0 && weights[toAssign - 1] > strengths[iFriend - 1]) // BIN SEARCH?
            {
                round++;
                toAssign--;
            }
        nAssigned += round;

        // without increasing maxRounds, iFriend carries at most maxRounds boxes
        if (round <= maxRounds + nEmpty)
            nEmpty -= (round - maxRounds);
        else // we must enlarge maxRounds
        {
            maxRounds = (int)std::ceil(1.0 * nAssigned / i); // CAUTION: 1.0 is necessary !!!
            nEmpty = i * maxRounds - nAssigned;
        }
    }
    
    printf("%d\n", 3 * maxRounds - 1);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}