/**
 * IMPORTANT: this code use `long long` type to compute cross product. 
 * Theoretically, it will NOT generate correct outputs for all data in the input space.
 * (It did pass the online jude, though :p )
 * 
 * This code is written only for curiosity. Forget it in exam preparation.
 */

#include <cstdio>

// judge p3 on which side of p1-p2
long long crossProduct(long long x1, long long y1, long long x2, long long y2, long long x3, long long y3)
{
    // cross product (q.x-p.x)(r.y-p.y)-(q.y-p.y)(r.x-p.x)
    return (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
}

void testcase(int n)
{
    long long x1, y1, x2, y2;
    scanf("%lld%lld%lld%lld", &x1, &y1, &x2, &y2);

    bool hit = false;
    for (int i = 0; i < n; i++)
    {
        long long u1, v1, u2, v2;
        scanf("%lld%lld%lld%lld", &u1, &v1, &u2, &v2);
        
        if (hit)
            continue;
        
        long long cp1, cp2, cp3; 
        cp1 = crossProduct(x1, y1, x2, y2, u1, v1);
        cp2 = crossProduct(x1, y1, x2, y2, u2, v2);
        cp3 = crossProduct(x1, y1, u1, v1, u2, v2); // needed: we're testing a ray, not a line
        
        bool colinear = cp1 == 0 || cp2 == 0;
        bool p1_left  = cp1 > 0 && cp2 < 0 && cp3 <= 0; // p1(u1, v1) is on the left side
        bool p1_right = cp1 < 0 && cp2 > 0 && cp3 >= 0; // p1(u1, v1) is on the right side

        if (colinear || p1_left || p1_right)
            hit = true;
    }

    if (hit)
        printf("yes\n");
    else
        printf("no\n");
}

int main()
{
    while(true)
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