# Triangulation



## Concepts & Properties

- planar graph: edges intersect only at their ends
- maximal planar graph: no edges can be added without violating its planarity
- **triangulation**: planar graph in which every face is bounded by three edges (including its infinite face)
  - always maximal planar
  - exterior of the convex hull is triangulated by edges connecting "infinite vertex"



### Empty Disk

- Empty Disk: Given n points, a circle that does not contain any. We are interested in large empty disk, especially the maximal one.

- The empty disk is maximal only if its boundary passes through **three** points, otherwise it could keep growing
  - There are always arbitrary large circles outside the convex hull
  - **Delaunay triangulation** captures all maximal disks; Also, all circumcircles of triangles in Delaunay triangulation is an empty disk
  - inclusion-maximal empty disk: consider we have an initial disk. we can enlarge this disk by "gradually" increasing its radius and "gradually" moving its center. An "inclusion" disk must contains the initial disk. If the empty disk cannot grow any larger while also fully contains the initial disk, it is called "inclusion-maximal"
    - if the initial center is inside the convex hull --> the inclusion-maximal empty disk passes through **two** points



### Delaunay Triangulation

- can be defined as: the triangulation given by all triples of points whose circumcircle is empty
- properties:
  - maximizes the smallest angle among all triangulations
  - contains the Euclidean **minimum spanning tree** & the **nearest neighbor graph**.
    - circumdisk of any edge in EMST must be empty 
      - suppose another point is in the circle. the point and diameter forms a triangle, with two sides shorter than the diameter. we can always improve the spanning tree by replacing diameter with one of these sides.
    - EMST is contained --> every vertex must have been connected to its nearest neighbor
      - also prove by edge-replacing
  - unique if the points are in general position(No three points collinear and no four points co-circular).
  - can be constructed efficiently (O(NlogN) for 2D triangulation).
  - It is the straight line dual of **Voronoi Diagram**



### Voronoi Diagram

- relation with Delaunay Triangulation: straight line dual
  - Delaunay vertex ≅ Voronoi face, Delaunay triangle ≅ Voronoi vertex.
  - Voronoi vertex is at the intersection of all perpendicular bisectors of a Delaunay triangle side.
    - the perpendicular bisectors of a Delaunay edge connects two Voronoi vertices; the edge is shared by their two corresponding faces. (dual edge)
  - Voronoi vertex is the circumcircle center of corresponding triangle
- Voronoi edges are all on perpendicular bisectors of a Delaunay edge, therefore they mark the boundaries where the point has equal distance to two Delaunay vertex.
  - In every face of Voronoi, there is only one Delaunay vertex inside. This is always the closest Delaunay vertex to all points within this face.



> - the interior of a Voronoi region, there is one closest point from P; 
>
> - in the relative interior of a Voronoi edge, there are two closest points from P;
>
> - on a Voronoi vertex, there are three (or more) closest points from P.
>
>   
>
>   A Delaunay edge is a convex hull edge <=> its dual Voronoi edge is a ray.



Application:

- Nearest post office: with Delaunay triangulation, time complexity reduce from O(N) to O(logN)
- Motion planning: stay as far as possible to a set of vertices --> walk on Voronoi edges





## API

Triangulations only involves construction of segment and triangles, which are trivial and not numerically risky, therefore inexact `CGAL` kernel (`Epic`) is the common choice.

``` c++
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
```



### Vertex, Edge and Face

**Face**

CGAL’s triangulation data structure is vertex/face based. Edges are not explicitly represented

Assume we have a face handle `f`, then we can access related vertices and faces by

- vertex: `f.vertex(0)`, `f.vertex(1)`, `f.vertex(2)`
- face: `f`, `f.neighbor(0)`, `f.neighbor(1)`. `f.neighbor(2)`

![Triangulation Data Structure Illusration](img/triangulation.png "Triangulation Data Structure Illusration")

The indices grow counter-clockwise. Also,  `f.vertex(i)` is <u>opposite</u> to `f.neighbor(i)` and edge i (`std::pair(f,i)`). 

As we can see, Delaunay triangulation brought no new geometric information -  they are still stored at vertices. Faces, are just grouped vertices who know their neighboring edges/faces. Edges are merely grouped face and vertex, with data type `std::pair<Face_handle,int>`.



**Vertex**

Vertices in triangulation are in the type of `Triangulation::Vertex_handle`.

It can be directly converted into a CGAL point using `.point()` member function:

```c++
K::Point_2 point = v->point();
```



We can fetch it by face

```
Triangulation::Face_handle f = ...;
Triangulation::Vertex_handle v = face.vertex( index );
```



