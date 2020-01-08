# CGAL: Computational Geometric



## Hit

'Hello world' problem for CGAL.



## First Hit

Idea: Avoid unnecessary intersection constructions by reuse computed results. For this problem, we 'shorten' the ray by recording the nearest intersection point. 

`CGAL::to_double` is not accurate enough for larger scale. (My initial version used `std::floor(CGAL::to_double(x))`, and results in `WA` when the |x| goes beyond 2^51) Use the enhanced `floor_to_double` function here.

The last testcase contains adversarial input, so we need to shuffle our data in some way.



## Antenna

A simple application of Minimum Enclosing Circle



## Almost Antenna

The Minimum Enclosing Circle is decided by the points lies on its boundaries.

Avoid calling `CGAL:sqrt` repeatedly.