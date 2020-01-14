# Algorithms Lab@ETHz, HS19

My own solutions to Algo-lab exercises.

All codes have passed online judge.



## Index

### Week 1

Introduction & **precomputing** (e. g. partial sum)

- Build The Sum ("hello world" problem)
- Dominoes (Remind us always reads all inputs, or inputs to the following testcase will be messed up)
- Even Pairs (Tutorial problem, precomputing)
- Even Matrices (Submatrix problem, precomputing)



### Week 2

**Dynamic programming** & **sliding window**

- Search Snippets (sliding window)
- Beach Bars (sliding window)
- Burning Coins From Two Sides (dynamic programming)
- The Great Game (dynamic programming)



### Week 3

Solving problems with CGAL library: **Basic Computational Geometric Algorithms**

- Hit (Intersection)
- First Hit (Intersection)
- Antenna (Minimum Enclosing Circles)
- Almost Antenna (Minimum Enclosing Circles)



### Week 4

Solving graph problem with BGL library: **Basic Graph Algorithms**

- First Step with BGL (MST, Dijkstra)
- Ant Challenge (MST, Dijkstra)
- Buddy Selection (Maximum Matching)
- Shortest Paths (Dijkstra)



### Week 5

**Greedy** Algorithms, **Brute Force** Algorithms (and Split & List Technique)

- Boats (Greedy)
- Octopussy (Greedy)
- Punch (Dynamic Programming)
- Light at the Museum  (Brute Force: Split & List)
- **Planks (Brute Force: Split & List)**



### Week 6

Solving graph problem with BGL library: **Maximum Flows**

- Coin Tossing Tournament(A variation of "Soccer Prediction" in the tutorial)
- Shopping Trip (Edge Disjoint Paths)
- Tetris (Edge Disjoint Paths)
- London (Construct smaller network for efficiency)



### Week 7

Solving problems with CGAL library: **Linear Programming**

- Maximize it!
- Diet
- Inball
- Radiation Therapy



### Week 8

Solving graph problems with CGAL library: **Triangulation**, **Nearest Vertex**, **Motion Planning**

- Bistro (Voronoi Diagram)
- Germs (Nearest Adjacent Vertex)
- Graypes (Nearest Adjacent Vertex)
- H1N1 (Motion Planning)



### Week 9

More topics on maximum flow: **Minimum Cut, {Maximum Matching, Maximum Independent Set, Minimum Vertex Cover} of Bipartite Graph,  Minimum Cost Maximum Flow**

- Satellites (Minimum Vertex Cover)
- Algocoön Group (Minimum Cut)
- Canteen (Minimum Cost Maximum Flow)
- Marathon (Dijkstra & Maximum Flow)



### Problem of the Week (PotW)

1.  Deck of Cards (Sliding Window)
2. Attack of the Clones (Greedy, intricate details)
3.  Cantonal Courier (Maximum network flow, hard to model it as flow problem)
4.  Suez (Linear programming)
5.  Golden Eye (Delaunay triangulation & Union Find)



### Online Judge

Some personal experience

- A non-void function without a return statement works fine on my own PC, but sometimes caused a TL in OJ
  - if OJ result is not as expected, compile with `-Wall` option
- C's `qsort` once caused a `forbidden` error. However, C++'s `std::sort` always works fine...
- When your needs a lot of memory, use `std::vector` instead of C's array



## TODO

- PotW - "From Russia With Love" (also TA's idea)

- Rewrite PotW - "Attack of the Clones" using Yu Fei's idea
- Use binary search to accelerate W7 - radiation
- TA's comment on "london" --> README