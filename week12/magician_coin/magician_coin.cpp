#include <cstdio>
#include <algorithm>

const int MAX_TARGET = 1000, MAX_N_DAYS = 100;
const double UNKNOWN = -1.0;
double pWin[MAX_N_DAYS + 1][MAX_TARGET];
double p[MAX_N_DAYS + 1];

void init(int nDays, int funds, int target)
{
    for (int i = 1; i <= nDays; i++) // day
        for (int j = 1; j < target; j++)
            pWin[i][j] = UNKNOWN;
}

// funds: money you have at the start of the day (before gambling)
double getPWin(int day, int nDays, int funds, int target)
{
    if (funds >= target)
        return 1.0;
    if (day > nDays)
        return 0;
    if (pWin[day][funds] != UNKNOWN)
        return pWin[day][funds];
    
    const double &pHeads = p[day], pTails = 1 - p[day];
    for (int i = 0; i <= funds; i++) // amount to bet
    {
        double prob_win  = getPWin(day + 1, nDays, funds + i, target);
        double prob_lose = getPWin(day + 1, nDays, funds - i, target);
        double prob = pHeads * prob_win + pTails * prob_lose;

        pWin[day][funds] = std::max(pWin[day][funds], prob); // find optimal bet
    }
    return pWin[day][funds];
}

void testcase()
{
    int nDays, funds, target;
    scanf("%d%d%d", &nDays, &funds, &target);

    for (int i = 1; i <= nDays; i++)
        scanf("%lf", &p[i]);
    
    init(nDays, funds, target);
    printf("%.5f\n", getPWin(1, nDays, funds, target));
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}