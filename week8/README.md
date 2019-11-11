# Triangulation (CGAL)

## Bistro

Voronoi Diagram



## Germs

In Delaunay triangulation, each point has an edge to all closest other points



Here I assigned an index to each edge

TODO: check if some edges will be iterated twice



## Graypes

In Delaunay triangulation, each point has an edge to all closest other points



(Though the description elaborates on how graypes will act if they have several closest graype, we can safely ignore it. This is the key to code a simple solution)



## H1N1

Motion planning (Voronoi diagram is construction using perpendicular **bisector**)



It is safest to walk through two infected people by their perpendicular bisector (if we cannot, then there is no way we can walk through them).

Therefore, we can do the follows:

1. Identify nearest infected person: find out which faces in Voronoi Diagram we are initially in. O(logN)
2. move to a Voronoi face border safely. This requires us to find out which "sector" of the faces we are in, this can be done by circulating through all edges adjacent to our nearest infected person and check which angles are we currently in. Typically, we could move to either of the boundary segments safely
3. move along the borders towards infinite edges. for each edge along our path, the bottleneck is always the midpoints between two adjacent vertices (infected people), which is the half of their distance. 
4. The maximum possible `d` for escape is decided by the bottleneck along one path. Thus we need to calculate minimal vertex-distance along that paths. There are many possible paths to infinite edges. We can maximize through all possibilities. This can be precomputed for all paths (I guess something like Dijkstra algorithm is most efficient: we start from infinite edges, selecting the edges with maximum possible `d` at each stage....)

It is an edge-centered algorithms, thus hard to program using `CGAL::triangulation`. (`Edge` here is simply a`std::pair<Face_handle, int>` with few operation supported). Fortunately, it can be easily switched into a face-centered algorithms: we first decide which face we are in; safely (keep Voronoi in mind and you'll know why) move to the centroid of this face, and move to an infinite face step by step.

My final solution is face-centered. There is one implementation detail worth mentioning: As `std::prority_queue` does not support key update. I use balanced tree instead. `std::set` and `std::map` is NOT the right substitution: If there are different elements with same key, their behaviors is not what we want. Use `std::multiset` and `std::multimap` and deal with the tie carefully.





