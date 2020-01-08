#include <cstdio>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a >= x + 1) a -= 1;
    return a;
}

void testcase(int n)
{
    std::vector<K::Point_2> points;
    points.reserve(n);
    for (int i = 0; i < n; i++)
    {
        double x, y;
        scanf("%lf%lf", &x, &y);
        points.push_back(K::Point_2(x, y));
    }
    
    Min_circle mc(points.begin(), points.end(), true); // randomize input for robustness
    Traits::Circle circle = mc.circle();
    
    K::FT minSquaredRadius = circle.squared_radius();
    for (int i = 0; i < n; i++)
    {
        if (circle.has_on_boundary( points[i] )) // alternative: compare dist & rad
        {
            // hide this point, avoid copying the whole vector
            const K::Point_2 copy = points[i];
            points[i] = i ? points[i - 1] : points[i + 1];

            // Reconstruct circle
            Min_circle mc_prime(points.begin(), points.end(), true); // randomize input for robustness
            Traits::Circle circle_prime = mc_prime.circle();
            K::FT squaredRadius = circle_prime.squared_radius();

            if (squaredRadius < minSquaredRadius) // Update
                minSquaredRadius = squaredRadius;
            
            // restore points set
            points[i] = copy;
        }
    }
    printf("%.0f\n", ceil_to_double( CGAL::sqrt(minSquaredRadius) ));
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