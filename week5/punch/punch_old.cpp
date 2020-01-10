/**
 * This is my first version of punch. Less readable but (little) faster.
 * The idea is almost the same, except that I sort berverage by cost/volume, and enable early stop
 * 
 * Early stop condition:
 * 0. Assume we process beverage in descending order (by the quotient: cost/volume)
 * 1. There is already a combination of beverage whose total volume is exactly `k`, and
 * 2. known optimal combinations' (c/v) is lower than the next beverage
 */

#include <cstdio>
#include <cstdlib>

const int MAXN = 10000, MAXK = 10000, MAXV = 10000;
const int MAXCOST = 100000000;

struct BeverageStruct
{
    int c;
    int v;
};
typedef BeverageStruct Bev;
Bev bev[MAXN];

int comparator(const void *p, const void *q)  
{ 
    int pc = ((Bev *)p)->c, pv = ((Bev *)p)->v; 
    int qc = ((Bev *)q)->c, qv = ((Bev *)q)->v;
    
    // first compare (c/v), then v
    if (pc * qv - qc * pv) // (pc / pv) < (qc / qv)
        return pc * qv - qc * pv;
    return pv - qv;
} 

struct ResultStruct
{
    int cost;
    int nBev;
};
typedef ResultStruct Res;
Res result[2][MAXK + MAXV];
bool taked[MAXK + MAXV];

bool isBetter(const Res& r1, const Res& r2)
{
    if (r1.cost < r2.cost)
        return true;
    if (r1.cost == r2.cost && r1.nBev > r2.nBev)
        return true;
    return false;
}

void testcase()
{
    int n, k;
    scanf("%d%d", &n, &k);

    int maxV = 0;
    for (int i = 0; i < n; i++)
    {
        scanf("%d%d", &bev[i].c, &bev[i].v);
        if (bev[i].v > maxV)
            maxV = bev[i].v;
    }
    qsort(bev, n, sizeof(bev[0]), comparator);

    result[0][0].cost = result[1][0].cost = 0;
    result[0][0].nBev = result[1][0].nBev = 0;
    
    for (int i = 1; i < k + maxV; i++)
    {
        result[0][i].cost = result[1][i].cost = MAXCOST;
        result[0][i].nBev = result[1][i].nBev = MAXN;
    }
    
    int todo = k; // how many result[][i] for i in [1,k] remains unknown

    taked[0] = false;
    int cur, pre;
    // gradually enlarge the beverage type we are able to use
    for (int i = 0; i < n; i++)
    {
        // We can use beverage type: [0, i]
        cur = i % 2;
        pre = 1 - cur;
        int &c = bev[i].c, &v = bev[i].v;

        for (int j = 1; j < k + maxV; j++)
        {
            taked[j] = false;
            if (j < v || result[cur][j - v].cost == MAXCOST)
                result[cur][j] = result[pre][j];
            else
            {
                Res resNew;
                resNew.cost = result[cur][j - v].cost + c;
                resNew.nBev = result[cur][j - v].nBev;
                if (!taked[j - v]) // therefore we must take conservatively; not take for tie
                    resNew.nBev++;
                
                if (isBetter(resNew, result[pre][j]))
                {
                    taked[j] = true;
                    result[cur][j] = resNew;
                }
                else
                    result[cur][j] = result[pre][j];
            }
        }
        // no need to keep going
        if (todo == 0 && i + 1 < n && bev[i].c * bev[i+1].v != bev[i+1].c * bev[i].v)
            break;
    }

    int bestIdx = k;
    for (int i = k + 1; i < k + maxV; i++)
    {
        if (isBetter(result[cur][i], result[cur][bestIdx]))
            bestIdx = i;
    }

    printf("%d %d\n", result[cur][bestIdx].cost, result[cur][bestIdx].nBev);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}