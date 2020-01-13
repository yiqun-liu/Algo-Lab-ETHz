#include <cstdio>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const int X = 0, Y = 1, Z = 2; // variable indices

void maximize()
{
    int a, b;
    scanf("%d%d", &a, &b);

    Program lp(CGAL::SMALLER, true, 0, false, 0); // lower bound = 0
    
    // Set linear constriants
    lp.set_a(X, 0, 1);  lp.set_a(Y, 0, 1); lp.set_b(0, 4); // x + y <= 4
    lp.set_a(X, 1, 4);  lp.set_a(Y, 1, 2); lp.set_b(1, a * b); // 4x + 2y <= ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); // -x + y <= 1

    // Set objective function
    lp.set_c(X, a); lp.set_c(Y, -b); // Maximize(by - ax) = Minimize(ax - by)

    // Solve
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) // no solution
        printf("no\n");
    else if (s.is_unbounded())
        printf("unbounded\n");
    else
        printf("%d\n", -(int)std::ceil( CGAL::to_double(s.objective_value()) ));

}

void minimize()
{
    int a, b;
    scanf("%d%d", &a, &b);
    Program lp(CGAL::LARGER, false, 0, true, 0);

    // Set linear constraints
    lp.set_a(X, 0, 1);  lp.set_a(Y, 0, 1);                    lp.set_b(0, -4);     // x + y >= -4
    lp.set_a(X, 1, 4);  lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a * b); // 4x + 2y + z >= -ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1);                    lp.set_b(2, -1);      // -x + y >= 1
    
    // Set objective function
    lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1); // Minimize(ax + by + z)

    // Solve
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) // no solution
        printf("no\n");
    else if (s.is_unbounded())
        printf("unbounded\n");
    else
        printf("%d\n", (int)std::ceil( CGAL::to_double(s.objective_value()) ));
}

int main()
{
    int p;
    while (true)
    {
        scanf("%d", &p);
        if (p == 0)
            break;
        else if (p == 1)
            maximize();
        else
            minimize();
    }
}
