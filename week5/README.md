# Greedy Algorithms, Brute Force (Split & List)



## Boats

**Greedy Algorithms**

A variation of Interval scheduling problem



## Octopussy

**Greedy Algorithms**



## Punch

**Dynamic Programming**

Similar to Knapsack problem, a very typical dynamic programming problem. 



## Light at the Museum

**Brute Force (Split & List)**

The idea is clear, but (my) coding is rather tedious and need patience. A big problem is the memory size. To write efficient code I use certain kind of memoization. But keep in mind the need of memory also grows exponentially, and for this problem we even cannot do memoization with C's built-in arrays. (If split & list technique is not used) 

Lessons learned:

- Use `std::vector` when memory management might become a problem
- keep split & list in mind from the very beginning. If I did so I wouldn't have to rewrite code in ` std::vector` , which takes lots of time...
- Don't try to write iterative code if you are not fluent writing it. Recursive code and naive iterations are enough. Code efficiency becomes a problem only when the programmer's efficiency is not a problem...



## Planks

**Brute Force (Split & List)**

It takes me a long time to realize that it is essentially the same problem as "light of the museum".

I was trying to assign planks to each sides. This is hard because number of planks of each side varies, depending on your choice. Backtracking is a seemingly possible way, but I think it needs a lot of effort to code it correctly and elegantly.

For this problem, it is much easier to decide "sides" for each plank, as we only have constant decisions to make. We can encode all possibilities via 4-base encoding. I reused a little computations by computing it in BFS order, like what I do in "light of the museum".
