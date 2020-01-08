#include <cstdio>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

// sample code: recommended way to round down to an integer
double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return a;
}

void testcase(int n)
{
    double x0, y0, x1, y1;
    scanf("%lf%lf%lf%lf", &x0, &y0, &x1, &y1);

    P origin(x0, y0);
    R ray(origin, P(x1, y1));
    S ray_segment;
    
    std::vector<S> obstacles;
    obstacles.reserve(n);

    bool hit = false;
    for (int i = 0; i < n; i++)
    {
        double u0, v0, u1, v1;
        scanf("%lf%lf%lf%lf", &u0, &v0, &u1, &v1);

        obstacles.push_back( S(P(u0, v0), P(u1, v1)) );
    }

    // In case we encounter bad inputs
    std::random_shuffle( obstacles.begin(), obstacles.end() );

    for (int i = 0; i < n; i++)
    {
        S &obstacle = obstacles[i];
        if ( (!hit && CGAL::do_intersect(ray, obstacle)) ||
             CGAL::do_intersect(ray_segment, obstacle) )
        {
            hit = true;
            auto o = CGAL::intersection(ray, obstacle);
            if (const P* op = boost::get<P>(&*o)) // intersection: point
                ray_segment = S(origin, *op);
            else // intersection: segment
            {
                const S* os = boost::get<S>(&*o);
                K::FT distToSource = CGAL::squared_distance( origin, os->source() );
                K::FT distToTarget = CGAL::squared_distance( origin, os->target() );
                P closerEndpoint = distToSource < distToTarget ? os->source() : os->target();
                ray_segment = S(origin, closerEndpoint);
            }   
        }
    }

    if (hit)
        printf( "%.0f %.0f\n", 
            floor_to_double(ray_segment.target().x()), 
            floor_to_double(ray_segment.target().y()) );
    else
        printf("no\n");
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