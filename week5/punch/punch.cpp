#include <cstdio>
#include <algorithm>

struct BevNode
{
    int cost;
    int volume;
};
typedef struct BevNode Bev; // berverage

struct CombNode
{
    int cost;
    int type;
};
typedef struct CombNode Comb; // combination of beverages 

const int MAX_N = 100, MAX_K = 10000, MAX_COST = 10000;
Bev bevs[MAX_N];
Comb combs1[MAX_K + 1], combs2[MAX_K + 1];

void testcase()
{
    int n, k;
    scanf("%d%d", &n, &k);

    for (int i = 0; i < n; i++)
        scanf("%d%d", &bevs[i].cost, &bevs[i].volume);
    
    // Initialize
    combs1[0].cost = combs1[0].type = 0; // base case
    for (int i = 1; i <= k; i++) // unknown: init to worst
    {
        combs1[i].cost = MAX_K * MAX_COST;
        combs1[i].type = 0;
    }
    
    // we need to keep track of beverage already been used, so we enrich our choices step by step
    // lastComb[]: best results with current beverage not in use; 
    // thisComb[]: best results with all available choices now; current beverage may in use
    for (int i = 0; i < n; i++) // beverage
    {
        Bev &bev = bevs[i];

        Comb* lastCombs = i & 1 ? combs2 : combs1;
        Comb* thisCombs = i & 1 ? combs1 : combs2;
        thisCombs[0].cost = thisCombs[0].type = 0;
        
        for (int j = 1; j <= k; j++)
        {
            thisCombs[j] = lastCombs[j]; // option 1: current bev not taken
            
            Comb taken;
            int base = std::max(0, j - bev.volume);
            
            taken.cost = lastCombs[base].cost + bev.cost;  // option 2: current beverage taken the first time
            taken.type = lastCombs[base].type + 1;
            if (thisCombs[j].cost > taken.cost || thisCombs[j].cost == taken.cost && thisCombs[j].type < taken.type)
                thisCombs[j] = taken;
            
            taken.cost = thisCombs[base].cost + bev.cost;  // option 3: current beverage taken again
            taken.type = thisCombs[base].type;
            if (thisCombs[j].cost > taken.cost || thisCombs[j].cost == taken.cost && thisCombs[j].type < taken.type)
                thisCombs[j] = taken;
        }
    }

    Comb* finalCombs = n & 1 ? combs2 : combs1;
    printf("%d %d\n", finalCombs[k].cost, finalCombs[k].type);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}