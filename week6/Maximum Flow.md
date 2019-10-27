# Maximum Flow



Maximum flow problem is essentially a set of inequality constrains. Its algorithms can solve some type of existence problem. 



If we let the constant amount equals the possible maximum flow (weights sum of edges going out from source, or that going to the sink), and connect (some) edge capacities with inequalities, this model can solve some constrained assignment/distribution problem. (e.g. soccer)



---

## Tetris

At first I was struggling building a network that constructed the layout of each layer, which were finally merged to a single sink. I was not able to express that complicated constrains using network flow model.

What led me to my solution to "Tetris" is the keyword: disjoint. The question asked us to count layouts which have different "crack" positions, and every piece may not be used twice. Therefore I started to consider the connections between this problem and disjoint edge paths problem. If the bricks are  treated as "edge", then "vertices" can be connected with total length of bricks.

Everything looks exactly the same, except that this graph is a strictly directed network, and there is a "crack" constrains. If we put this constrains in disjoint edge paths problem, this implies that a node may only be visited once. I solved this by splitting each node into two, one have all incoming edges, the other have all outgoing edges, and they are connected by a unit-weighted edge.



Network edge with weight 1 is very practical. It is useful in counting; it can "simulate" choose-or-not decisions; and it helps us to solve optimization problems without explicit enumerations or brute force.



---

## London

"London" should be simple after modeling previous problems. However, I got TL for the first time.

Rough evaluation matters. Newspaper may be long, but the space of possible combination is quite small.



Remember to merge the nodes that have same neighbors and keep the graph as sparse as possible.