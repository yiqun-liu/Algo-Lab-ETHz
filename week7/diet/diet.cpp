#include <cstdio>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// helper function: translate index
int minRow(int nutrient) { return 2 * nutrient; }
int maxRow(int nutrient) { return 2 * nutrient + 1; }

// idea: what we need to do is just transpose the input
void testcase(int nNutrients, int nFoods)
{
    Program lp(CGAL::SMALLER, true, 0, false, 0); // lower bound: 0

    for (int i = 0; i < nNutrients; i++) // nutrient ranges
    {
        int minVal, maxVal;
        scanf("%d%d", &minVal, &maxVal);

        lp.set_b( minRow(i), -minVal ); // negate all min rows because we use <=
        lp.set_b( maxRow(i), maxVal );
    }

    for (int i = 0; i < nFoods; i++) // available food
    {
        int price;
        scanf("%d", &price);
        lp.set_c( i, price ); // objective function: minimize total cost
        
        for (int j = 0; j < nNutrients; j++)
        {
            int amount;
            scanf("%d", &amount);
            lp.set_a( i, minRow(j), -amount );
            lp.set_a( i, maxRow(j), amount );
        }
    }

    // Solve
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_optimal())
        printf("%d\n", (int)std::floor(CGAL::to_double(s.objective_value())) );
    else // there will be no unbounded case, accroding to the specifications
        printf("No such diet.\n");
}

int main()
{
    while (true)
    {
        int n, m;
        scanf("%d%d", &n, &m);
        if (n == 0 && m == 0)
            break;
        testcase(n, m);
    }
    return 0;
}