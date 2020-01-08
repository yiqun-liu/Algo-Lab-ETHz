#include <cstdio>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

void testcase(int n)
{
    double x0, y0, x1, y1;
    scanf("%lf%lf%lf%lf", &x0, &y0, &x1, &y1);

    R ray(P(x0, y0), P(x1, y1));

    bool hit = false;
    for (int i = 0; i < n; i++)
    {
        double u0, v0, u1, v1;
        scanf("%lf%lf%lf%lf", &u0, &v0, &u1, &v1);

        S segment(P(u0, v0), P(u1, v1));

        if (!hit && CGAL::do_intersect(ray, segment))
            hit = true;
    }

    if (hit)
        printf("yes\n");
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