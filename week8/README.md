# Triangulation (CGAL)

## Bistro

**Voronoi Diagram**

Hello World problem.



## Graypes

**Nearest Neighbor Graph**

In Delaunay triangulation, each point has an edge to all closest other points. 

Although the description elaborates on how graypes will act if they have several closest graype, we can safely ignore it.



## Germs

**Nearest Neighbor Graph**

In Delaunay triangulation, each point has an edge to all closest other points



## H1N1

**Motion planning** (Voronoi diagram can be viewed as constructed using perpendicular bisector)



Jan. 14th: I implemented a new version. It seems that I am stuck with my stereotype with Dijkstra. The new code is simpler and slightly faster. Here are the differences:

- As all heap-like structure in STL lacks update-key support, we allow the duplicate elements, and skip it when it is visited the second time.
- `->info()` is now used to store squared distance rather than index. Index is not necessary for this problem

---

It is safest to walk through two infected people by their perpendicular bisector (if we cannot, then there is no way we can walk through them).

Therefore, we can do the follows:

1. Identify nearest infected person: find out which faces in Voronoi Diagram we are initially in. O(logN)
2. move to a Voronoi face border safely. This requires us to find out which "sector" of the faces we are in, this can be done by circulating through all edges adjacent to our nearest infected person and check which angles are we currently in. Typically, we could move to either of the boundary segments safely
3. move along the borders towards infinite edges. for each edge along our path, the bottleneck is always the midpoints between two adjacent vertices (infected people), which is the half of their distance. 
4. The maximum possible `d` for escape is decided by the bottleneck along one path. Thus we need to calculate minimal vertex-distance along that paths. There are many possible paths to infinite edges. We can maximize through all possibilities. This can be precomputed for all paths (I guess something like Dijkstra algorithm is most efficient: we start from infinite edges, selecting the edges with maximum possible `d` at each stage....)

It is an edge-centered algorithms, thus hard to program using `CGAL::triangulation`. (`Edge` here is simply a`std::pair<Face_handle, int>` with few operation supported). Fortunately, it can be easily switched into a face-centered algorithms: we first decide which face we are in; safely (keep Voronoi in mind and you'll know why) move to the centroid of this face, and move to an infinite face step by step.

My final solution is face-centered. There is one implementation detail worth mentioning: As `std::prority_queue` does not support key update. I use balanced tree instead. `std::set` and `std::map` is NOT the right substitution: If there are different elements with same key, their behaviors is not what we want. Use `std::multiset` and `std::multimap` and deal with the tie carefully.