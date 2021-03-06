# Problem of the Week



## Deck of Cards

**Sliding Window**

**Week2's Slides contain sketch of the idea**

Precomputing with partial-sum is not enough to get the full points. Fortunately, all elements are non-negative. We can make use of this monotonous property to efficiently rule out many possibilities. For instance, if `x[0] + x[1] > k`, `x[0] + x[1] + x[2]` should not be considered. Therefore we can keep a sliding whose interval endpoints moves one way to the right, resulting in an `O(N)` time complexity.



## Attack of the Clones

**Greedy Algorithm**

I have been thinking about this problem for days. In the "input" section, Prof. Steger hinted that this problem can be solved using greedy algorithm, as in the classic "job scheduling problem". However, there are much more complicated details we have to deal with (the so called segments are linked in a circular manner, therefore we cannot process it in one-direction way). Also, the input size is very challenging. number of segments can be as large as 1e9, making it infeasible to process segment one by one.

Therefore I tried following techniques:

- store and process sparse objects (the set of segments protected by each Jedi rather than marking segments one by one), because n (num of Jedi) << m (num of segment)
  - using this can secure first 60 points of this problem
  - traverse all m can got only 30 points
- duplicate the set of segments, and "expand" some of them, so as to process them in a consistent way
  - \[a, b]: [a + m, b + m] (a <= b)
  - \[a, b]: [a , b + m] (a > b)  - "expansion"
  - after copy, we can process from position k (k <= m) to k + m in a one direction manner, everything worth attention is reconstructed. (ignore those whose b < a; the meaning of such notation is different from others, (a, b + m) is the right one to considered)
  - To convince myself of its correctness, I classify those sets of segments into two categories and thought of all possible combinations
- use "prefix-sum" tricks to count how many jedis cover certain segments



Special thanks to YaoBIG. Key parts in my solution are all his idea. However, I am not able to implement all optimizations he suggested, such as: precomputed the "successor" in greedy algorithms, and jump 2^i steps accordingly... 



## Courier

**Maximum Flow**

I came up with the solution when I tried all the possibilities to make it a network flow problem...

Some ideas:

- This is not a counting problem, 0-1 assignment makes no sense
- There is  a bipartite relationship between zone ticket and assignments. At first glance, two layer structure is most natural
- Maximum flow is good at doing logical OR, not AND.
  - At first I got stuck considering the and relationship: we need all related tickets so that we can fulfill an assignment
  - However, the reverse is an OR relationship: selecting any of related targets require the tickets for this zone to be bought
  - --> better make assignment the first layer

I just set the capacity according to my experiences and tried... then I got some interpretation of one combination. It is all luck...



## Suez

**Linear Programming**

It seems like a collision-detection problems which are really hard to tackle using linear programming. However, this is not a general one: all poster are similar to each other in shapes. The horizontal distance & vertical distance decide to what extent can these new posters "extend" themselves --> boxes collides when **both** distance of horizontal distance and vertical distance reduce below zero. Therefore, the collisions between old posters and new posters and those collisions merely between new ones are essentially the same. The former is a special case. Encoding them in a pairwise style will result in `TL`. Observe carefully and you will soon find out the equations relating to old posters can be merged into `n` inequalities, rather than `n*m`.

The problem is simple. However, I didn't get the full mark because I did a wrong estimation: though `|x|<2^24`, `|x1-x2|` might be greater than `2^24`... and an intermediate expression might be greater than `2^32`, not the `2^31` I expected...



## Golden Eye

**Delaunay Triangulation**

This problem has some similarity with motion planning introduced in the lecture but is simpler. How to efficiently determine reachability is key to this problem. I solved the first question with disjoint set. This technique is efficient for a given radius, but it doesn't reserve much information, e. g. which edges are used for a given mission. It is a powerful tool to try sth out, but we cannot do inference on it.

Fortunately, the candidate bounds are discrete. `a` and `b` are either bound by the distance between to "jammers" or the distance from `s` or `t` to the nearest jammers. Trying all "jammers distance" in ascending order is unexpectedly easy, because it is incremental modifications.