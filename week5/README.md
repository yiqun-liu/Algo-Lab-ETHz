## Planks

It takes me a long time to realize that it is essentially the same problem as "light of the museum".

I was trying to assign planks to each sides. This is hard because number of planks of each side varies, depending on your choice. Backtracking is a seemingly possible way, but I think it needs a lot of effort to code it correctly and elegantly.

For this problem, it is much easier to decide "sides" for each planks. Life is much easier as we only have constant decisions to make. We can encode all possibilities via 4-base encoding. I reused a little computations by computing in BFS order, like what I do in "light of the museum".



Focus but do not get too focused. If A-centered thinking does not pay off, try B-centered counterparts.