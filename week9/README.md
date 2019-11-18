# Week 9

More topics on maximum flow

- Maximum Flow Minimum Cut (MF**MC**)
- Bipartite Grpah & Maximum Flow:
  - Maximum Matchings
  - Maximum Independent Set (MaxIS)
  - Minimum Vertex Cover(MinVC)

And a more generalized flow model:

- Minimum Cost Maximum Flow (**MC**MF)



## Satellites

Looking for a vertex cover for a bipartite graph

Apply the trick introduced in slides (Use residual map to make the code simple)



## Algocoön Group

Minimum Cut with arbitrary source and sink



Key insight: After we have the graph partitioned (into S and T), it doesn't matter which exact vertex in S is the source, and which exact vertex in T is the sink.

It is by this observation could we solve the problem by running maximum flow for only O(N) time. (Naively: N^2)



## Canteen



