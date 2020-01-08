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
    
    printf("%.0f\n", ceil_to_double( CGAL::sqrt(circle.squared_radius()) ));
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