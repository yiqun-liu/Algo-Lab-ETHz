# Personal Solutions (Algo-Lab@ETHz, HS19)

I write code at computer rooms and also on my laptop, so I created this repo mainly to sync my working environment. (At least for now, I have no time to keep the repo 'tidy' and my codes sufficiently readable.)

All final version codes have passed online judge (If there are several versions of solution, the one suffixed by greatest number is the final version).



## Index

### Week 6

Solving graph problem using BGL library: **Maximum Flows**

- Coin Tossing (A variation of "Soccer Prediction" in the Slides)
- Shopping Trip (Edge Disjoint Paths)
- Tetris (Edge Disjoint Paths)
- London (Model assignment/selection as a maximum flows problem, and keep the graph small)



### Week 7

Solving problems using CGAL library: **Linear Programming**

TODO



### Week 8

Solving graph problems using CGAL library: **Triangulation**, **Nearest Vertex**, **Motion Planning**

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

1.  TODO
2. Attack of the Clones (Greedy, intricate details)
3.  Cantonal Courier (Maximum network flow, hard to model it as flow problem)
4.  Suez (Linear programming)
5.  Golden Eye (Delaunay triangulation & Union Find)



### Online Judge

Some personal experience

- A non-void function without a return statement works fine on my own PC, but sometimes caused a TL in OJ
  - always compile with `-Wall` option
- C's `qsort` once caused a 'forbidden'. However, C++'s `std::sort` works fine...



## TODO

- Upload previous codes & notes

- Rewrite PotW - "Attack of the Clones" using Yu Fei's idea
- Use binary search to accelerate W7 - radiation
- Implement TA's idea, "From Russia With Love"; TA's comment on "london" --> README