Generally we have no need to iterate through vertices. Because we already know all of them before we could construct triangulation.



**Edge**

`Triangulation::Edge` is just another name for `std::pair<Face_handle,int>`, where the first element is face handle, and the second element is the vertex index within that face.

These make it possible to indirectly convert it into a CGAL segment:

```c++
// Triangulation::Edge is just std::pair<Face_handle,int>
Triangulation::Edge e = ...; // whatever

// First get the end point (note we use -> operator)
Triangulation::Vertex_handle v1 = e.first->vertex((e.second + 1) % 3);
Triangulation::Vertex_handle v2 = e.first->vertex((e.second + 2) % 3);
// Then you can constrcut segment, ray, etc.
K::Segment_2 segment( v1->point(), v2->point() );
```



It is useful to circulate through all edges incident to a specific vertex(iterator in cyclic manner)

Here, [c,c) denotes the full circular sequence, while in iterator world it is empty.

```c++
Triangulation::Vertex_handle v = ...;
Triangulation::Edge_circulator c = t.incident_edges(v);
do {
	if (t.is_infinite(c)) { ... }
		...
} while (++c != t.incident_edges(v));
```



**Additional information to faces/vertices**

The additional information can be many type `T`(an `enum` type for color, an `int` type for indices, etc).

It is read/written through `handle.info()`.



Assign to faces

```c++
typedef CGAL::Triangulation_vertex_base_2<K> Vb; // vertex: no additional info
typedef CGAL::Triangulation_face_base_with_info_2<T,K> Fb; // face: T
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation; // More parameters
```

Vertices info

```c++
typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<T, K>           Vb; // vertex: T
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;
```



Then we can do read and write through `handle.info()` , `iterator->info()` and `circulator->info()`:

```c++
v.info() = 123; // assignment
if (it->info() == 123)
	// do something
```



For vertices, we can also assign their `info` before triangulation by inserting `std::pair<K::Point_2, T>`, instead of  `K::Point_2`.



### Delaunay Triangulation

**Construction**

The triangulation is incrementally constructed after you specified the point set. Make sure that **set**

> Insertion of a point is done by locating a face that contains the point, and then splitting this face. If the point falls outside the convex hull, the triangulation is restored by flips.

Therefore the input order would affect efficiency of triangulation constructions.

It is recommended to insert in batch, so that libraries will sort(`CGAL::spatial_sort()`) them and ensure a good run-time:

```c++
std::vector<K::Point_2> pts;
pts = ...; // batch points in a vector

Triangulation t;
t.insert(pts.begin(), pts.end()); // 
```



If the number of points are small, it is also fine to add them one by one

```c++
t.insert(K::Point_2(x, y));
```



**Iterator**

`CGAL` provides API to iterate through all vertices, edges and faces. There are two types of iterator, one will iterate though all geometric objects, including the infinite vertex and edges/faces that extends to infinite far. The other will iterate through the convex hull.

- All-iterator: `All_vertices_iterator`, `All_faces_iterator`, `All_faces_iterator`
- Finite-iterator: `Finite_vertices_iterator`, `Finite_faces_iterator`, `Finite_faces_iterator`

```c++
typedef Triangulation::Finite_faces_iterator  Face_iterator;
```



!!! more convinient

They are iterators for handles (`Vertex_handle`, `Face_handle` ), and should not be used dereference operator `*`. The proper way for conversion is:

```c++
Triangulation t;
for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
    // NOT: handle = *f;
    handle = t.triangle(f); // do something
	// handle = t.segment(it); for edge iterator
```





### Voronoi Diagram

> for our purposes, we can extract all information needed from the Delaunay triangulation.

Find nearest vertex / decide Voronoi region:

```c++
handle vertex = t.nearest_vertex( K::Point_2(x, y) );
```

Find the Delaunay face?

```c++
K::Point_2 point;
F_Handle face = t.locate(point);
```



```c++
t.is_infinite(f)
```

handle issue

```
(t.segment(f, i)
```

## References

1. [Theory on the Structure and Coloring of Maximal Planar Graphs](https://arxiv.org/ftp/arxiv/papers/1603/1603.05130.pdf)
2. [Moodle Discussions](https://moodle-app2.let.ethz.ch/mod/forum/discuss.php?d=40112&lang=en)
3. CGAL docs on [Triangulation](https://doc.cgal.org/latest/Triangulation_2/classCGAL_1_1Triangulation__2.html), [Spatial Sorting](https://doc.cgal.org/latest/Spatial_sorting/index.html), and [2D Triangulation User Manual](https://doc.cgal.org/latest/Triangulation_2/index.html)

