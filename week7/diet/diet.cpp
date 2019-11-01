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
    int m;
    scanf("%d", &m);

    Program lp (CGAL::SMALLER, true, 0, false, 0); // lower bound: 0

    int minAmount, maxAmount;
    for (int i = 0; i < n; i++)
    {
        scanf("%d%d", &minAmount, &maxAmount);
        lp.set_b(2 * i, -minAmount);
        lp.set_b(2 * i + 1, maxAmount);
    }
        
    int amount, price;
    for (int i = 0; i < m; i++) // food
    {
        scanf("%d", &price);
        lp.set_c(i, price);
        for (int j = 0; j < n; j++) // nutrient
        {
            scanf("%d", &amount);
            lp.set_a(i, 2 * j, -amount);
            lp.set_a(i, 2 * j + 1, amount);
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_optimal())
        printf("%d\n", (int)CGAL::to_double(s.objective_value()));
    else
        printf("No such diet.\n");
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