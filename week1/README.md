# Precomputing



## Build The Sum

straightforward.



## Dominoes

Reminder: when running an online algorithm, take more care when you break a loop. In algolab, multiple testcases are fed to the program in a single run. You have to **read in all the input data for each testcase**, or you'll mess up the alignment of following data. This is usually an unintentional outcome of an early `break`.



## Even Pairs

The tutorial problem, detailed discussion provided in the slides

- To iteratively sum elements in a **consecutive** interval, **partial sum** is an important technique to speed up.

- Sometimes it is possible to avoid counting by calculating combinatorial numbers



My solution: an online algorithm. As we only need to consider consecutive interval, we could reuse previous results. ( `x[i:j] = concat(x[i:j-1], x[j])` ). It's easy to reuse latest result to calculate the number of all even pairs ending at position `j`.



## Even Matrices

The problem `Even Pairs` is related to this problem: Not only is it the 1d version, but also a subproblem. If we fix the row coordinates of our submatrix, say `r1`, `r2`, then this problem reduces to an `Even Pairs` problem, but now the element `x[c]` in the array is `sum(x[r1:r2, c])`. 



> 子矩阵问题，基本都是这么做：枚举个上下边界，之后压成一行 ——YaoBIG
>
> The common way to solve these submatrix problems is: Enumerate upper and lower boundaries (for row indices), and then 'press' what is between into a single row --YaoBIG



For this problem, what YaoBIG said is:

1. Enumerate all combinations of upper and lower boundaries of row indices (`r1` & `r2`)
   1. For what is between the boundaries (`x[r1:r2, c]`), press them into a single row (`temp[c]=sum(x[r1:r2, c]), for any column c`)
   2. Solve this 1d problem (`Even Pairs`)

The complexity in worst case is O(N^2 * (N + N)) = O(N^3)