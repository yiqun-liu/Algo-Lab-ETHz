# Week 9

More topics on maximum flow

- Maximum Flow Minimum Cut (MF**MC**)
- Bipartite Graph & Maximum Flow:
  - Maximum Matchings
  - Maximum Independent Set (MaxIS)
  - Minimum Vertex Cover(MinVC)

And a more generalized flow model:

- Minimum Cost Maximum Flow (**MC**MF)



## Satellites

**Vertex Cover**

Apply the trick introduced in slides (Use residual map to make code simpler)



## Algocoön Group

**Minimum Cut** with arbitrary source and sink



Key insight: After we have the graph partitioned (into S and T), it doesn't matter which exact vertex in S is the source, and which exact vertex in T is the sink. (The idea to think in set view, rather than the micro, flow view)

It is by this observation could we solve the problem by running maximum flow for only O(N) time. (Naively: N^2)



## Canteen

**Minimum Cost Maximum Flow**



An intuitive implementation results in `TL` because it involves negative cost.

Notice: cost always takes the second priority. Changes in costs never affect the value of maximum flow. Also, on every unit flow flowing from source to sink, there is one and only one edge with negative cost. With that observation, we can add a constant, positive bias to all negative cost. This modification will increase cost by `max_flow * increased_amount`, with no other side effects (the relative differences are not changed, so it do not change the preference).



## Marathon

(Oh Lord, so much pointless story in the description... >_<)



Everyone follows the shortest path(s), but the shortest path(s) may not be unique. Therefore we must first figure out all such paths before we calculating the maximum flow...

Be careful the graph is undirected when you run Dijkstra.