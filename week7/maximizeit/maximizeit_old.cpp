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

// variable encodings
const int X = 0; 
const int Y = 1;
const int Z = 2;

void p1(int a, int b)
{
    static Program lp (CGAL::SMALLER, true, 0, false, 0);

    lp.set_a(X, 0,  1); lp.set_a(Y, 0,  1); lp.set_b(0, 4);     //  x + y  <= 4
    lp.set_a(X, 1,  4); lp.set_a(Y, 1,  2); lp.set_b(1, a * b); // 4x + 2y <= ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2,  1); lp.set_b(2, 1);     // -x + y  <= 1

    lp.set_c(X, a);     lp.set_c(Y, -b);                        // min(ax - by) (reversed)

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible())
        printf("no\n");
    else if (s.is_unbounded())
        printf("unbounded\n");
    else
    {
        double num = s.objective_value_numerator().to_double();
        double den = s.objective_value_denominator().to_double();
        printf("%d\n", (int)std::floor(-num / den)); // reverse back before flooring
    }
}

void p2(int a, int b)
{
    static Program lp (CGAL::LARGER, false, 0, true, 0);

    lp.set_a(X, 0,  1); lp.set_a(Y, 0,  1); lp.set_a(Z, 0,  0);  lp.set_b(0, -4);     //  x  +  y     >= -4
    lp.set_a(X, 1,  4); lp.set_a(Y, 1,  2); lp.set_a(Z, 1,  1);  lp.set_b(1, -a * b); //  4x + 2y + z >= −ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2,  1); lp.set_a(Z, 2,  0);  lp.set_b(2, -1);     //  −x +  y     >= −1

    lp.set_c(X, a);     lp.set_c(Y, b);     lp.set_c(Z, 1);      // min(ax + by + z)

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible())
        printf("no\n");
    else if (s.is_unbounded())
        printf("unbounded\n");
    else
    {
        double num = s.objective_value_numerator().to_double();
        double den = s.objective_value_denominator().to_double();
        printf("%d\n", (int)std::ceil(num / den));
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    
    int a, b, p;
    bool terminate;
    
    while (true)
    {
        scanf("%d", &p);
        if (p == 0) return 0;
        scanf("%d%d", &a, &b);
        
        if (p == 1)
            p1(a, b);
        else
            p2(a, b);
    }
}
