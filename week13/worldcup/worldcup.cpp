/**
 * Reminder:
 * 1. using triangulation to make cheap judgement
 * 2. note the difference in unit (alcohol content)
 */

#include <cstdio>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using std::vector;

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

void testcase()
{
    int nSupplies, nDemands, nCircles;
    scanf("%d%d%d", &nSupplies, &nDemands, &nCircles);

    Program lp(CGAL::SMALLER, true, 0, false, 0); // lower bound: 0

    vector<K::Point_2> points;
    points.reserve(nSupplies + nDemands);

    // row: 
    // [0, nSupplies): supply constraints
    // [nSupplies, nSupplies+nDemands) demand constraints
    // [nSupplies+nDemands, nSupplies + nDemands + nDemands) alcohol content constraints
    const int baseDemand = nSupplies, baseContent = nSupplies + nDemands;
    
    int index = 0; // index = i * nSupplies + j
    for (int i = 0; i < nSupplies; i++) // warehouse
    {
        double x, y;
        int supply, alcohol;
        scanf("%lf%lf%d%d", &x, &y, &supply, &alcohol);
        points.push_back(K::Point_2(x, y));
        
        for (int j = 0; j < nDemands; j++) // stadium
        {
            lp.set_a(index, i, 1); // supply
            lp.set_a(index, baseDemand + j, 1); // demand
            lp.set_a(index, baseContent + j, alcohol); // content in percentage
            index++;
        }
        lp.set_b(i, supply); // supply
    }

    for(int i = 0; i < nDemands; i++)
    {
        double x, y;
        int demand, maxAlcohol;
        scanf("%lf%lf%d%d", &x, &y, &demand, &maxAlcohol);
        points.push_back(K::Point_2(x, y));

        lp.set_b(baseDemand + i, demand); // demand
        lp.set_b(baseContent + i, maxAlcohol * 100); // content in percentage

        lp.set_r(baseDemand + i, CGAL::EQUAL); // exact equal
    }

    vector<int> profits; // times 100
    profits.reserve(nSupplies * nDemands);
    for (int i = 0; i < nSupplies; i++)
        for (int j = 0; j < nDemands; j++)
        {
            int profit;
            scanf("%d", &profit);
            profits.push_back(profit * 100); // avoid ratio
        }
    
    Triangulation t;
    t.insert(points.begin(), points.end());

    vector<bool> inCircles(nSupplies + nDemands);
    for (int i = 0; i < nCircles; i++)
    {
        double x, y, r;
        scanf("%lf%lf%lf", &x, &y, &r);

        const double r_2 = r * r;
        K::Point_2 center(x, y);
        if (CGAL::squared_distance( t.nearest_vertex( center )->point(), center ) > r_2 )
            continue; // no influence
        
        for (int j = 0; j < nSupplies + nDemands; j++)
            inCircles[j] = CGAL::squared_distance(points[j], center) <= r_2;

        index = 0;
        for (int j = 0; j < nSupplies; j++)
            for (int k = 0; k < nDemands; k++)
            {
                if ( inCircles[j] != inCircles[nSupplies + k] ) // cross line
                    profits[index] -= 1; // 0.01 * 100
                index++;
            }
    }

    index = 0;
    for (int i = 0; i < nSupplies; i++)
        for (int j = 0; j < nDemands; j++)
        {
            if ( profits[index] ) // cross line
                lp.set_c( index, -profits[index] );
            index++;
        }
    
    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.is_optimal())
        printf("%d\n",  int(std::floor(CGAL::to_double(s.objective_value()) / -100.0)) );
    else
        printf("RIOT!\n");
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}