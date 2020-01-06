# Sliding Window & Dynamic Programming



## Search Snippets

**sliding window**

Merge those word list and then run a simple sliding window scan. The simplest way to merge is **concatenate those word list and run `std::sort`** (complexity: O( N\*M\*log(N\*M)) ). However, this ignores the fact that every list are already sorted. A somewhat faster way to **merge sorted list**, like the second phase of merge sort. I implemented k-way merge using `std::priority_queue` (complexity:  O( N\*M\*logN )).

The latter's implementation is much more complicated, but the gain is not significant (less than 0.05s for the biggest testcase on OJ). Therefore I **suggest trying the former first** in examination.



## Beach Bars

**sliding window**

Sort the sequence and then run a typical sliding window scan. There is a trap when dealing with the integral midpoint(s) of intervals (Remember to consider whether the interval length is an even number. When it is an odd number, there is two midpoints to calculate. If you calculate using `(right-left)/2`, pay attention to its sign. ). The scan is linear and it is the sort that bounds the time complexity.



## Burning Coins From Two Sides

**dynamic programming**

The solution is clear after you understand what the problem is asking. Actually what the player and its opponent do are symmetric.

While coding the iterative (bottom-up) solution, I got a `SEG-FAULT` due to out of bounds index. Pay more attention to bound-check as we're constructing the solution to bigger problems from the smaller one, and the number of subproblems might vary for boundary problems. 

This is not a problem for the recursive solution, though.



## The Great Game

**dynamic programming**

Regular DP does not works well with this problem. There would be `N *N  * 2` states (three dimensions refer to the positions of two meeples and whose turn it is), which is impractical to calculate with this `N = 5 * 10^4` scale.

I was hinted by one sentence:

> Remark: The two meeples are allowed to be at the same position.

It is not really a game. The movement of two meeples are actually independent, and we can calculate it one by one. The number of states reduces to `O(N)` scale.

Time complexity: it is like a BFS on an DAG, `O(|V|+|E|)` it is.



A trick for recursive DP with memoization. For the base case, it is common to use an `if` statement. A faster way is to directly set the memoization array (so that the base case result is fetched in "already computed" check), but this might not be a good practice in the software engineering perspective.