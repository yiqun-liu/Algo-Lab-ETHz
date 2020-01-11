# Maximum Flow



Maximum flow problem is essentially a set of inequality constrains. The name suggests that it could model well some optimization problems; besides, it is also a powerful tool for "satisfaction" problem or "existence" problem. (usually, the constrains is satisfied when the max flow is equal to a certain value)



If we let the constant amount equals the possible maximum flow (weights sum of edges going out from source, or that going to the sink), and connect (some) edge capacities with inequalities, this model can solve some constrained assignment/distribution problem. (e.g. soccer)



## Coin Tossing Tournament

A variation of Tutorial problem "soccer"



## Shopping Trip

Typical "Edge Disjoint Paths" problem.

Read the description carefully: streets in this problem is undirected.

If you use the sample code, instead of calling the `edge_adder.add_edge()` twice, I strongly suggest you extends the adder class by changing `c_map[rev_e] = 0;` into `c_map[rev_e] = capacity;`. This would keep graph sparser.



## Tetris

What led me to my solution to "Tetris" is the keyword: disjoint. The question asked us to count layouts which have different "crack" positions, and every piece may not be used twice. Therefore I started to consider the connections between this problem and disjoint edge paths problem. If the bricks are treated as "edge", then "vertices" can be connected with total length of bricks.

Everything looks exactly the same, except that this graph is a strictly directed network, and there is a "crack" constrains. If we put this constrains in disjoint edge paths problem, this implies that a node may only be visited once (not only disjoint edges but also disjoint vertices). I solved this by splitting each node into two: the unit-weighted edge ensures that only unit flow can go through each vertex.



Lessons:

- Network edge with weight 1 is very useful. It is useful in counting; it can "simulate" choose-or-not decisions; and it helps us to solve optimization problems without explicit enumerations or brute force.
- Network is a graph. It has only two kinds of elements: a) edges and b) vertices. This provides hint for modeling: just try it out. In sounds weird to model something like bricks as "edges", but if we think it through we will find they are a good match for our purpose.



## London

"London" should be simple after modeling previous problems. However, I got TL for the first time. Newspaper might be very long, but the space of possible combination is quite small. Therefore it is quite worthwhile to merge the nodes who have same set of characters, keeping the network as sparse as possible.



## Courier (PotW)

For some highly structured network (e.g. "bipartite"), the maximum flow is carrying out an implicit comparison. The output is always limited by the bottleneck, in some case the smaller one.