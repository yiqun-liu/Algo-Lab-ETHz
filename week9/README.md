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

Looking for a vertex cover for a bipartite graph

Apply the trick introduced in slides (Use residual map to make the code simple)



## Algocoön Group

Minimum Cut with arbitrary source and sink



Key insight: After we have the graph partitioned (into S and T), it doesn't matter which exact vertex in S is the source, and which exact vertex in T is the sink.

It is by this observation could we solve the problem by running maximum flow for only O(N) time. (Naively: N^2)



## Canteen

A MCMF problem



It is pretty obvious in many ways.

Information about one edge are grouped when provided.

The are exactly two units which go in parallel, reminding us of cost & capacity.



However, the intuitive implementation results in `TL` because it introduce negative cost.



Notice: cost always takes the second priority. Any change in costs does not affect the flow. We can add a constant cost to all negative-cost edges. As the max flow remain unchanged, and there is only one negative-cost edge in the original model, this modification will increase cost in all cases by `max_flow * increased_amount`, with no more side effect.



## Marathon

(Oh Lord, so much pointless story in the description... >_<)



Everyone follows the shortest path(s), but the shortest path(s) may not be unique. Therefore we must first figure out all such paths before we calculating the maximum flow...

