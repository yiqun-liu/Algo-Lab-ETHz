# Linear Programming



## Maximize it!

Hello World Problem to CGAL linear programming libraries.



## Diet

We can translate the input information into linear constraints by a simple transpose.



## Inball

This time we are asked to optimize an variable (radius) which does not appear in the original problem. So we must create it and adapt the given linear constraints.

My initial idea is describe the ball by center coordinates and radius. We create two sets of constraints: the first ensures the center is in the cave, while the second make sure that radius is valid (i.e. radius &le; distance to any plane). However we can see that the second set of constraints is always stronger than the first set if we ensure that radius is non-negative. Therefore the second set suffices.



## Radiation Therapy

- strict inequalities: we can do this by leaving a constant margin in the inequalities. The actual number does not affect the existence of solution: as the margin scales, the polynomial could scale in same ratio.
- data range: 2<sup>300 </sup> might show up in this problem, use `CGAL::Gmpz`
- boundaries: 0-degree polynomial is just a constant function. Don't  forget it.
- trick: there might be variable which is always 0 in the input. In this case, ignoring such variables would bring us huge gain. (>3s  -->  ~1s) 
  - It is possible to pass the online judge without resorting to this trick, but it "requires luck". `radiation_old.cpp` is my initial version. Later on I finished a version with built-in arrays and more optimizations (I expect it to be faster), but strangely it got  `TL` in one case.
  - I am very unsure if this is the intended trick, as there is no hint in the description showing that there will be lots of 0s. (Although in the tutorial we are reminded to take care of variable index... related? )
- search: the cost to verify an alternative is not constant. Therefore binary search is not a good idea, even worse than sequential search. YaoBIG managed to pass the OJ with "exponential search"

In short, this problem really confuses me. I have no idea what does it want to convey. 



Many thanks to mebottle, who told me trick.