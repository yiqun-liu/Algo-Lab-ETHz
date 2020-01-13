#include <cstdio>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n)
{
    int d;
    scanf("%d", &d);

    Program lp(CGAL::SMALLER, false, 0, false, 0); // no variable bounds

    // variable: (0, 1, ... d-1) - coordinates of center, [d] - radius
    const int radius_idx = d;
    lp.set_l(radius_idx, true, 0); // radius >= 0
    lp.set_c(radius_idx, -1); // minimize (-radius)

    // Set linear constraints
    for (int i = 0; i < n; i++) // plane
    {
        int squareSum = 0;
        for (int j = 0; j < d; j++) // variable
        {
            int a;
            scanf("%d", &a);
            lp.set_a(j, i, a);
            
            squareSum += a * a;
        }
        int norm = sqrt(squareSum); // int is fine
        lp.set_a(radius_idx, i, norm);

        int b;
        scanf("%d", &b);
        lp.set_b(i, b);
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible())
        printf("none\n");
    else if (s.is_unbounded())
        printf("inf\n");
    else // optimal
        printf("%d\n", (int)-CGAL::to_double(s.objective_value()) );
    
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
            break;
    }
    return 0;
}