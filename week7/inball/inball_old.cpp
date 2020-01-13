#include <cstdio>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// input & solver types
typedef int IT;
typedef CGAL::Gmpz ET;

// program & solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n)
{   
    int d;
    scanf("%d", &d);

    Program lp (CGAL::SMALLER, false, 0, false, 0);

    // if all constrains satisfied, distance is: d = ( -Sum_{aij * xj} + bi ) / Z
    // transform this equation we can get: Sum_{aij * xj} + Z * d = bi
    // r = min{d1, d2, ... dn}, so r <= d
    // so we can satisfy Sum_{aij * xj} <= bi by satisfying Sum_{aij * xj} + Z * r <= bi (r >= 0)
    
    for (int i = 0; i < n; i++)
    {
        double squareSum = 0;
        int cof; // elemetns of matrix
        for (int j = 0; j < d; j++)
        {
            scanf("%d", &cof);
            squareSum += cof * cof;
            lp.set_a(j, i, cof);
        }
        lp.set_a(d, i, std::sqrt(squareSum)); // we plug in minDist as a var
        
        scanf("%d", &cof);
        lp.set_b(i, cof);
    }
    lp.set_l(d, true, 0); // r >= 0

    lp.set_c(d, -1); // minimize -r

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible())
        printf("none\n");
    else if (s.is_unbounded())
        printf("inf\n");
    else
    {
        int radius = -CGAL::to_double(s.objective_value());
        printf("%d\n", radius);
    }
}

int main()
{
    while (true)
    {
        int n;
        scanf("%d", &n);
        if (n)
            testcase(n);
        else
            return 0;        
    }
}