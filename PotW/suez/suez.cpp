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

void testcase()
{
    Program lp (CGAL::SMALLER, true, 1, false, 0); 

    int n, m, h, w;
    scanf("%d%d%d%d", &n, &m, &h, &w);

    std::vector<int> xNew(n), yNew(n);
    std::vector<int> xOld(m), yOld(m);

    for (int i = 0; i < n; i++)
        scanf("%d%d", &xNew[i], &yNew[i]);
    for (int i = 0; i < m; i++)
        scanf("%d%d", &xOld[i], &yOld[i]);

    int eqnNo = 0; // equation counter
    for (int i = 0; i < n; i++)
    {
        // prevent collisions between new posters, processed pairwisely
        for (int j = i + 1; j < n; j++)
        {
            int dx = abs(xNew[j] - xNew[i]);
            int dy = abs(yNew[j] - yNew[i]);
            bool b = (double)dx * h > (double)dy * w; // careful: int is not enough
            
            if (b) // maximum-extensible axis
            { lp.set_a(i, eqnNo, w); lp.set_a(j, eqnNo, w); lp.set_b(eqnNo, 2 * dx); }
            else
            { lp.set_a(i, eqnNo, h); lp.set_a(j, eqnNo, h); lp.set_b(eqnNo, 2 * dy); }
            eqnNo++;
        }

        if (!m) // If there is no old posters
        	continue;

        // prevent collision between this new poster & old posters
        int maxB, maxC;
        bool unset = true;
        for (int j = 0; j < m; j++)
        {
            int dx = abs(xOld[j] - xNew[i]);
            int dy = abs(yOld[j] - yNew[i]);
            bool b = (double)dx * h > (double)dy * w;
            
            int newB, newC;
            if (b)
            {
                newB = 2 * dx - w;
                newC = w;
            }
            else
            { 
                newB = 2 * dy - h;
                newC = h;
            }

            // update max
            if (unset || (double)newB * maxC < (double) maxB * newC) // bottleneck
            {
                maxB = newB;
                maxC = newC;
                unset = false;
            }
        }
        lp.set_a(i, eqnNo, maxC); lp.set_b(eqnNo, maxB); 
        eqnNo++;
        
        lp.set_c(i, -2 * (h + w));
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    double result = std::ceil(-CGAL::to_double(s.objective_value()));

    // output solution
    printf("%.0f\n", result);
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}