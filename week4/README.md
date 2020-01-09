# BGL: Basic Graph Algorithms



## First Steps with BGL

'Hello world' problem for BGL: Run Minimum Spanning Tree and Dijkstra sample code.



## Ant Challenge

Understanding the description if the hardest part for this question. The procedure of establishment of private networks are is exactly the Prim's algorithm. (But we can get the same result with Kruskal, because as the description suggests, the MST is unique) So we calculate MST for each species, and get a new graph based on these MSTs. With the new graph, we run Dijkstra and we are done.

The complicated parts is how we get the final graph after computing MSTs. I experimented three ways

- direct update: for each edge in each MST, I check in the final graph if the same edge already exists and has greater weight. If so, the weight is updated.
- hash map: In essence, previous method updates in adjacency lists. This time I use a C++ `unordered_map`.
- duplicate edges: All edges of MST is added to the final graph. Dijkstra works while with duplicate edges, so the correctness is still guaranteed.

I observed no significant differences among them on OJ (running time differences <= 0.01s for all test set).  `unordered_map` with custom key needs more codes. Therefore, I suggest the first and the last for their simplicities.



## Buddy Selection

An obvious maximum matching matching problem. To answer this question, we should only consider pairs of students who share at least `f+1` characteristics. Otherwise the matching information will not be helpful. I use hash map here once again. Sequential search is also an alternative.



## Shortest Paths

Solution provided.