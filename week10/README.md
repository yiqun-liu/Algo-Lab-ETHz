# Week 10: Practice



## Return of the Jedi

**Minimum Spanning Tree**

In the problem we are asked to break the tie based on index, therefore we have to implement Prim algorithms by hand, and then find a substitution for any of its edge.

The graph is dense, my searching is of O(n^3) complexity:

1. Select an edge in MST
2. Group vertices into two connected component (I used Union-Find, not DFS because of my choices of internal data structure, but this is not the bottleneck) 
3. Find the edge with minimum cost which connects two component



Be careful: the index is 1-based. Do step 3 in an efficient way (naive version is also O(n^2), but the computations is at least 4x than the pre-partitioned implementation).

