#include <cstdio>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const int T = 0, X = 1, Y = 2;

void testcase()
{
    int n;
    double x0, y0;
    scanf("%lf%lf%d", &x0, &y0, &n);

    Program lp(CGAL::SMALLER, false, 0, false, 0); // no bounds
    for (int i = 0; i < n; i++)
    {
        double a, b; // related to square computations
        int c, v;
        scanf("%lf%lf%d%d", &a, &b, &c, &v);
        int sign = a * x0 + b * y0 + c > 0 ? 1 : -1;
        int den = sqrt(a * a + b * b);
        lp.set_a(T, i, v * den); lp.set_a(X, i, -a * sign); lp.set_a(Y, i, -b * sign);
        lp.set_b(i, c * sign); // v*t <= dist
    }
    lp.set_l(T, true, 0); // time >= 0;
    lp.set_c(T, -1); // minimize -time

    Solution s = CGAL::solve_linear_program(lp, ET());
    int time = (int)std::floor(-CGAL::to_double(s.objective_value()));
    printf("%d\n", time);
}

int main()
{
    int t;
    scanf("%d", &t);
    while(t--) testcase();
    return 0;
}