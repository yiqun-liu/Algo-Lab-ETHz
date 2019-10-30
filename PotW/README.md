# Problem of the Week



## Attack of the Clones

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