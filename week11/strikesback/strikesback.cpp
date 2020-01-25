#include <cstdio>
#include <vector>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

using std::vector;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase()
{
    int nParticles, nShots, nHunters, energy;
    scanf("%d%d%d", &nParticles, &nShots, &nHunters);
    scanf("%d", &energy);

    vector<K::Point_2> particles;   particles.reserve(nParticles);
    vector<int> densities;          densities.reserve(nParticles);
    for (int i = 0; i < nParticles; i++)
    {
        double x, y;
        int d;
        scanf("%lf%lf%d", &x, &y, &d);
        particles.push_back(K::Point_2(x, y));
        densities.push_back(d);
    }
    
    vector<K::Point_2> shots; shots.reserve(nShots);
    
    for (int i = 0; i < nShots; i++)
    {
        double x, y;
        scanf("%lf%lf", &x, &y);
        shots.push_back(K::Point_2(x, y));
    }

    vector<K::Point_2> hunters; hunters.reserve(nHunters);
    for (int i = 0; i < nHunters; i++)
    {
        double x, y;
        scanf("%lf%lf", &x, &y);
        hunters.push_back(K::Point_2(x, y));
    }  

    const double MAX_DOUBLE = std::numeric_limits<double>::max();
    vector<double> maxRadius_2(nShots, MAX_DOUBLE); // squared radius

    if ( nHunters )
    {
        Triangulation t_hunters;
        t_hunters.insert( hunters.begin(), hunters.end() );
        for (int i = 0; i < nShots; i++)
        {
            K::Point_2 nearest = t_hunters.nearest_vertex( shots[i] )->point();
            maxRadius_2[i] = CGAL::squared_distance( nearest, shots[i] );
        }

        // optional optimization: eliminate shots that cannot reach particles
        int nEffective = 0;
        Triangulation t_particles;
        t_particles.insert( particles.begin(), particles.end() );
        for (int i = 0; i < nShots; i++)
        {
            K::Point_2 nearest = t_particles.nearest_vertex( shots[i] )->point();
            double dist_2 = CGAL::squared_distance( nearest, shots[i] );
            
            if (dist_2 <= maxRadius_2[i])
            {
                shots[nEffective] = shots[i]; // nEffective <= i always holds
                maxRadius_2[nEffective] = maxRadius_2[i];
                nEffective++;
            }
        }
        // printf("%d -> %d\n", (int)shots.size(), nEffective );
        shots.resize(nEffective);
        maxRadius_2.resize(nEffective);
    }
    
    Program lp(CGAL::LARGER, true, 0, false, 0);  // all energy >= 0
    for (int i = 0; i < nParticles; i++) // row: particles
    {
        for (int j = 0; j < shots.size(); j++) // column: shot
        {
            double dist_2 = CGAL::squared_distance( particles[i], shots[j] );
            if (dist_2 > maxRadius_2[j])
                continue;
            
            if (dist_2)
                lp.set_a(j, i, 1 / IT(dist_2) ); // quotients are necessary
            else
                lp.set_a(j, i, 1);
        }
        lp.set_b(i, densities[i]);
    }

    // available energy constraints & maximum radius constraints
    for (int i = 0; i < shots.size(); i++)
        lp.set_a(i, nParticles, -1); // row index = nParticles, negated
    lp.set_b(nParticles, -energy); // negated

    // No objective
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible())
        printf("n\n");
    else
        printf("y\n");
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--) testcase();
    return 0;
}