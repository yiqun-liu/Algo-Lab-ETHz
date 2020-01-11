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

As for the coding, "light of the museum" is more difficult, because now we only have to deal with 4 sides. In that problem number of room is variable, and we have to deal with the duplicates.

But instructors consider this problem as an advanced exercise. The reason is at the modeling part. I think the most obvious hint is the input specifications (n <= 20, indicating brute force is the intended solution)... The description talking about what is "really different" really misleads me.

A key problem how to enumerate all possibilities. Note that each plank has limited, enumerable state (belongs to the 1st , 2nd, 3rd or 4th side), thus it's natural to make breakthrough at this point. If we consider a plank-layout as plank-to-side assignment (which is very similar to 0-1 assignment in Knapsack problem and "light of the museum"), and enumerate such assignment, we can see that:

- there is no **permutations of the planks within a side** - it is directly a combination at this level, nice.
- **permutations of the sides** will be done. But fortunately, we know how many times we counted a single plank-layout: 4!=24

This is exactly we want.



By the way, redundancy handling appears to be important in my implementation (It is not surprsing, as the same layout repeats 24 times). Also, although the final output is guaranteed to <= 2^31, as there are redundancies, our counting results can go over this limit, resulting in overflow. Use `long long` or `double`.