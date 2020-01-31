# CGAL

A brief summary of algolab-related API.

**TODO Linear Programming**



## Terminologies

**Kernel**: Collection of geometric data types and operations

**Field Type**: The number type used for the underlying algebra of a kernel. (`K::FT`)

**Trait**: Collection of geometric data types and operations specific to a geometric class. A complementary library to kernel library.



Computational geometric functions we need to know can be roughly divided into two categories: predicates  and constructions.

**Predicate**s: functions or objects that return/contain an enumeration type. E. g. Boolean, orientations, etc.

**Constructions**: functions  or objects that return/contain numerical values.

Constructions is significantly slower than predicates in use.



## Kernels

There are three kernels we need to know. The differs in the precision of construction:

- `CGAL::Exact_predicates_inexact_constructions_kernel` (`Epic`)
  - predicates: exact
  - constructions: have precision of C++ `double` type (`double` is its field type)
- `CGAL::Exact_predicates_exact_constructions_kernel` (`Epec`)
  - predicates: exact
  - constructions: use **exact** number type for these operations: `+` `-` `*` `/` 
- `CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt` (`Epecsqrt`)
  - predicates: exact
  - constructions: use **exact** number type for these operations: `+` `-` `*` `/` `sqrt`

speed: `Epic` > `Epec` > `Epecsqrt`



Objects of different kernels are not compatible. If you need to work across kernels, you have to select one kernel, and construct objects which is in the other kernel, with this kernel manually. (sample: `tut3-two-kernels.cpp`)



### Utilities

Constructions below returns `K::FT` or objects some of whose fields are `K::FT` type

- `K::FT` and `double` can be directly compared if the value of `K::FT ` is within the range of `double`.
- conversions between `K::FT` and `double`
  - `double` -> `K::FT`: conversion is automatically done
  - `K::FT` -> `double`: call `CGAL::to_double(x)`
    - However, `CGAL::to_double(x)` is not precise for big numbers. To get accurate results we should do refinement based comparisons with original value (see `tut3-hello-exact.cpp`)
- square root: `CGAL::sqrt(x)`. Only available with `Epecsqrt` kernel
- squared distance: `CGAL::squared_distance(geo_object_a, geo_object_b);`. A non-trivial construction. Two parameters can be different types
  - `CGAL::squared_distance(point, segment);`



Code to refine  `CGAL::to_double(x)` (Usually we will be asked to round or ceil the answer so that it is comparable)

```c++
// recommended way to round down to an integer
double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}
```



## Basic Geometric Objects

In 2D plane, there are many types of geometric object we can use: `Vector_2`, `Direction_2`, `Line_2`, `Ray_2`, `Segment_2`, `Triangle_2`, `Iso_rectangle_2` and `Circle_2`. Below are the snippets in tutorials.



**Comparisons** (`==` and `!=`) between these types are typically well-defined (as defined mathematically).



kernel objects are **immutable** (from [Official FAQ](https://www.cgal.org/FAQ.html)):

> ... We recognize, however, that users may want this flexibility and thus are working on providing mutable kernel objects. The only way to do this currently is to construct a new point and assign it to the old one : use `p = Point_2(p.x(), p.y()+1);` as if you would like to do `p.y() += 1;`.



### Points

**Construction**: `K::Point_2 point(x, y);`

**Access properties**: `point.x();`, `point.y();`



### Lines

**Construction**: `K::Line_2 line(a, b, c);`, `K::Line_2 line(point1, point2);`, `K::Line_2 line(segment);`, `K::Line_2 line(ray);`

The internal representation is `ax+by+c=0`. Therefore constructing it from points is non-trivial.

**Useful functions**: `line.y_at_x( x )`, `line.x_at_y( y )`: works like function `y(x)`, `x(y)`

**Predicates**: `line.has_on(point)`: judge whether `point` is on the `line`.  **Also available in`Ray`, `Segment`.**



### Ray

**Construction**: `K::Ray_2 ray(origin, any_other_point);`

`K::Ray_2` are represented by two `K::Point_2`, thus this construction is trivial

**Access properties**:`ray.source()` returns the source (origin) of `ray`, `ray.point()` returns the other of `ray`. 



### Segment

**Construction**: `K::Segment_2 segment(point1, point2);`

`K::Segment_2` are represented by two `K::Point_2`, thus this construction is trivial

**Access properties**:`point1 = segment.source()`, `point2 = segment.target()`, read two endpoints of `segment`



### Circle

**Construction**: `K::Circle_2 circle(point1, point2, point3);`

**Access properties**: `circle.center()`, `circle.squared_radius()`

**Predicates**: `circle.has_on_boundary(point)`: return whether `point` is on the boundaries of `circle`





## Intersections

**Sample Code: **`tut3-intersect.cpp`



Typically there are three steps when handling intersections:

1. Check if two geometric objects intersect: `CGAL::do_intersect(object1, object2)`
2. If intersect, get the pointer to the object representing intersecting area: `CGAL::intersection(object1, object2)`
3. Decide the type of intersecting area by dynamic casting(`boost::get<T>`), then do what you want.

```c++
typedef K::Point_2 P;
typedef K::Segment_2 S;

// ...

if (CGAL::do_intersect(line, segment)) // test whether they intersect
{
    auto o = CGAL::intersection(line, segment);
    if (const P* op = boost::get<P>(&*o)) // intersecting area is a single point
        ... // do something with point pointer `op`
    else if (const S* os = boost::get<S>(&*o)) // intersecting area is a segment
        ... // do something with segment pointer `os`
}
```



Note: 

- `CGAL::intersection(object1, object2)` returns a generic wrapper class `std::result_of\<K::Intersect_2(T1, T2)\>::type`, defined in header `<type_traits>`
- `boost::get<T>` returns 0 if a variable does not contain an object of type `T`



## Minimum Enclosing Circle

**Sample Code: **`tut3-miniball.cpp`



Minimum Enclosing Circle is determined by ≤ 3 support points on its boundary.

We construct a `Min_circle_2` object with a set of `Point_2`, and then convert it into `Trais::Circle`, which we can use as `K::Circle_2`

```c++
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

...

std::vector<K::Point_2> P = ...;

Min_circle mc(P.begin(), P.end(), true); // true: randomize input for robustness
Traits::Circle c = mc.circle(); // Now can be treated as K::Circle

std::cout << c.center() << " " << c.squared_radius() << "\n";

```

This snippet runs in expected linear time.



## Hints in Input Specification

- `|x| <= 2^25`: you can calculate squared Euclidean distance using `double`. It is very likely that `CGAL` will not be used.
- `|x| <= 2^51` and the like: the final results fit in C++ `double`s, but you may need to calculate intermediate results using `CGAL`.



## IO Formatting

Usually we are asked to output `double` as integer. For `cstdio` this is generally not a problem. For `iostream` output, one solutions is to use `long long` as a workaround. 

Another is to include this snippet in the very start of program:

```c++
std::cout << std::setiosflags(std::ios::fixed); // no scientific notations
std::cout << std::setprecision(0); // no fractions following
```

